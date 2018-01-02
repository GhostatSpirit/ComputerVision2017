#ifndef IMAGEHISTOGRAM_H
#define IMAGEHISTOGRAM_H
#include <QVector>
#include <QImage>


class ImageHistogram
{
public:
    ImageHistogram();

    enum HistogramType{
        RED,
        GREEN,
        BLUE,
        GRAY
    };

    static QVector<int> GetHistogram(const QImage& image, HistogramType type);
};

#endif // IMAGEHISTOGRAM_H
