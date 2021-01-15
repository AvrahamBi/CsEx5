
#include <stdbool.h>

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} pixel;

typedef struct {
    int red;
    int green;
    int blue;
    // int num;
} pixel_sum;


/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2
*/
void smooth(int dim, pixel* src, pixel* dst, int kernelScale, bool filter, bool sharp) {
    //using register ints to drastically improve runtime

    register int i, j, size = dim - 1, midRow = dim, aboveRow, belowRow, min_intensity, max_intensity, min_row, min_col, max_row, max_col, midRowJ,
    aboveRowJ, belowRowJ, temp;
    for (i = 1; i < size; ++i) {
        aboveRow = midRow + dim;
        belowRow = midRow - dim;
        for (j = 1; j < size; ++j) {
            //initializing pixel_sum locally
            pixel_sum sum = { 0, 0, 0 };
            pixel current_pixel, loop_pixel;
            min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
            max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
            midRowJ = midRow + j;
            aboveRowJ = aboveRow + j;
            belowRowJ = belowRow + j;
            //
            /* sharp Kernel:
            * [-1, -1, -1]
            * [-1, 9, -1]
            * [-1, -1, -1]
            * adding the values manually to avoid extra loops and funciton calls.
            */
            if (sharp) {
                sum.red = (((int)src[midRowJ].red) << 3 ) + (int)src[midRowJ].red;
                sum.green = (((int)src[midRowJ].green) << 3 ) + (int)src[midRowJ].green;
                sum.blue = (((int)src[midRowJ].blue) << 3 ) + (int)src[midRowJ].blue;
                temp = midRowJ + 1;
                //minus1 = midRowJ - 1;
                sum.red -= ((int)src[temp].red);
                sum.green -= ((int)src[temp].green);
                sum.blue -= ((int)src[temp].blue);
                temp -= 2;
                sum.red -= ((int)src[temp].red);
                sum.green -= ((int)src[temp].green);
                sum.blue -= ((int)src[temp].blue);

                sum.red -= ((int)src[aboveRowJ].red);
                sum.green -= ((int)src[aboveRowJ].green);
                sum.blue -= ((int)src[aboveRowJ].blue);
                temp = aboveRowJ + 1;
                sum.red -= ((int)src[temp].red);
                sum.green -= ((int)src[temp].green);
                sum.blue -= ((int)src[temp].blue);
                temp -= 2;
                sum.red -= ((int)src[temp].red);
                sum.green -= ((int)src[temp].green);
                sum.blue -= ((int)src[temp].blue);

                sum.red -= ((int)src[belowRowJ].red);
                sum.green -= ((int)src[belowRowJ].green);
                sum.blue -= ((int)src[belowRowJ].blue);
                temp = belowRowJ + 1;
                sum.red -= ((int)src[temp].red);
                sum.green -= ((int)src[temp].green);
                sum.blue -= ((int)src[temp].blue);
                temp -= 2;
                sum.red -= ((int)src[temp].red);
                sum.green -= ((int)src[temp].green);
                sum.blue -= ((int)src[temp].blue);
            }
                // Blur Kernel is 3x3 of 1's
            else {
                sum.red += ((int)src[midRowJ].red);
                sum.green += ((int)src[midRowJ].green);
                sum.blue += ((int)src[midRowJ].blue);
                temp = midRowJ + 1;
                sum.red += ((int)src[temp].red);
                sum.green += ((int)src[temp].green);
                sum.blue += ((int)src[temp].blue);
                temp -= 2;
                sum.red += ((int)src[temp].red);
                sum.green += ((int)src[temp].green);
                sum.blue += ((int)src[temp].blue);

                sum.red += ((int)src[aboveRowJ].red);
                sum.green += ((int)src[aboveRowJ].green);
                sum.blue += ((int)src[aboveRowJ].blue);
                temp = aboveRowJ + 1;
                sum.red += ((int)src[temp].red);
                sum.green += ((int)src[temp].green);
                sum.blue += ((int)src[temp].blue);
                temp -= 2;
                sum.red += ((int)src[temp].red);
                sum.green += ((int)src[temp].green);
                sum.blue += ((int)src[temp].blue);

                sum.red += ((int)src[belowRowJ].red);
                sum.green += ((int)src[belowRowJ].green);
                sum.blue += ((int)src[belowRowJ].blue);
                temp = belowRowJ + 1;
                sum.red += ((int)src[temp].red);
                sum.green += ((int)src[temp].green);
                sum.blue += ((int)src[temp].blue);
                temp -= 2;
                sum.red += ((int)src[temp].red);
                sum.green += ((int)src[temp].green);
                sum.blue += ((int)src[temp].blue);
            }
            if (filter) {
                //checkign min/max locally to avoid funciton calls + avoid using it every iteration of the loop
                register int temp1 = 0;
                register int temp2 = 0;
                if (i - 1 > 0) {
                    temp1 = i - 1;
                }
                if (j - 1 >= 0) {
                    temp2 = j - 1;
                }
                register int iiLimit = i + 1;
                register int jjLimit = j + 1;
                if (dim - 1 < iiLimit) {
                    iiLimit = dim - 1;
                }
                if (dim - 1 < jjLimit) {
                    jjLimit = dim - 1;
                }
                register int ii, jj;
                for (ii = temp1; ii <= iiLimit; ++ii) {
                    register int midRow = ii;
                    midRow *= dim;
                    for (jj = temp2; jj <= jjLimit; ++jj) {
                        loop_pixel = src[midRow + jj];
                        //calculating pColor only once instead of 3 times
                        register int pColor = (int)loop_pixel.red;
                        pColor += (int)loop_pixel.green;
                        pColor += (int)loop_pixel.blue;
                        if (pColor <= min_intensity) {
                            min_intensity = pColor;
                            min_row = ii;
                            min_col = jj;
                        }
                        if (pColor > max_intensity) {
                            max_intensity = pColor;
                            max_row = ii;
                            max_col = jj;
                        }
                    }
                }
                register int minIndex = min_row;
                minIndex *= dim;
                minIndex += min_col;
                register int maxIndex = max_row;
                maxIndex *= dim;
                maxIndex += max_col;
                pixel p1 = src[minIndex];
                pixel p2 = src[maxIndex];
                sum.red -= ((int)p1.red + (int)p2.red);
                sum.green -= ((int)p1.green + (int)p2.green);
                sum.blue -= ((int)p1.blue + (int)p2.blue);
            }
            sum.red /= kernelScale;
            sum.green /= kernelScale;
            sum.blue /= kernelScale;
            // truncate each pixel's color values to match the range [0,255]
            register int maxRed = sum.red;
            if(maxRed < 0) {
                maxRed = 0;
            }
            if (maxRed > 255) {
                maxRed = 255;
            }
            register int maxGreen = sum.green;
            if (maxGreen < 0) {
                maxGreen = 0;
            }
            if (maxGreen > 255) {
                maxGreen = 255;
            }
            register int maxBlue = sum.blue;
            if (maxBlue < 0) {
                maxBlue = 0;
            }
            if (maxBlue > 255) {
                maxBlue = 255;
            }
            current_pixel.red = (unsigned char)maxRed;
            current_pixel.green = (unsigned char)maxGreen;
            current_pixel.blue = (unsigned char)maxBlue;
            dst[midRow + j] = current_pixel;
        }
        // equals to midRow = i * dim but more efficent
        midRow += dim;
    }
}

