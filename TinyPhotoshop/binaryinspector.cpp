#include "binaryinspector.h"
#include "ui_binaryinspector.h"
#include "colorinspector.h"
#include "imagehistogram.h"

#include "binaryimage.h"

BinaryInspector::BinaryInspector(const QImage& _image, QWidget *parent) :
    BaseInspector(_image, parent),
    ui(new Ui::BinaryInspector)
{
    ui->setupUi(this);

    connect(ui->otsuButton, &QPushButton::clicked, this, &BinaryInspector::ProcessOtsu);
    connect(ui->manualButton, &QPushButton::clicked, this, &BinaryInspector::ProcessManualClicked);

    connect(ui->minValueSlider, &QSlider::valueChanged, this, &BinaryInspector::ProcessManualValueChange);
    connect(ui->maxValueSlider, &QSlider::valueChanged, this, &BinaryInspector::ProcessManualValueChange);
    connect(ui->minValueLineEdit, &QLineEdit::editingFinished, this, &BinaryInspector::ProcessManualString);
    connect(ui->maxValueLineEdit, &QLineEdit::editingFinished, this, &BinaryInspector::ProcessManualString);

}

BinaryInspector::~BinaryInspector()
{
    delete ui;
}

void BinaryInspector::ResetImage(const QImage &newImage)
{
    ui->minValueSlider->setValue(128);
    ui->maxValueSlider->setValue(128);

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

QImage BinaryInspector::ApplyManual(const QImage &original, int minVal, int maxVal)
{
    if(minVal < 0 || minVal > 255) return original;
    if(maxVal < 0 || maxVal > 255) return original;
    if(minVal > maxVal) return original;

    QImage newImage(original.width(), original.height(), QImage::Format_Grayscale8);
    for(int row = 0; row < original.height(); row++){
        for(int col = 0; col < original.width(); col++){
            int gray = qGray(original.pixel(col, row));
            QColor newColor;

            if(gray >= minVal && gray <= maxVal){
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
//    QVector<int> histogram(256, 0);
//    for(int row = 0; row < original.height(); row++){
//        for(int col = 0; col < original.width(); col++){
//            int gray = qGray(original.pixel(col, row));
//            histogram[gray]++;
//        }
//    }
    QVector<int> histogram = ImageHistogram::GetHistogram(original, ImageHistogram::GRAY);

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

void BinaryInspector::ProcessManualClicked()
{
    int minValue = ui->minValueSlider->value();
    int maxValue = ui->maxValueSlider->value();
    currentImage = ApplyManual(originalImage, minValue, maxValue);
    emit ImageModified(currentImage);
}

void BinaryInspector::ProcessManualValueChange()
{
    int minValue = ui->minValueSlider->value();
    int maxValue = ui->maxValueSlider->value();

    ui->minValueLineEdit->setText(QString::number(minValue));
    ui->maxValueLineEdit->setText(QString::number(maxValue));

    if(minValue > maxValue) ui->manualButton->setEnabled(false);
    else ui->manualButton->setEnabled(true);

    if(minValue <= maxValue){
        ProcessManualClicked();
    }
}

void BinaryInspector::ProcessManualString()
{
    QLineEdit* from = dynamic_cast<QLineEdit*>(QObject::sender());
    if(from == nullptr) return;

    QString newStr = from->text();

    bool ok = false;
    int newVal = newStr.toInt(&ok);
    if(ok){
        if(newVal > 255) newVal = 255;
        if(newVal < 0) newVal = 0;

        if(from == ui->minValueLineEdit){
            ui->minValueSlider->setValue(newVal);
        } else if(from == ui->maxValueLineEdit){
            ui->maxValueSlider->setValue(newVal);
        }
    } else {
        int minValue = ui->minValueSlider->value();
        ui->minValueLineEdit->setText(QString::number(minValue));
        int maxValue = ui->maxValueSlider->value();
        ui->maxValueLineEdit->setText(QString::number(maxValue));
    }
}

