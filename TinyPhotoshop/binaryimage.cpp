#include "binaryimage.h"

#include <QDebug>

#include "binaryinspector.h"
#include "geometryinspector.h"

BinaryImage::BinaryImage(int size, bool init)
{
    QVector<QVector<bool>> bwImage(size, QVector<bool>(size, init));
    raw = bwImage;
}

BinaryImage::BinaryImage(int width, int height, bool init)
{
    QVector<QVector<bool>> bwImage(height, QVector<bool>(width, init));
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

BinaryImage::BinaryImage()
{
    QVector<QVector<bool>> bwImage(0, QVector<bool>(0, 0));
    raw = bwImage;
}

BinaryImage BinaryImage::operator+(const BinaryImage &right)
{
    BinaryImage newImage(this->width(), this->height());
    int colMax = std::min(this->width(), right.width());
    int rowMax = std::min(this->height(), right.height());

    for(int i = 0; i < rowMax; ++i){
        for(int j = 0; j < colMax; ++j){
            newImage.raw[i][j] = this->raw[i][j] || right.raw[i][j];
        }
    }
    return newImage;
}

BinaryImage BinaryImage::operator-(const BinaryImage &right)
{
    BinaryImage newImage(this->width(), this->height());
    int colMax = std::min(this->width(), right.width());
    int rowMax = std::min(this->height(), right.height());

    for(int i = 0; i < rowMax; ++i){
        for(int j = 0; j < colMax; ++j){
            newImage.raw[i][j] = this->raw[i][j] & !(right.raw[i][j]);
        }
    }
    return newImage;
}

bool BinaryImage::operator ==(const BinaryImage &right)
{
    if(width() != right.width() || height() != right.height()){
        return false;
    } else{
        for(int i = 0; i < width(); ++i){
            for(int j = 0; j < height(); ++j){
                if(raw[i][j] != right.raw[i][j])
                    return false;
            }
        }
        return true;
    }
}

BinaryImage BinaryImage::rotate90(bool clockwise)
{
    QVector<QVector<bool>> matrix = raw;

    int n = std::min(width(), height());

    for(int layer = 0; layer < n/2; layer ++){
        int first = layer;
        int last = n-1-layer;
        for(int i = first; i < last; i++)
        {
            int offset = i - first;
            int top = matrix[first][i];
            //left -> top
            matrix[first][i] = matrix[last-offset][first];
            //bottom -> left
            matrix[last-offset][first] = matrix[last][last -  offset];
            //right -> bottom
            matrix[last][last - offset] = matrix[i][last];
            //top -> right
            matrix[i][last] = top;
        }
    }

    BinaryImage newImage(matrix);

    return newImage;
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

bool BinaryImage::isBlank() const
{
    for(int row = 0; row < height(); row++){
        for(int col = 0; col < width(); col++){
            if(raw[row][col] == 1){
                return false;
            }
        }
    }

    return true;
}

QImage BinaryImage::toQImage() const
{
    QImage qImage(width(), height(), QImage::Format_RGBA8888);

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
