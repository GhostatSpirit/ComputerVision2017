#include "grayscaleimage.h"

GrayscaleImage::GrayscaleImage()
{
    QVector<QVector<uchar>> newRaw(0, QVector<uchar>(0));
    raw = newRaw;
}

GrayscaleImage::GrayscaleImage(int width, int height, uchar init)
{
    QVector<QVector<uchar>> newRaw(height, QVector<uchar>(width, init));
    raw = newRaw;
}

GrayscaleImage::GrayscaleImage(const QImage &qImage)
{
    if(qImage.isNull()){
        QVector<QVector<uchar>> newRaw(0, QVector<uchar>(0));
        raw = newRaw;
        return;
    }

    int width = qImage.width();
    int height = qImage.height();

    QVector<QVector<uchar>> newRaw(height, QVector<uchar>(width, 0));

    for(int row = 0; row < height; row++){
        for(int col = 0; col < width; col++){
            QRgb curColor = qImage.pixel(col, row);
            uchar value = qGray(curColor);
            newRaw[row][col] = value;
        }
    }

    raw = newRaw;
}

bool GrayscaleImage::operator ==(const GrayscaleImage &right)
{
    if(width() != right.width() || height() != right.height()){
        return false;
    } else{
        for(int i = 0; i < height(); ++i){
            for(int j = 0; j < width(); ++j){
                if(raw[i][j] != right.raw[i][j])
                    return false;
            }
        }
        return true;
    }
}

bool GrayscaleImage::operator !=(const GrayscaleImage &right)
{
    if(width() != right.width() || height() != right.height()){
        return true;
    } else{
        for(int i = 0; i < height(); ++i){
            for(int j = 0; j < width(); ++j){
                if(raw[i][j] != right.raw[i][j])
                    return true;
            }
        }
        return false;
    }
}

bool GrayscaleImage::isValid() const
{
    return raw.size() != 0 && raw[0].size() != 0;
}

int GrayscaleImage::width() const
{
    if(!isValid()){
        return 0;
    } else {
        return raw[0].size();
    }
}

int GrayscaleImage::height() const
{
    if(!isValid()){
        return 0;
    } else {
        return raw.size();
    }
}

bool GrayscaleImage::isBlank() const
{
    for(int row = 0; row < height(); row++){
        for(int col = 0; col < width(); col++){
            if(raw[row][col] > 0){
                return false;
            }
        }
    }

    return true;
}

GrayscaleImage GrayscaleImage::clampWithMask(const GrayscaleImage &mask)
{
    GrayscaleImage final = *this;

    int minheight = std::min(height(), mask.height());
    int minwidth = std::min(width(), mask.width());

    for(int i = 0; i < minheight; ++i){
        for(int j = 0; j < minwidth; ++j){
            auto curpixel = final.raw[i][j];
            auto maskPixel = mask.raw[i][j];

            if(curpixel > maskPixel){
                final.raw[i][j] = maskPixel;
            }
        }
    }

    return final;
}


QImage GrayscaleImage::toQImage() const
{
    QImage qImage(width(), height(), QImage::Format_RGBA8888);

    for(int row = 0; row < height(); row++){
        for(int col = 0; col < width(); col++){
            uchar value = raw[row][col];
            QColor color(value, value, value);
            qImage.setPixelColor(col, row, color);
        }
    }

    return qImage;
}


