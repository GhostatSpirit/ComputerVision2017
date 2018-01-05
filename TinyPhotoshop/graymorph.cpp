#include "graymorph.h"

// treat binary image element as a "flat" element: 1 -> 0, 0 -> -infinity
GrayscaleImage GrayMorph::dilate(const GrayscaleImage &image, const BinaryImage &element)
{
    int width = image.width();
    int height = image.height();

    int eWidth = element.width();
    int eHeight = element.height();

    int eXCenter = (eWidth - 1) / 2;
    int eYCenter = (eHeight - 1) / 2;

    GrayscaleImage newImage(width, height);

    for(int row = 0; row < height; ++row){
        for(int col = 0; col < width; ++col){
            // main image loop
            auto mainVal = image.raw[row][col];
                // do dilate on this pixel
                // element row & element col
            uchar maxVal = mainVal;

            for(int erow = 0; erow < eHeight; ++erow){
                for(int ecol = 0; ecol < eWidth; ++ecol){
                    if(element.raw[erow][ecol]){
                        // compare it to the maxVal
                        int nrow = row + erow - eYCenter;
                        int ncol = col + ecol - eXCenter;


                        if(isRowColValid(nrow, ncol, width, height)){
                            uchar curVal = image.raw[nrow][ncol];
                            if(curVal > maxVal){
                                maxVal = curVal;
                            }
                        }
                    }
                }
            }

            newImage.raw[row][col] = maxVal;

        }
    }

    return newImage;
}

// treat binary image element as a "flat" element: 1 -> 0, 0 -> -infinity
GrayscaleImage GrayMorph::erode(const GrayscaleImage &image, const BinaryImage &element)
{
    int width = image.width();
    int height = image.height();

    int eWidth = element.width();
    int eHeight = element.height();

    int eXCenter = (eWidth - 1) / 2;
    int eYCenter = (eHeight - 1) / 2;

    GrayscaleImage newImage(width, height);

    for(int row = 0; row < height; ++row){
        for(int col = 0; col < width; ++col){
            // main image loop
            auto mainVal = image.raw[row][col];
            // do erosion on this pixel
            uchar minVal = mainVal;

            for(int erow = 0; erow < eHeight; ++erow){
                for(int ecol = 0; ecol < eWidth; ++ecol){
                    if(element.raw[erow][ecol]){
                        // compare it to the minVal
                        int nrow = row + erow - eYCenter;
                        int ncol = col + ecol - eXCenter;


                        if(isRowColValid(nrow, ncol, width, height)){
                            uchar curVal = image.raw[nrow][ncol];
                            if(curVal < minVal){
                                minVal = curVal;
                            }
                        }
                    }
                }
            }

            newImage.raw[row][col] = minVal;

        }
    }

    return newImage;
}

GrayscaleImage GrayMorph::morphReconstruction(const GrayscaleImage &mask, const GrayscaleImage &mark, const BinaryImage &element)
{
//    int width = mask.width();
//    int height = mask.height();

    GrayscaleImage reconstruct(mark);
    GrayscaleImage oldReconstruct;

    while(oldReconstruct != reconstruct){
        oldReconstruct = reconstruct;
        GrayscaleImage dilated = dilate(reconstruct, element);
        reconstruct = dilated.clampWithMask(mask);
    }

    return reconstruct;
}

GrayscaleImage GrayMorph::dilateAndMask(const GrayscaleImage &mask, const GrayscaleImage &mark, const BinaryImage &element)
{

    GrayscaleImage dilated = dilate(mark, element);
    dilated = dilated.clampWithMask(mask);

    return dilated;
}

bool GrayMorph::isRowColValid(int row, int col, int width, int height)
{
    return (row >= 0 && row < height && col >= 0 && col < width);
}
