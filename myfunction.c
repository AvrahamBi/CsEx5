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
* this function smooth an image
*/
void smooth(int M, pixel* src, pixel* dst, register int kernelScale, register int isFilter, register int isSharp) {

    register int i, j, size = M - 1, mRow = M, upperRow, underRow, min_intensity, max_intensity, min_row, min_col, max_row, max_col, mRowJ,
    upperRowJ, underRowJ, temp, ii, jj, tempRow, tempColor, temp1, temp2, maxIi, maxJj;
    pixel current_pixel, loop_pixel;
    for (i = 1; i < size; ++i) {
        upperRow = mRow + M;
        underRow = mRow - M;
        for (j = 1; j < size; ++j) {
            //initializing pixel_sum locally
            pixel_sum sum = { 0, 0, 0 };
            min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
            max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
            mRowJ = mRow + j;
            upperRowJ = upperRow + j;
            underRowJ = underRow + j;
            //
            /* sharp Kernel:
            * [-1, -1, -1]
            * [-1, 9, -1]
            * [-1, -1, -1]
            * adding the values manually to avoid extra loops and funciton calls.
            */
            if (isSharp) {

                sum.green = (src[mRowJ].green << 3)  + src[mRowJ].green;
                sum.blue = (src[mRowJ].blue << 3)  + src[mRowJ].blue;
                sum.red = (src[mRowJ].red << 3)  + src[mRowJ].red;

                temp = mRowJ + 1;
                sum.red -= src[temp].red;
                sum.green -= src[temp].green;
                sum.blue -= src[temp].blue;
                temp -= 2;
                sum.red -= src[temp].red;
                sum.green -= src[temp].green;
                sum.blue -= src[temp].blue;

                sum.red -= src[upperRowJ].red;
                sum.green -= src[upperRowJ].green;
                sum.blue -= src[upperRowJ].blue;
                temp = upperRowJ + 1;
                sum.red -= src[temp].red;
                sum.green -= src[temp].green;
                sum.blue -= src[temp].blue;
                temp -= 2;
                sum.red -= src[temp].red;
                sum.green -= src[temp].green;
                sum.blue -= src[temp].blue;

                sum.red -= src[underRowJ].red;
                sum.green -= src[underRowJ].green;
                sum.blue -= src[underRowJ].blue;
                temp = underRowJ + 1;
                sum.red -= src[temp].red;
                sum.green -= src[temp].green;
                sum.blue -= src[temp].blue;
                temp -= 2;
                sum.red -= src[temp].red;
                sum.green -= src[temp].green;
                sum.blue -= src[temp].blue;
            }
                // Blur Kernel is 3x3 of 1's
            else {
                sum.red += src[mRowJ].red;
                sum.green += src[mRowJ].green;
                sum.blue += src[mRowJ].blue;
                temp = mRowJ + 1;
                sum.red += src[temp].red;
                sum.green += src[temp].green;
                sum.blue += src[temp].blue;
                temp -= 2;
                sum.red += src[temp].red;
                sum.green += src[temp].green;
                sum.blue += src[temp].blue;

                sum.red += src[upperRowJ].red;
                sum.green += src[upperRowJ].green;
                sum.blue += src[upperRowJ].blue;
                temp = upperRowJ + 1;
                sum.red += src[temp].red;
                sum.green += src[temp].green;
                sum.blue += src[temp].blue;
                temp -= 2;
                sum.red += src[temp].red;
                sum.green += src[temp].green;
                sum.blue += src[temp].blue;

                sum.red += src[underRowJ].red;
                sum.green += src[underRowJ].green;
                sum.blue += src[underRowJ].blue;
                temp = underRowJ + 1;
                sum.red += src[temp].red;
                sum.green += src[temp].green;
                sum.blue += src[temp].blue;
                temp -= 2;
                sum.red += src[temp].red;
                sum.green += src[temp].green;
                sum.blue += src[temp].blue;
            }
            if (isFilter) {
                //checkign min/max locally to avoid funciton calls + avoid using it every iteration of the loop

                if (i - 1 > 0) {
                    temp1 = i - 1;
                }
                if (j - 1 >= 0) {
                    temp2 = j - 1;
                }
                maxIi = i + 1;
                maxJj = j + 1;
                if (M - 1 < maxIi) {
                    maxIi = M - 1;
                }
                if (M - 1 < maxJj) {
                    maxJj = M - 1;
                }

                tempRow = temp1 * M;
                for (ii = temp1; ii <= maxIi; ++ii) {

                    for (jj = temp2; jj <= maxJj; ++jj) {

                        loop_pixel = src[tempRow + jj];

                        //calculating  only once instead of 3 times

                        tempColor = loop_pixel.red + loop_pixel.green + loop_pixel.blue;

                        if (tempColor <= min_intensity) {
                            min_intensity = tempColor;
                            min_row = ii;
                            min_col = jj;
                        }
                        if (tempColor > max_intensity) {
                            max_intensity = tempColor;
                            max_row = ii;
                            max_col = jj;
                        }
                    }
                    tempRow += M;
                }

                pixel p1 = src[min_row * M + min_col];
                pixel p2 = src[max_row * M + max_col];
                sum.red -= (p1.red + p2.red);
                sum.green -= (p1.green + p2.green);
                sum.blue -= (p1.blue + p2.blue);
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

            dst[mRow + j] = current_pixel;
        }
        mRow += M;
    }
}

void doConvolution(register int kernelScale, register int isFilter, register int isSharp) {
    //calculating size only once, reduce funciton calls (sizeof) as well
    register int M = m, N = n, size = M * N * sizeof(pixel), row, col, rowCol, threeRows, temp3 = 0;
    pixel* src = malloc(size);
    pixel* dst = malloc(size);
    //implementing called functions.
    for (row = 0; row < M; ++row) {

        rowCol = temp3;

        for (col = 0; col < N; ++col) {
            threeRows = rowCol + rowCol + rowCol;
            src[rowCol].red = image->data[threeRows];
            src[rowCol].green = image->data[threeRows + 1];
            src[rowCol].blue = image->data[threeRows + 2];

            dst[rowCol].red = src[rowCol].red;
            dst[rowCol].green = src[rowCol].green;
            dst[rowCol].blue = src[rowCol].blue;
            ++rowCol;
        }
        temp3 += N;
    }

    smooth(M, dst, src, kernelScale, isFilter, isSharp);

    temp3 = 0;
    for (row = 0; row < M; ++row) {
        rowCol = temp3;
        for (col = 0; col < N; ++col) {
            threeRows = rowCol + rowCol + rowCol;
            image->data[threeRows] = src[rowCol].red;
            image->data[threeRows + 1] = src[rowCol].green;
            image->data[threeRows + 2] = src[rowCol].blue;
            ++rowCol;
        }
        temp3 += N;
    }
    free(src);
    free(dst);
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
