#include "bwmorph.h"

BinaryImage BWMorph::dilate(const BinaryImage &image, const BinaryImage &element)
{
    int width = image.width();
    int height = image.height();

    int eWidth = element.width();
    int eHeight = element.height();

    int eXCenter = (eWidth - 1) / 2;
    int eYCenter = (eHeight - 1) / 2;

    BinaryImage newImage(width, height);

    for(int row = 0; row < height; ++row){
//        int erowStart = 0, erowEnd = 0;
//        if(row < eYCenter){
//           erowStart = eYCenter - row;
//        }
//        if(row + eHeight - eYCenter)

        for(int col = 0; col < width; ++col){
            // main image loop
            bool mainVal = image.raw[row][col];
            if(mainVal){
                // do dilate on this pixel
                // element row & element col
                for(int erow = 0; erow < eHeight; ++erow){
                    for(int ecol = 0; ecol < eWidth; ++ecol){

                        if(element.raw[erow][ecol]){
                            // set the pixel in newImage!
                            int nrow = row + erow - eYCenter;
                            int ncol = col + ecol - eXCenter;


                            if(isCoordValid(nrow, ncol, width, height)){
                                newImage.raw[nrow][ncol] = 1;
                            }
                        }
                    }
                }
            }

        }
    }

    return newImage;
}

BinaryImage BWMorph::erode(const BinaryImage &image, const BinaryImage &element)
{
    int width = image.width();
    int height = image.height();

    int eWidth = element.width();
    int eHeight = element.height();

    int eXCenter = (eWidth - 1) / 2;
    int eYCenter = (eHeight - 1) / 2;

    BinaryImage newImage(width, height);

    for(int row = 0; row < height; ++row){
//        int erowStart = 0, erowEnd = 0;
//        if(row < eYCenter){
//           erowStart = eYCenter - row;
//        }
//        if(row + eHeight - eYCenter)

        for(int col = 0; col < width; ++col){
            // main image loop
            bool mainVal = image.raw[row][col];
            if(mainVal){
                // do erode on this area
                // element row & element col
                bool canSet = true;
                for(int erow = 0; erow < eHeight; ++erow){
                    for(int ecol = 0; ecol < eWidth; ++ecol){

                        if(element.raw[erow][ecol]){
                            // check if the corresponding pixel is white
                            int nrow = row + erow - eYCenter;
                            int ncol = col + ecol - eXCenter;

                            if(!isCoordValid(nrow, ncol, width, height)){
                                canSet = false;
                            } else if(!image.raw[nrow][ncol]){
                                canSet = false;
                            }
                        }
                    }
                    if(!canSet) break;
                }

                if(canSet){
                    newImage.raw[row][col] = true;
                }
            }

        }
    }

    return newImage;
}

bool BWMorph::isCoordValid(int row, int col, int width, int height)
{
    return (row >= 0 && row < height && col >= 0 && col < width);
}
