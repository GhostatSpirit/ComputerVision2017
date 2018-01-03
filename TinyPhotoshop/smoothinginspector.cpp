#include "smoothinginspector.h"
#include "ui_smoothinginspector.h"

#include "imagefilter.h"
#include <QDebug>

SmoothingInspector::SmoothingInspector(const QImage &original, QWidget *parent) :
    BaseInspector(original, parent),
    ui(new Ui::SmoothingInspector)
{
    ui->setupUi(this);

    connect(ui->meanButton, &QPushButton::clicked, this, &SmoothingInspector::ProcessMean);
    connect(ui->medianButton, &QPushButton::clicked, this, &SmoothingInspector::ProcessMedian);
    connect(ui->gaussianButton, &QPushButton::clicked, this, &SmoothingInspector::ProcessGaussian);
    connect(ui->manualButton, &QPushButton::clicked, this, &SmoothingInspector::ProcessManual);
}

SmoothingInspector::~SmoothingInspector()
{
    delete ui;
}

void SmoothingInspector::ResetImage(const QImage &newImage)
{
    BaseInspector::ResetImage(newImage);
}

float SmoothingInspector::GetLineEditFloat(QLineEdit *lineEdit)
{
    QString text = lineEdit->text();
    bool ok = false;
    float f = text.toFloat(&ok);
    if(!ok){
        f = 0;
        lineEdit->setText("0");
    }

    return f;
}

void SmoothingInspector::ProcessMedian()
{
    currentImage = ImageFilter::MedianFilterImage(originalImage, 1);
    emit ImageModified(currentImage);
}

void SmoothingInspector::ProcessMean()
{
    int size = 3;
    float value = 1.0 / float(size * size);

    auto kernel = ImageFilter::MakeKernel(size, value);
    currentImage = ImageFilter::FilterImage(originalImage, kernel);

    emit ImageModified(currentImage);
}

void SmoothingInspector::ProcessGaussian()
{
    float sigma = 1.0;
    int halfRange = 1; // 3 * 3

    currentImage = ImageFilter::GaussianFilterImage(originalImage, sigma, halfRange);

    emit ImageModified(currentImage);
}

void SmoothingInspector::ProcessManual()
{
    auto kernel = ImageFilter::MakeKernel(3);

    kernel[0][0] = GetLineEditFloat(ui->manual00LineEdit);
    kernel[0][1] = GetLineEditFloat(ui->manual01LineEdit);
    kernel[0][2] = GetLineEditFloat(ui->manual02LineEdit);
    kernel[1][0] = GetLineEditFloat(ui->manual10LineEdit);
    kernel[1][1] = GetLineEditFloat(ui->manual11LineEdit);
    kernel[1][2] = GetLineEditFloat(ui->manual12LineEdit);
    kernel[2][0] = GetLineEditFloat(ui->manual20LineEdit);
    kernel[2][1] = GetLineEditFloat(ui->manual21LineEdit);
    kernel[2][2] = GetLineEditFloat(ui->manual22LineEdit);


//    qDebug()<< kernel[0][0] << " " << kernel[0][1] << " " << kernel[0][2];
//    qDebug()<< kernel[1][0] << " " << kernel[1][1] << " " << kernel[1][2];
//    qDebug()<< kernel[2][0] << " " << kernel[2][1] << " " << kernel[2][2];

    currentImage = ImageFilter::FilterImage(originalImage, kernel);

    emit ImageModified(currentImage);
}

void SmoothingInspector::on_applyPushButton_clicked()
{
    ResetImage(currentImage);
}

void SmoothingInspector::on_revertPushButton_clicked()
{
    ResetImage(originalImage);
}
