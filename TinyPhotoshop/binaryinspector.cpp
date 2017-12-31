#include "binaryinspector.h"
#include "ui_binaryinspector.h"
#include "colorinspector.h"

BinaryInspector::BinaryInspector(const QImage& _image, QWidget *parent) :
    BaseInspector(_image, parent),
    ui(new Ui::BinaryInspector)
{
    ui->setupUi(this);

    connect(ui->otsuButton, &QPushButton::clicked, this, &BinaryInspector::ProcessOtsu);
}

BinaryInspector::~BinaryInspector()
{
    delete ui;
}

void BinaryInspector::ResetImage(const QImage &newImage)
{
    BaseInspector::ResetImage(newImage);
}

QImage BinaryInspector::ApplyOtsu(const QImage &original)
{
    int threshold = OtsuThreshold(original);

    QImage newImage(original.width(), original.height(), QImage::Format_Grayscale8);
    for(int row = 0; row < original.height(); row++){
        for(int col = 0; col < original.width(); col++){
            int gray = qGray(original.pixel(col, row));
            QColor newColor;

            if(gray > threshold){
                newColor = QColor(255, 255, 255);
            } else {
                newColor = QColor(0, 0, 0);
            }
            newImage.setPixelColor(col, row, newColor);
        }
    }

    return newImage;
}

int BinaryInspector::OtsuThreshold(const QImage &original)
{
    QVector<int> histogram(256, 0);
    for(int row = 0; row < original.height(); row++){
        for(int col = 0; col < original.width(); col++){
            int gray = qGray(original.pixel(col, row));
            histogram[gray]++;
        }
    }
    int pixelCount = original.height() * original.width();
    int threshold = 0;
    double maxVar = 0;
    double w0 = 0, w1 = 0, u0 = 0, u1 = 0;
    for(int i = 0; i < 256; ++i){
        w0 = w1 = u0 = u1 = 0;

        // calculate parameters for background
        for(int j = 0; j <= i; ++j){
            w1 += histogram[j];
            u1 += j * histogram[j];
        }
        if(w1 == 0) continue;

        u1 = u1 / w1;
        w1 = w1 / pixelCount;

        // calculate parameters for foreground
        for(int j = i + 1; j < 256; ++j){
            w0 += histogram[j];
            u0 += j * histogram[j];
        }
        if(w0 == 0) break;
        u0 = u0 / w0;
        w0 = w0 / pixelCount;

        double var = w0 * w1 * (u1 - u0) * (u1 - u0);
        if(var > maxVar){
            maxVar = var;
            threshold = i;
        }
    }

    return threshold;
}

void BinaryInspector::on_applyPushButton_clicked()
{
    ResetImage(currentImage);
}

void BinaryInspector::on_revertPushButton_clicked()
{
    ResetImage(originalImage);
}

void BinaryInspector::ProcessOtsu()
{
    currentImage = ApplyOtsu(originalImage);
    emit ImageModified(currentImage);
}
