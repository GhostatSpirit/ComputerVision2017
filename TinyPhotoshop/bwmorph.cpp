#include "bwmorph.h"

#include <cmath>
#include <QDebug>

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

BinaryImage BWMorph::hitOrMiss(const BinaryImage &image, const BinaryImage &element, const BinaryImage &mask)
{
    int width = image.width();
    int height = image.height();

    int eWidth = element.width();
    int eHeight = element.height();

    int eXCenter = (eWidth - 1) / 2;
    int eYCenter = (eHeight - 1) / 2;

    int dLeft = eXCenter;
    int dRight = (eWidth - 1) - eXCenter;
    int dUp = eYCenter;
    int dDown = (eHeight - 1) - eYCenter;


    if(mask.width() != eWidth || mask.height() != eHeight){
        qDebug() << "hitOrMiss aborted: mask size does not match element";
        BinaryImage copy = image;
        return copy;
    }

    QVector<QVector<char>> masked(eHeight, QVector<char>(eWidth, -1));
    for(int i = 0; i < eHeight; ++i){
        for(int j = 0; j < eWidth; ++j){
            if(mask.raw[i][j]){
                // we care this pixel
                masked[i][j] = element.raw[i][j] ? 1 : 0;
            } else {
                // not in mask, does not care, set to -1
                masked[i][j] = -1;
            }
        }
    }

    BinaryImage newImage(width, height);

    // policy: ignore pixels on the edge
    for(int row = dUp; row < height - dDown; ++row){
//        int erowStart = 0, erowEnd = 0;
//        if(row < eYCenter){
//           erowStart = eYCenter - row;
//        }
//        if(row + eHeight - eYCenter)

        for(int col = dLeft; col < width - dRight; ++col){
            // do check & miss on this area,
            // check if every cared pixel matches

            // element row & element col
            bool canSet = true;
            for(int erow = 0; erow < eHeight; ++erow){
                for(int ecol = 0; ecol < eWidth; ++ecol){
                    char refPixel = masked[erow][ecol];

                    if(refPixel < 0){
                        // does not care about this bit
                        continue;
                    } else {
                        // care about this bit
                        int nrow = row + erow - eYCenter;
                        int ncol = col + ecol - eXCenter;

                        Q_ASSERT(isCoordValid(nrow, ncol, width, height));

                        char imagePixel = image.raw[nrow][ncol] ? 1 : 0;
                        if(imagePixel != refPixel){
                            canSet = false;
                            break;
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

    return newImage;

}

SignedImage BWMorph::distanceTransform(const BinaryImage &image, const BinaryImage& element)
{
    SignedImage distanceImage(image.width(), image.height(), 0);
    if(!image.isValid()) return distanceImage;

    int width = image.width();
    int height = image.height();

    BinaryImage eroded = image;

    while(!eroded.isBlank()){
        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){
                if(eroded.raw[i][j]){
                    (distanceImage.raw[i][j])++;
                }
            }
        }
        eroded = erode(eroded, element);
    }

    return distanceImage;
}

SignedImage BWMorph::skeletonization(const BinaryImage &image, const BinaryImage& element)
{
    SignedImage distanceImage = distanceTransform(image, element);

    int width = image.width();
    int height = image.height();

    SignedImage skeleton(width, height);

//    for(int i = 1; i < height - 1; ++i){
//        for(int j = 1; j < width - 1; ++j){
//             auto current = distanceImage.raw[i][j];

//             bool canSet = true;
//             for(int di = -1; di <= 1; ++di){
//                 for(int dj = -1; dj <= 1; ++dj){
//                     auto neighbor = distanceImage.raw[i + di][j + dj];
//                     if(neighbor > current){
//                         canSet = false;
//                         break;
//                     }
//                 }
//                 if(!canSet) break;
//             }

//             if(canSet){
//                 skeleton.raw[i][j] = distanceImage.raw[i][j];
//             }
//        }
//    }

    int eWidth = element.width();
    int eHeight = element.height();

    int eXCenter = (eWidth - 1) / 2;
    int eYCenter = (eHeight - 1) / 2;

    int dLeft = eXCenter;
    int dRight = (eWidth - 1) - eXCenter;
    int dUp = eYCenter;
    int dDown = (eHeight - 1) - eYCenter;


    // policy: ignore pixels on the edge
    for(int row = dUp; row < height - dDown; ++row){
        for(int col = dLeft; col < width - dRight; ++col){
            // do check & miss on this area,
            // check if every cared pixel matches
            auto current = distanceImage.raw[row][col];

            // element row & element col
            bool canSet = true;
            for(int erow = 0; erow < eHeight; ++erow){
                for(int ecol = 0; ecol < eWidth; ++ecol){
                    if(!element.raw[erow][ecol]){
                        continue;
                    } else {
                        int nrow = row + erow - eYCenter;
                        int ncol = col + ecol - eXCenter;

                        Q_ASSERT(isCoordValid(nrow, ncol, width, height));

                        auto neighbor = distanceImage.raw[nrow][ncol];
                        if(neighbor > current){
                            // not maxima
                            canSet = false;
                            break;
                        }
                    }

                }
                if(!canSet) break;
            }

            if(canSet){
                skeleton.raw[row][col] = current;
            }

        }
    }

    return skeleton;
}

BinaryImage BWMorph::skeletonReconstruction(const SignedImage &skeleton, const BinaryImage &element)
{
    // assume the element is a circle
    int unitSize = element.width() / 2;
    int width = skeleton.width();
    int height = skeleton.height();

    BinaryImage reconstruct(width, height);

    for(int i = 0; i < height; ++i){
        for(int j = 0; j < width; ++j){
            int distance = skeleton.raw[i][j];
            if(distance == 0)  continue;


            int newSize = unitSize * (distance - 1);

            BinaryImage disk = GetDisk(2 * newSize + 1);
            for(int di = -newSize; di <= newSize; di++){
                for(int dj = -newSize; dj <= newSize; dj++){
                    if(disk.raw[di + newSize][dj + newSize]){
                        // paint a pixel on the reconstruct
                        int ni = i + di;
                        int nj = j + dj;
                        if(isCoordValid(ni, nj, width, height)){
                            reconstruct.raw[ni][nj] = true;
                        }
                    }
                }
            }
        }
    }

    return reconstruct;
}

BinaryImage BWMorph::morphReconstruction(const BinaryImage &mask, const BinaryImage &mark, const BinaryImage &element)
{
    int width = mask.width();
    int height = mask.height();

    BinaryImage reconstruct(mark);
    BinaryImage oldReconstruct;

    while(!(oldReconstruct == reconstruct)){
        oldReconstruct = reconstruct;
        reconstruct = dilate(reconstruct, element) & mask;
    }

    return reconstruct;
}

bool BWMorph::isCoordValid(int row, int col, int width, int height)
{
    return (row >= 0 && row < height && col >= 0 && col < width);
}

BinaryImage BWMorph::GetDisk(int diameter)
{
    BinaryImage element(diameter);
    float radius = diameter / 2.0;
    float center = radius;

    for(int i = 0; i < diameter; ++i){
        for(int j = 0; j < diameter; ++j){
            float icenter = i + 0.5;
            float jcenter = j + 0.5;
            float di = icenter - center;
            float dj = jcenter - center;

            if(sqrt(di * di + dj * dj) <= radius){
                element.raw[i][j] = true;
            }
        }
    }

    return element;
}

BinaryImage BWMorph::GetSquare(int a)
{
    BinaryImage element(a, true);
    return element;
}

BinaryImage BWMorph::GetRect(int width, int height)
{
    BinaryImage element(width, height, true);
    return element;
}
