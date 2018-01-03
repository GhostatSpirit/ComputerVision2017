#ifndef IMAGEFILTER_H
#define IMAGEFILTER_H

#include <QVector>
#include <QColor>
#include <QImage>

class ImageFilter
{
public:
    ImageFilter();

    static QImage FilterImage(const QImage& original, const QVector<QVector<float> >& kernel);

    static QImage MedianFilterImage(const QImage& original, int halfRange);

    static QImage GaussianFilterImage(const QImage& original, float sigma = 1.0, int halfRange = 1);

    static QVector<QVector<float> > MakeKernel(int size, float value = 0.0);
    static QVector<QVector<float> > MakeGaussianKernel(int size, float sigma = 1);

    static QImage CannyFilterImage(const QImage& original, int t1 = 0, int t2 = 255, float sigma = 1.0);

    // x -> [0, length), if x fall out of range, mirror it
    static inline int MirrorClamp(int x, int length);
private:

};

#endif // IMAGEFILTER_H
