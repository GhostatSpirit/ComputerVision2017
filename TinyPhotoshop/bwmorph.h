#ifndef BWMORPH_H
#define BWMORPH_H

#include "binaryimage.h"

class BWMorph
{
public:
    static BinaryImage dilate(const BinaryImage& image, const BinaryImage& element);
    static BinaryImage erode(const BinaryImage& image, const BinaryImage& element);

    static bool isCoordValid(int row, int col, int width, int height);
};

#endif // BWMORPH_H
