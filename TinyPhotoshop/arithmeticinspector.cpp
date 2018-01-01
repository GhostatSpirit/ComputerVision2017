#include "arithmeticinspector.h"
#include "ui_arithmeticinspector.h"

#include <QFileDialog>
#include <QDebug>

ArithmeticInspector::ArithmeticInspector(const QImage& _image, QWidget *parent) :
    BaseInspector(_image, parent),
    ui(new Ui::ArithmeticInspector)
{
    ui->setupUi(this);

    connect(ui->loadImageButton, &QPushButton::clicked, this, &ArithmeticInspector::LoadOperationImage);
    connect(ui->addButton, &QPushButton::clicked, this, &ArithmeticInspector::ProcessOperationButton);
    connect(ui->minusButton, &QPushButton::clicked, this, &ArithmeticInspector::ProcessOperationButton);
    connect(ui->multiplyButton, &QPushButton::clicked, this, &ArithmeticInspector::ProcessOperationButton);
}

ArithmeticInspector::~ArithmeticInspector()
{
    delete ui;
}

void ArithmeticInspector::ResetImage(const QImage &newImage)
{
    BaseInspector::ResetImage(newImage);
    CreateResizedImage();
}

// the output image will be the same size of left
QImage ArithmeticInspector::ImageAdd(const QImage &left, const QImage &right)
{
    if(left.isNull()) return right;
    if(right.isNull()) return left;

    QImage result = left.copy();
    int dx = left.width() - right.width();
    int dy = left.height() - right.height();

    for(int row = 0; row < right.height(); row++){
        for(int col = 0; col < right.width(); col++){
            QColor rColor(right.pixel(col, row));

            int lrow = row + dy / 2;
            int lcol = col + dx / 2;
            QColor lColor(left.pixel(lcol, lrow));

            qreal r = lColor.redF() + rColor.redF();
            if(r > 1.0) r = 1.0;
            qreal g = lColor.greenF() + rColor.greenF();
            if(g > 1.0) g = 1.0;
            qreal b = lColor.blueF() + rColor.blueF();
            if(b > 1.0) b = 1.0;

            lColor.setRedF(r);
            lColor.setGreenF(g);
            lColor.setBlueF(b);

            result.setPixelColor(lcol, lrow, lColor);
        }
    }

    return result;
}

QImage ArithmeticInspector::ImageMinus(const QImage &left, const QImage &right)
{
    if(left.isNull()) return right;
    if(right.isNull()) return left;

    QImage result = left.copy();
    int dx = left.width() - right.width();
    int dy = left.height() - right.height();

    for(int row = 0; row < right.height(); row++){
        for(int col = 0; col < right.width(); col++){
            QColor rColor(right.pixel(col, row));

            int lrow = row + dy / 2;
            int lcol = col + dx / 2;
            QColor lColor(left.pixel(lcol, lrow));

            qreal r = lColor.redF() - rColor.redF();
            if(r < 0.0) r = 0.0;
            qreal g = lColor.greenF() - rColor.greenF();
            if(g < 0.0) g = 0.0;
            qreal b = lColor.blueF() - rColor.blueF();
            if(b < 0.0) b = 0.0;

            lColor.setRedF(r);
            lColor.setGreenF(g);
            lColor.setBlueF(b);

            result.setPixelColor(lcol, lrow, lColor);
        }
    }

    return result;
}

QImage ArithmeticInspector::ImageMultiply(const QImage &left, const QImage &right)
{
    if(left.isNull()) return right;
    if(right.isNull()) return left;

    QImage result = left.copy();
    int dx = left.width() - right.width();
    int dy = left.height() - right.height();

    for(int row = 0; row < right.height(); row++){
        for(int col = 0; col < right.width(); col++){
            QColor rColor(right.pixel(col, row));

            int lrow = row + dy / 2;
            int lcol = col + dx / 2;
            QColor lColor(left.pixel(lcol, lrow));

            qreal r = lColor.redF() * rColor.redF();
            qreal g = lColor.greenF() * rColor.greenF();
            qreal b = lColor.blueF() * rColor.blueF();

            lColor.setRedF(r);
            lColor.setGreenF(g);
            lColor.setBlueF(b);

            result.setPixelColor(lcol, lrow, lColor);
        }
    }

    return result;
}

bool ArithmeticInspector::CreateResizedImage()
{
    if(!originalImage.isNull() && !operationImage.isNull()){
        int width = originalImage.width();
        int height = originalImage.height();
        resizedImage = operationImage.scaled(width, height, Qt::KeepAspectRatio);

//        qDebug() << "resized width: " << resizedImage.width();
//        qDebug() << "resized height: " << resizedImage.height();

        return true;
    }else{
        return false;
    }
}


void ArithmeticInspector::on_applyPushButton_clicked()
{
    ResetImage(currentImage);
}

void ArithmeticInspector::on_revertPushButton_clicked()
{
    ResetImage(originalImage);
}

void ArithmeticInspector::LoadOperationImage()
{

    QString fileName = QFileDialog::getOpenFileName(
                this, "open image file",
                ".",
                "Image files (*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
    if(fileName != "")
    {
        if(operationImage.load(fileName))
        {
            QGraphicsScene *scene = new QGraphicsScene;
            scene->addPixmap(QPixmap::fromImage(operationImage));
            ui->operationImageGraphicsView->setScene(scene);
            // ui->imageGraphicsView->resize(image->width() + 10, image->height() + 10);
            ui->operationImageGraphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
            ui->operationImageGraphicsView->show();

            ui->addButton->setEnabled(true);
            ui->minusButton->setEnabled(true);
            ui->multiplyButton->setEnabled(true);

            CreateResizedImage();
        }
    }
}

void ArithmeticInspector::ProcessOperationButton()
{
    if(originalImage.isNull() || resizedImage.isNull()) return;

    QPushButton* from = dynamic_cast<QPushButton*>(QObject::sender());
    if(from == nullptr) return;

    if(from == ui->addButton){
        currentImage = ImageAdd(originalImage, resizedImage);
    } else if(from == ui->minusButton){
        currentImage = ImageMinus(originalImage, resizedImage);
    } else if(from == ui->multiplyButton){
        currentImage = ImageMultiply(originalImage, resizedImage);
    }

    emit ImageModified(currentImage);
}

