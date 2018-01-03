#ifndef BINARYIMAGE_H
#define BINARYIMAGE_H

#include <QImage>
#include <QVector>

class BinaryImage
{
public:
    BinaryImage(int size);
    BinaryImage(int width, int height);
    BinaryImage(const QImage& qImage);
    BinaryImage(const QVector<QVector<bool>>& qVector);

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

    QImage toQImage() const;

    QVector<QVector<bool>> raw;
};

#endif // BINARYIMAGE_H