void doConvolution(int kernelScale, int filter, int sharp) {
    //calculating size only once, reduce funciton calls (sizeof) as well
    register int size = m * n * sizeof(pixel);
    pixel* pixelsImg = malloc(size);
    pixel* backupOrg = malloc(size);
    //implementing called functions.
    register int row, col;
    for (row = 0; row < m; ++row) {
        register int thisRow = row * n;
        for (col = 0; col < n; ++col) {
            register int index1 = thisRow + col;
            register int index2 = 3 * index1;
            pixelsImg[index1].red = image->data[index2];
            pixelsImg[index1].green = image->data[index2 + 1];
            pixelsImg[index1].blue = image->data[index2 + 2];
            backupOrg[index1].red = pixelsImg[index1].red;
            backupOrg[index1].green = pixelsImg[index1].green;
            backupOrg[index1].blue = pixelsImg[index1].blue;
        }
    }

    smooth(m, backupOrg, pixelsImg, kernelScale, filter, sharp);

    for (row = 0; row < m; ++row) {
        register int thisRow = row * n;
        for (col = 0; col < n; ++col) {
            register int index1 = thisRow + col;
            register int index2 = 3 * index1;
            image->data[index2] = pixelsImg[index1].red;
            image->data[index2 + 1] = pixelsImg[index1].green;
            image->data[index2 + 2] = pixelsImg[index1].blue;
        }
    }
    free(pixelsImg);
    free(backupOrg);
}

void myfunction(Image* image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

    /*
    * [1, 1, 1]
    * [1, 1, 1]
    * [1, 1, 1]
    */
    //int blurKernel[3][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };

    /*
    * [-1, -1, -1]
    * [-1, 9, -1]
    * [-1, -1, -1]
    */
    //int sharpKernel[3][3] = { {-1,-1,-1},{-1,9,-1},{-1,-1,-1} };

    if (flag == '1') {
        // blur image
        doConvolution(9, 0, 0);

        // write result image to file
        writeBMP(image, srcImgpName, blurRsltImgName);

        // sharpen the resulting image
        doConvolution(1, 0, 1);

        // write result image to file
        writeBMP(image, srcImgpName, sharpRsltImgName);
    }
    else {
        // apply extermum filtered kernel to blur image
        doConvolution(7, 1, 0);

        // write result image to file
        writeBMP(image, srcImgpName, filteredBlurRsltImgName);

        // sharpen the resulting image
        doConvolution(1, 0 , 1);

        // write result image to file
        writeBMP(image, srcImgpName, filteredSharpRsltImgName);
    }
}
