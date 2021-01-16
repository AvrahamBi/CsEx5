// 205937949 Avraham Bar Ilan

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} pixel;

typedef struct {
    int red;
    int green;
    int blue;
} pixel_sum;


/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2
*/
void smooth(int dim, pixel* src, pixel* dst, register int kernelScale, register int filter, register int sharp) {

    register int i, j, size = dim - 1, midRow = dim, aboveRow, belowRow, min_intensity, max_intensity, min_row, min_col, max_row, max_col, midRowJ,
    aboveRowJ, belowRowJ, temp, ii, jj, midRow1, pColor, temp1, temp2, iiLimit, jjLimit;
    pixel current_pixel, loop_pixel;
    for (i = 1; i < size; ++i) {
        aboveRow = midRow + dim;
        belowRow = midRow - dim;
        for (j = 1; j < size; ++j) {
            //initializing pixel_sum locally
            pixel_sum sum = { 0, 0, 0 };
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
                // todo check which is faster
                /*sum.green = (int)((src[midRowJ].green << 3 ) + src[midRowJ].green);
                sum.blue = (int)((src[midRowJ].blue << 3 ) + src[midRowJ].blue);
                sum.red = (int)((src[midRowJ].red << 3 ) + src[midRowJ].red);*/

                temp = midRowJ + 1;
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

                if (i - 1 > 0) {
                    temp1 = i - 1;
                }
                if (j - 1 >= 0) {
                    temp2 = j - 1;
                }
                iiLimit = i + 1;
                jjLimit = j + 1;
                if (dim - 1 < iiLimit) {
                    iiLimit = dim - 1;
                }
                if (dim - 1 < jjLimit) {
                    jjLimit = dim - 1;
                }

                // todo its four 'for' loops its most be more efficient

                midRow1 = temp1 * dim;
                for (ii = temp1; ii <= iiLimit; ++ii) {

                    for (jj = temp2; jj <= jjLimit; ++jj) {

                        loop_pixel = src[midRow1 + jj];

                        //calculating pColor only once instead of 3 times

                        pColor = (int)(loop_pixel.red + loop_pixel.green + loop_pixel.blue);

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
                    midRow1 += dim;
                }

                pixel p1 = src[min_row * dim + min_col];
                pixel p2 = src[max_row * dim + max_col];
                sum.red -= (int)(p1.red + p2.red);
                sum.green -= (int)(p1.green + p2.green);
                sum.blue -= (int)(p1.blue + p2.blue);
            }
            sum.red /= kernelScale;
            sum.green /= kernelScale;
            sum.blue /= kernelScale;

            // truncate each pixel's color values to match the range [0,255]

            if(sum.red < 0) {
                sum.red = 0;
            } else if (sum.red > 255) {
                sum.red = 255;
            }
            current_pixel.red = (unsigned char)sum.red;

            if (sum.green < 0) {
                sum.green = 0;
            }else if (sum.green > 255) {
                sum.green = 255;
            }
            current_pixel.green = (unsigned char)sum.green;

            if (sum.blue < 0) {
                sum.blue = 0;
            } else if (sum.blue > 255) {
                sum.blue = 255;
            }
            current_pixel.blue = (unsigned char)sum.blue;

            dst[midRow + j] = current_pixel;
        }
        midRow += dim;
    }
}

void doConvolution(register int kernelScale, register int filter, register int sharp) {
    //calculating size only once, reduce funciton calls (sizeof) as well
    register int size = m * n * sizeof(pixel), row, col, index1, index2, thisRow = 0;
    pixel* pixelsImg = malloc(size);
    pixel* backupOrg = malloc(size);
    //implementing called functions.
    for (row = 0; row < m; ++row) {

        index1 = thisRow;

        for (col = 0; col < n; ++col) {
            index2 = index1 + index1 + index1;
            pixelsImg[index1].red = image->data[index2];
            pixelsImg[index1].green = image->data[index2 + 1];
            pixelsImg[index1].blue = image->data[index2 + 2];

            backupOrg[index1].red = pixelsImg[index1].red;
            backupOrg[index1].green = pixelsImg[index1].green;
            backupOrg[index1].blue = pixelsImg[index1].blue;
            ++index1;
        }
        thisRow += n;
    }

    smooth(m, backupOrg, pixelsImg, kernelScale, filter, sharp);

    thisRow = 0;
    for (row = 0; row < m; ++row) {
        index1 = thisRow;
        for (col = 0; col < n; ++col) {
            index2 = index1 + index1 + index1;
            image->data[index2] = pixelsImg[index1].red;
            image->data[index2 + 1] = pixelsImg[index1].green;
            image->data[index2 + 2] = pixelsImg[index1].blue;
            ++index1;
        }
        thisRow += n;
    }
    free(pixelsImg);
    free(backupOrg);
}

void myfunction(Image* image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

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
