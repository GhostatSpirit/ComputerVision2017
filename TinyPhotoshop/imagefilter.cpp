#include "imagefilter.h"
#include <cmath>

#include <QDebug>

ImageFilter::ImageFilter()
{

}

QImage ImageFilter::FilterImage(const QImage &original, const QVector<QVector<float> >& kernel)
{
    QImage image = original;

    int kHeight = kernel.size();
    int kWidth = kernel[0].size();

    Q_ASSERT(kHeight == kWidth);

    int halfRange = (kHeight - 1) / 2;
    int filterRange = kHeight;

    int height = image.height();
    int width = image.width();

    if(halfRange < 1 || halfRange > height / 2 || halfRange > width / 2){
        return image;
    }


    for(int y = 0; y < image.height(); y++){
        for(int x = 0; x < image.width(); x++){

            float redSum = 0, greenSum = 0, blueSum = 0;
            for(int i = 0; i < filterRange; ++i){
                for(int j = 0; j < filterRange; ++j){
                    int curX = MirrorClamp(x + i - halfRange, width);
                    int curY = MirrorClamp(y + j - halfRange, height);

                    QRgb pixel = original.pixel(curX, curY);

                    redSum += qRed(pixel) * kernel[j][i];
                    greenSum += qGreen(pixel) * kernel[j][i];
                    blueSum += qBlue(pixel) * kernel[j][i];
                }
            }

            int red = roundf(redSum);
            int green = roundf(greenSum);
            int blue = roundf(blueSum);

            red = abs(red);
            green = abs(green);
            blue = abs(blue);

            // clamp value to 0 - 255
            red = std::min(std::max((int)red, 0), 255);
            green = std::min(std::max((int)green, 0), 255);
            blue = std::min(std::max((int)blue, 0), 255);

            image.setPixel(x, y, qRgb(red, green, blue));
        }
    }
    return image;
}

QImage ImageFilter::MedianFilterImage(const QImage &original, int halfRange)
{
    QImage image = original.copy();

    int height = image.height();
    int width = image.width();

    int filterRange = 2 * halfRange + 1;
    int filterSize = filterRange * filterRange;

    if(halfRange < 1 || halfRange > height / 2 || halfRange > width / 2){
        return image;
    }

    std::vector<int> reds(filterSize, 0);
    std::vector<int> blues(filterSize, 0);
    std::vector<int> greens(filterSize, 0);

    for(int y = 0; y < image.height(); y++){
        for(int x = 0; x < image.width(); x++){

            for(int i = 0; i < filterRange; ++i){
                for(int j = 0; j < filterRange; ++j){
                    int curX = MirrorClamp(x + i - halfRange, width);
                    int curY = MirrorClamp(y + j - halfRange, height);

                    QRgb pixel = original.pixel(curX, curY);
                    reds[i * filterRange + j] = qRed(pixel);
                    greens[i * filterRange + j] = qGreen(pixel);
                    blues[i * filterRange + j] = qBlue(pixel);
                }
            }

            std::sort(reds.begin(), reds.end());
            std::sort(greens.begin(), greens.end());
            std::sort(blues.begin(), blues.end());

            int red = reds[halfRange];
            int green = greens[halfRange];
            int blue = blues[halfRange];

            image.setPixel(x, y, qRgb(red, green, blue));
        }
    }

    return image;
}

QImage ImageFilter::GaussianFilterImage(const QImage &original, float sigma, int halfRange)
{
    int height = original.height();
    int width = original.width();

    int filterRange = 2 * halfRange + 1;

    if(halfRange < 1 || halfRange > height / 2 || halfRange > width / 2){
        return original.copy();
    }

    auto kernel = MakeGaussianKernel(filterRange, sigma);

    return FilterImage(original, kernel);
}

QVector<QVector<float> > ImageFilter::MakeKernel(int size, float value)
{
    QVector<QVector<float> > kernel(size, QVector<float>(size, value));
    return kernel;
}

QVector<QVector<float> > ImageFilter::MakeGaussianKernel(int size, float sigma)
{
    int m = size, n = size;

    double cx = (double)(m - 1) / 2.0;
    double cy = (double)(n - 1) / 2.0;
    // Mat K(m, n, CV_64FC1);

    auto K = MakeKernel(size);
    double s2 = 2.0 * sigma * sigma;
    double x2, y2;

    for (int i = 0; i < m; i++)
    {
        x2 = pow(double(i - cx), 2);
        for (int j = 0; j < n; j++)
        {
            y2 = pow(double(j - cy), 2);
            K[i][j] = exp(-(x2 + y2) / s2);
        }
    }

    double sum = 0;
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            sum += K[i][j];
        }
    }

    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            K[i][j] /= sum;
        }
    }

    return K;
}

