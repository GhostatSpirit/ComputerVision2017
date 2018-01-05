#ifndef BINARYIMAGE_H
#define BINARYIMAGE_H

#include <QImage>
#include <QVector>

class BinaryImage
{
public:
    BinaryImage(int size, bool init = false);
    BinaryImage(int width, int height, bool init = false);
    BinaryImage(const QImage& qImage);
    BinaryImage(const QVector<QVector<bool>>& qVector);
    BinaryImage();

    BinaryImage operator+(const BinaryImage& right);
    BinaryImage operator-(const BinaryImage& right);
    BinaryImage operator&(const BinaryImage& right);

    bool operator ==(const BinaryImage& right);

    BinaryImage rotate90(bool clockwise = true);

//    BinaryImage(const BinaryImage& right){
//        raw = right.raw;
//    }

//    BinaryImage& operator =(const BinaryImage& right){
//        raw = right.raw;
//        return *this;
//    }

//    BinaryImage& operator =(BinaryImage& right){
//        raw = right.raw;
//        return *this;
//    }

    bool isValid() const;
    int width() const;
    int height() const;
    bool isBlank() const;

    QImage toQImage() const;

    QVector<QVector<bool>> raw;
};

#endif // BINARYIMAGE_H
