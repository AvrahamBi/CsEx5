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

// I used an array instead of the pixel_sum struct
// to make it faster

/*
* this function smooth an image, i put here the implementation of
* all the methods smooth call to, if opened loop for (if the loop iterates over few rounds)
*/
void smooth(int M, pixel* src, pixel* dst, register int kernelScale, register int isFilter, register int isSharp) {
    // register int is faster a int
    register int i, j, size = M - 1, mRow = M, upperRow, underRow, min_intensity, max_intensity, min_row, min_col, max_row, max_col, mRowJ,
            upperRowJ, underRowJ, temp, ii, jj, tempRow, tempColor, temp1, temp2, maxIi, maxJj;
    pixel current_pixel, loop_pixel;
    // initialize an array for the values of the pixel colors
    register int sum[3] = {0, 0, 0};
    // ++i is faster then i++
    for (i = 1; i < size; ++i) {
        upperRow = mRow + M;
        underRow = mRow - M;
        for (j = 1; j < size; ++j) {
            // set the values back to 0
            sum[0] = sum[1] = sum[2] = 0;
            min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
            max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
            mRowJ = mRow + j;
            upperRowJ = upperRow + j;
            underRowJ = underRow + j;
            /* sharp Kernel:
            * [-1, -1, -1]
            * [-1, 9, -1]
            * [-1, -1, -1]
            */
            // here I opened the nested for loops to make it faster
            // because the size of the kernel is constant and its 3
            if (isSharp) {
                // shifts are faster then multiplication
                sum[0] = (src[mRowJ].red << 3)  + src[mRowJ].red;
                sum[1] = (src[mRowJ].green << 3)  + src[mRowJ].green;
                sum[2] = (src[mRowJ].blue << 3)  + src[mRowJ].blue;
                // i used temp var to avoid redundant calculations
                temp = mRowJ + 1;
                sum[0] -= src[temp].red;
                sum[1] -= src[temp].green;
                sum[2] -= src[temp].blue;
                temp -= 2;
                sum[0] -= src[temp].red;
                sum[1] -= src[temp].green;
                sum[2] -= src[temp].blue;
                // the upper row in the kernel
                sum[0] -= src[upperRowJ].red;
                sum[1] -= src[upperRowJ].green;
                sum[2] -= src[upperRowJ].blue;
                temp = upperRowJ + 1;
                sum[0] -= src[temp].red;
                sum[1] -= src[temp].green;
                sum[2] -= src[temp].blue;
                temp -= 2;
                sum[0] -= src[temp].red;
                sum[1] -= src[temp].green;
                sum[2] -= src[temp].blue;
                sum[0] -= src[underRowJ].red;
                sum[1] -= src[underRowJ].green;
                sum[2] -= src[underRowJ].blue;
                temp = underRowJ + 1;
                sum[0] -= src[temp].red;
                sum[1] -= src[temp].green;
                sum[2] -= src[temp].blue;
                temp -= 2;
                sum[0] -= src[temp].red;
                sum[1] -= src[temp].green;
                sum[2] -= src[temp].blue;
            } else {
                sum[0] += src[mRowJ].red;
                sum[1] += src[mRowJ].green;
                sum[2] += src[mRowJ].blue;
                temp = mRowJ + 1;
                sum[0] += src[temp].red;
                sum[1] += src[temp].green;
                sum[2] += src[temp].blue;
                temp -= 2;
                sum[0] += src[temp].red;
                sum[1] += src[temp].green;
                sum[2] += src[temp].blue;
                sum[0] += src[upperRowJ].red;
                sum[1] += src[upperRowJ].green;
                sum[2] += src[upperRowJ].blue;
                temp = upperRowJ + 1;
                sum[0] += src[temp].red;
                sum[1] += src[temp].green;
                sum[2] += src[temp].blue;
                temp -= 2;
                sum[0] += src[temp].red;
                sum[1] += src[temp].green;
                sum[2] += src[temp].blue;
                sum[0] += src[underRowJ].red;
                sum[1] += src[underRowJ].green;
                sum[2] += src[underRowJ].blue;
                temp = underRowJ + 1;
                sum[0] += src[temp].red;
                sum[1] += src[temp].green;
                sum[2] += src[temp].blue;
                temp -= 2;
                sum[0] += src[temp].red;
                sum[1] += src[temp].green;
                sum[2] += src[temp].blue;
            }
            if (isFilter) {
                // i implemented here short version of the min/max function to reduce functions calls
                // in the code to reduce running time
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
                    // i added m every iteration instead of multiply by m every iteration
                    tempRow += M;
                }
                pixel p1 = src[min_row * M + min_col];
                pixel p2 = src[max_row * M + max_col];
                sum[0] -= (p1.red + p2.red);
                sum[1] -= (p1.green + p2.green);
                sum[2] -= (p1.blue + p2.blue);
            }
            // this way of dividing is faster than:  x = x / 3  for instance:
            sum[0] /= kernelScale;
            sum[1] /= kernelScale;
            sum[2] /= kernelScale;
            // make each value of the pixel to be in the range [0, 255]
            if(sum[0] < 0) {
                sum[0] = 0;
            } else if (sum[0] > 255) {
                sum[0] = 255;
            }
            current_pixel.red = (unsigned char)sum[0];
            if (sum[1] < 0) {
                sum[1] = 0;
            }else if (sum[1] > 255) {
                sum[1] = 255;
            }
            current_pixel.green = (unsigned char)sum[1];
            if (sum[2] < 0) {
                sum[2] = 0;
            } else if (sum[2] > 255) {
                sum[2] = 255;
            }
            current_pixel.blue = (unsigned char)sum[2];
            dst[mRow + j] = current_pixel;
        }
        // added m every iteration instead of multiplication
        mRow += M;
    }
}
void doConvolution(register int kernelScale, register int isFilter, register int isSharp) {
    // i initialize all the var in the beginning to make it faster
    // i didnt use the global vars m and n because its slower
    register int M = m, N = n, size = M * N * sizeof(pixel), row, col, rowCol, threeRows, temp3 = 0;
    pixel* src = malloc(size);
    pixel* dst = malloc(size);
    // i implemented the function chars to pixels and pixels to chars here instead of call them
    for (row = 0; row < M; ++row) {
        rowCol = temp3;
        for (col = 0; col < N; ++col) {
            threeRows = rowCol + rowCol + rowCol;
            dst[rowCol].red = src[rowCol].red = image->data[threeRows];
            dst[rowCol].green = src[rowCol].green = image->data[threeRows + 1];
            dst[rowCol].blue = src[rowCol].blue = image->data[threeRows + 2];
            ++rowCol;
        }
        temp3 += N;

    }
    smooth(M, src, dst, kernelScale, isFilter, isSharp);
    temp3 = 0;
    for (row = 0; row < M; ++row) {
        rowCol = temp3;
        for (col = 0; col < N; ++col) {
            threeRows = rowCol + rowCol + rowCol;
            image->data[threeRows] = dst[rowCol].red;
            image->data[threeRows + 1] = dst[rowCol].green;
            image->data[threeRows + 2] = dst[rowCol].blue;
            ++rowCol;
        }
        temp3 += N;
    }
    free(src);
    free(dst);
}
void myfunction(Image* image1, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

    if (flag == '1') {
        // if peferred to use the flags with int instead of char
        doConvolution(9, 0, 0);
        // save the image before sharpening
        writeBMP(image1, srcImgpName, blurRsltImgName);
        // sharpen the image
        doConvolution(1, 0, 1);
        // save the image after sharpening
        writeBMP(image1, srcImgpName, sharpRsltImgName);
    }
    else {
        doConvolution(7, 1, 0);
        // save image before sharpening
        writeBMP(image1, srcImgpName, filteredBlurRsltImgName);
        // sharpen the image
        doConvolution(1, 0 , 1);
        // save image
        writeBMP(image1, srcImgpName, filteredSharpRsltImgName);
    }
}
