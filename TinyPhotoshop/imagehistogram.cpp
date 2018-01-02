#include "imagehistogram.h"

ImageHistogram::ImageHistogram()
{

}

QVector<int> ImageHistogram::GetHistogram(const QImage &image, ImageHistogram::HistogramType type)
{
    if(image.isNull()){
        return QVector<int>(0);
    }

    QVector<int> histogram(256, 0);
    for(int row = 0; row < image.height(); row++){
        for(int col = 0; col < image.width(); col++){
            int value = 0;
            switch (type) {
            case HistogramType::RED:
                value = qRed(image.pixel(col, row));
                break;
            case HistogramType::GREEN:
                value = qGreen(image.pixel(col, row));
                break;
            case HistogramType::BLUE:
                value = qBlue(image.pixel(col, row));
                break;
            case HistogramType::GRAY:
                value = qGray(image.pixel(col, row));
                break;
            }
            histogram[value]++;
        }
    }
    return histogram;
}
