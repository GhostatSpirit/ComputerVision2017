#ifndef GRAYSCALEIMAGE_H
#define GRAYSCALEIMAGE_H

#include <QImage>
#include <QVector>

class GrayscaleImage
{
public:
    GrayscaleImage();
    GrayscaleImage(int width, int height, uchar init = 0);
    GrayscaleImage(const QImage& qImage);

    bool operator ==(const GrayscaleImage& right);
    bool operator !=(const GrayscaleImage& right);

    bool isValid() const;
    int width() const;
    int height() const;
    bool isBlank() const;

    GrayscaleImage clampWithMask(const GrayscaleImage& mask);

    QImage toQImage() const;

    QVector<QVector<uchar>> raw;
};

#endif // GRAYSCALEIMAGE_H
