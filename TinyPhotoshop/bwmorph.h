#ifndef BWMORPH_H
#define BWMORPH_H

#include "binaryimage.h"
#include "signedimage.h"

class BWMorph
{
public:
    static BinaryImage dilate(const BinaryImage& image, const BinaryImage& element);
    static BinaryImage erode(const BinaryImage& image, const BinaryImage& element);
    static BinaryImage hitOrMiss(const BinaryImage& image, const BinaryImage& element, const BinaryImage& mask);

    static SignedImage distanceTransform(const BinaryImage& image, const BinaryImage& element);
    static SignedImage skeletonization(const BinaryImage& image, const BinaryImage& element);
    static BinaryImage skeletonReconstruction(const SignedImage& skeleton, const BinaryImage& element);

    static bool isCoordValid(int row, int col, int width, int height);

    static BinaryImage GetDisk(int diameter);
    static BinaryImage GetSquare(int a);
    static BinaryImage GetRect(int width, int height);
};

#endif // BWMORPH_H
