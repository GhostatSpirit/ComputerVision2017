#ifndef GRAYMORPH_H
#define GRAYMORPH_H

#include "grayscaleimage.h"
#include "binaryimage.h"

class GrayMorph
{
public:
    // treat binary image element as a "flat" element: 1 -> 0, 0 -> -infinity
    static GrayscaleImage dilate(const GrayscaleImage& image, const BinaryImage& element);
    static GrayscaleImage erode(const GrayscaleImage& image, const BinaryImage& element);

    static GrayscaleImage morphReconstruction(const GrayscaleImage& mask, const GrayscaleImage& mark, const BinaryImage& element);
    static GrayscaleImage dilateAndMask(const GrayscaleImage& mask, const GrayscaleImage& mark, const BinaryImage& element);

private:
    static bool isRowColValid(int row, int col, int width, int height);
};

#endif // GRAYMORPH_H