QImage ImageFilter::CannyFilterImage(const QImage &original, int t1, int t2, float sigma)
{
    int cols = original.width();
    int rows = original.height();

    QImage processed = GaussianFilterImage(original, sigma);

    std::vector<double>blank(cols, 0);
    std::vector<std::vector<double>>P(rows, blank);
    std::vector<std::vector<double>>Q(rows, blank);
    std::vector<std::vector<double>>amplitude(rows, blank);
    std::vector<std::vector<double>>theta(rows, blank);

    std::vector<std::vector<uchar>>src(rows, std::vector<uchar>(cols, 0));
    std::vector<std::vector<uchar>>output(rows, std::vector<uchar>(cols, 0));

    for(int y = 0; y < original.height(); y++){
        for(int x = 0; x < original.width(); x++){
            src[y][x] = qGray(processed.pixel(x, y));
        }
    }

    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            double curP = double(src[i - 1][j + 1]) + 2 * double(src[i][j + 1])
                + double(src[i + 1][j + 1]) - double(src[i - 1][j - 1])
                - 2 * double(src[i][j - 1]) - double(src[i + 1][j - 1]);
            double curQ = double(src[i - 1][j - 1]) + 2 * double(src[i - 1][j])
                + double(src[i - 1][j + 1]) - double(src[i + 1][j - 1])
                - 2 * double(src[i + 1][j]) - double(src[i + 1][j + 1]);
            P[i][j] = curP;
            Q[i][j] = curQ;
            amplitude[i][j] = sqrt((curP / 2)*(curP / 2) + (curQ / 2)*(curQ / 2));
            double temp = atan(curQ / curP)*57.3;
            if (temp < 0) {
                temp += 360;
            }
            theta[i][j] = temp;
        }
    }
    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            double curTheta = theta[i][j], curAmplitude = amplitude[i][j];
            double gtemp1, gtemp2;
            if (curAmplitude == 0) {
                continue;
            }
            if ((curTheta >= 0 && curTheta < 45) || (curTheta >= 180 && curTheta < 225)) {
                double g1 = amplitude[i - 1][j + 1], g2 = amplitude[i][j + 1],
                    g3 = amplitude[i + 1][j - 1], g4 = amplitude[i][j - 1];
                double weight = abs(Q[i][j]) / abs(P[i][j]);
                gtemp1 = weight*g1 + (1 - weight)*g2;
                gtemp2 = weight*g3 + (1 - weight)*g4;
            }
            else if ((curTheta >= 45 && curTheta < 90) || (curTheta >= 225 && curTheta < 270)) {
                double g1 = amplitude[i - 1][j], g2 = amplitude[i - 1][j + 1],
                    g3 = amplitude[i + 1][j], g4 = amplitude[i + 1][j - 1];
                double weight = abs(P[i][j]) / abs(Q[i][j]);
                gtemp1 = weight*g2 + (1 - weight)*g1;
                gtemp2 = weight*g3 + (1 - weight)*g4;
            }
            else if ((curTheta >= 90 && curTheta < 135) || (curTheta >= 270 && curTheta < 315)) {
                double g1 = amplitude[i - 1][j - 1], g2 = amplitude[i - 1][j],
                    g3 = amplitude[i + 1][j], g4 = amplitude[i + 1][j + 1];
                double weight = abs(P[i][j]) / abs(Q[i][j]);
                gtemp1 = weight*g1 + (1 - weight)*g2;
                gtemp2 = weight*g4 + (1 - weight)*g3;
            }
            else if ((curTheta >= 135 && curTheta < 180) || (curTheta >= 315 && curTheta < 360)) {
                double g1 = amplitude[i - 1][j - 1], g2 = amplitude[i][j - 1],
                    g3 = amplitude[i][j + 1], g4 = amplitude[i + 1][j + 1];
                double weight = abs(Q[i][j]) / abs(P[i][j]);
                gtemp1 = weight*g2 + (1 - weight)*g1;
                gtemp2 = weight*g4 + (1 - weight)*g3;
            }
            else {
                qDebug() << "Sorry!Canny edge detection wrong!" << endl;
                return original;
            }
            if (gtemp1 <= curAmplitude && gtemp2 <= curAmplitude) {
                output[i][j] = 128;
            }
        }
    }
    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            if (output[i][j] == 128 && amplitude[i][j] >= t1) {
                output[i][j] = 255;
                for (int m = i - 1; m <= i + 1; m++) {
                    for (int n = j - 1; n <= j + 1; n++) {
                        if (output[m][n] == 128 && amplitude[i][j] >= t2) {
                            output[m][n] = 255;
                        }
                    }
                }
            }
        }
    }
    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            if (output[i][j] == 128) {
                output[i][j] = 0;
            }
        }
    }

    QImage outputImage(original.width(), original.height(), original.format());


    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            int value = output[i][j];
            QColor color(value, value, value);
            outputImage.setPixelColor(j, i, color);
        }
    }


    return outputImage;
}

int ImageFilter::MirrorClamp(int x, int length)
{
    if(x < 0){
        x = -x;
    } else if(x >= length){
        x = 2 * (length - 1) - x;
    }
    return x;
}
