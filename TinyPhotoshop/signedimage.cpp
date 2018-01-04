#include "signedimage.h"
#include <cmath>

SignedImage::SignedImage(int width, int height, int init)
{
    QVector<QVector<int>> newRaw(height, QVector<int>(width, init));
    raw = newRaw;
}

SignedImage::SignedImage()
{
    QVector<QVector<int>> newRaw(0, QVector<int>(0, 0));
    raw = newRaw;
}

bool SignedImage::isValid() const
{
    return raw.size() != 0 && raw[0].size() != 0;
}

int SignedImage::width() const
{
    if(!isValid()){
        return 0;
    } else {
        return raw[0].size();
    }
}

int SignedImage::height() const
{
    if(!isValid()){
        return 0;
    } else {
        return raw.size();
    }
}

BinaryImage SignedImage::toBinaryImage() const
{
    BinaryImage bwImage(width(), height());

    for(int i = 0; i < height(); ++i){
        for(int j = 0; j < width(); ++j){
            if(raw[i][j] <= 0){
                bwImage.raw[i][j] = 0;
            } else {
                bwImage.raw[i][j] = 1;
            }
        }
    }

    return bwImage;
}

QImage SignedImage::toQImage() const
{
    QImage qImage(width(), height(), QImage::Format_RGBA8888);

    if(!isValid()) return qImage;

    int min = raw[0][0];
    int max = raw[0][0];

    for(int row = 0; row < height(); row++){
        for(int col = 0; col < width(); col++){
            int pixel = raw[row][col];

            if(pixel < min)  min = pixel;
            if(pixel > max)  max = pixel;
        }
    }

    if(min - max == 0) return qImage;

    int delta = max - min;
    float factor = 255.0 / delta;

    for(int row = 0; row < height(); row++){
        for(int col = 0; col < width(); col++){
            int pixel = raw[row][col];

            int normalized = roundf((pixel - min) * factor);
            if(normalized < 0) normalized = 0;
            if(normalized > 255) normalized = 255;

            QColor color(normalized, normalized, normalized);
            qImage.setPixelColor(col, row, color);
        }
    }

    return qImage;
}
