#ifndef SIGNEDIMAGE_H
#define SIGNEDIMAGE_H

#include <QImage>
#include <binaryimage.h>

class SignedImage
{
public:
    SignedImage(int width, int height, int init = 0);
    SignedImage();


    bool isValid() const;
    int width() const;
    int height() const;

    BinaryImage toBinaryImage() const;
    QImage toQImage() const;

    QVector<QVector<int>> raw;
};

#endif // SIGNEDIMAGE_H
