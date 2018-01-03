#include "binaryimage.h"

#include <QDebug>

BinaryImage::BinaryImage(int size)
{
    QVector<QVector<bool>> bwImage(size, QVector<bool>(size, 0));
    raw = bwImage;
}

BinaryImage::BinaryImage(int width, int height)
{
    QVector<QVector<bool>> bwImage(height, QVector<bool>(width, 0));
    raw = bwImage;
}

BinaryImage::BinaryImage(const QImage &qImage)
{
    int height = qImage.height();
    int width = qImage.width();

    QVector<QVector<bool>> bwImage(height, QVector<bool>(width, 0));
    QRgb white = qRgb(255, 255, 255);
    QRgb black = qRgb(0, 0, 0);

    for(int row = 0; row < height; row++){
        for(int col = 0; col < width; col++){
            QRgb curColor = qImage.pixel(col, row);

            if(curColor == white){
                bwImage[row][col] = 1;
            } else if(curColor != black){
                qDebug() << "WARNING: QImage not BW at (" << col << ", " << row << ")";
            }
        }
    }

    raw = bwImage;
}

BinaryImage::BinaryImage(const QVector<QVector<bool> >& qVector)
{
    raw = qVector;
}

bool BinaryImage::isValid() const
{
    return raw.size() != 0 && raw[0].size() != 0;
}

int BinaryImage::width() const
{
    if(!isValid()){
        return 0;
    } else {
        return raw[0].size();
    }
}

int BinaryImage::height() const
{
    if(!isValid()){
        return 0;
    } else {
        return raw.size();
    }
}

QImage BinaryImage::toQImage() const
{
    QImage qImage(width(), height(), QImage::Format_Grayscale8);

    QColor black(0, 0, 0);
    QColor white(255, 255, 255);

    for(int row = 0; row < height(); row++){
        for(int col = 0; col < width(); col++){
            if(raw[row][col] == 1){
                qImage.setPixelColor(col, row, white);
            } else {
                qImage.setPixelColor(col, row, black);
            }
        }
    }

    return qImage;
}
