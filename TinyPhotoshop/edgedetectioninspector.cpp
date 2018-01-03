#include "edgedetectioninspector.h"
#include "ui_edgedetectioninspector.h"

#include "imagefilter.h"
#include "arithmeticinspector.h"

EdgeDetectionInspector::EdgeDetectionInspector(const QImage& original, QWidget *parent) :
    BaseInspector(original, parent),
    ui(new Ui::EdgeDetectionInspector)
{
    ui->setupUi(this);

    connect(ui->sobelButton, &QPushButton::clicked, this, &EdgeDetectionInspector::ProcessSobel);
    connect(ui->laplaceanButton, &QPushButton::clicked, this, &EdgeDetectionInspector::ProcessLaplacean);
    connect(ui->cannyButton, &QPushButton::clicked, this, &EdgeDetectionInspector::ProcessCanny);
}

EdgeDetectionInspector::~EdgeDetectionInspector()
{
    delete ui;
}

void EdgeDetectionInspector::ResetImage(const QImage &newImage)
{
    BaseInspector::ResetImage(newImage);
}

void EdgeDetectionInspector::on_applyPushButton_clicked()
{
    ResetImage(currentImage);
}

void EdgeDetectionInspector::on_revertPushButton_clicked()
{
    ResetImage(originalImage);
}

void EdgeDetectionInspector::ProcessSobel()
{
    bool gx = ui->sobelXCheckBox->isChecked();
    bool gy = ui->sobelYCheckBox->isChecked();

    if(!gx && !gy)  return;

    auto gxkernel = ImageFilter::MakeKernel(3);
    gxkernel[0] = {1, 0, -1};
    gxkernel[1] = {2, 0, -2};
    gxkernel[2] = {1, 0, -1};

    auto gykernel = ImageFilter::MakeKernel(3);
    gykernel[0] = {1, 2, 1};
    gykernel[1] = {0, 0, 0};
    gykernel[2] = {-1, -2, -1};

    if(gx && !gy){
        currentImage = ImageFilter::FilterImage(originalImage, gxkernel);
    } else if(!gx && gy){
        currentImage = ImageFilter::FilterImage(originalImage, gykernel);
    } else {
        QImage gxImage = ImageFilter::FilterImage(originalImage, gxkernel);
        QImage gyImage = ImageFilter::FilterImage(originalImage, gykernel);
        currentImage = ArithmeticInspector::ImageAdd(gxImage, gyImage);
    }

    emit ImageModified(currentImage);
}

void EdgeDetectionInspector::ProcessLaplacean()
{
    auto kernel = ImageFilter::MakeKernel(3);
    kernel[0] = {-1, -1, -1};
    kernel[1] = {-1, +8, -1};
    kernel[2] = {-1, -1, -1};

    currentImage = ImageFilter::FilterImage(originalImage, kernel);
    emit ImageModified(currentImage);

}

void EdgeDetectionInspector::ProcessCanny()
{
   int t1 = GetLineEditFloat(ui->cannyT1LineEdit);
   int t2 = GetLineEditFloat(ui->cannyT2LineEdit);

   float sigma = GetLineEditFloat(ui->cannySigmaLineEdit);

    currentImage = ImageFilter::CannyFilterImage(originalImage, t1, t2, sigma);
    emit ImageModified(currentImage);
}

float EdgeDetectionInspector::GetLineEditFloat(QLineEdit *lineEdit)
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
