#include "contrastinspector.h"
#include "ui_contrastinspector.h"

#include <cmath>
#include <QDebug>

ContrastInspector::ContrastInspector(const QImage &_original, QWidget *parent) :
    BaseInspector(_original, parent),
    ui(new Ui::ContrastInspector)
{
    ui->setupUi(this);

    connect(ui->linearButton, &QPushButton::clicked, this,&ContrastInspector::ProcessButtonClicked);
    connect(ui->logButton, &QPushButton::clicked, this,&ContrastInspector::ProcessButtonClicked);
    connect(ui->powerButton, &QPushButton::clicked, this,&ContrastInspector::ProcessButtonClicked);
}

ContrastInspector::~ContrastInspector()
{
    delete ui;
}

void ContrastInspector::ResetImage(const QImage &newImage)
{
    BaseInspector::ResetImage(newImage);
}

QImage ContrastInspector::LinearContrast(const QImage &image, float k)
{
    qDebug() << "in linear";

    QImage newImage(image);

    for(int row = 0; row < newImage.height(); row++){
        for(int col = 0; col < newImage.width(); col++){
            QColor curColor(newImage.pixel(col, row));
            short red = curColor.red();
            short blue = curColor.blue();
            short green = curColor.green();

            red = roundf(k * red);
            green = roundf(k * green);
            blue = roundf(k * blue);

            if(k < 0){
                red += 255;
                green += 255;
                blue += 255;
            }
            // clamp value to 0 - 255
            red = std::min(std::max((int)red, 0), 255);
            green = std::min(std::max((int)green, 0), 255);
            blue = std::min(std::max((int)blue, 0), 255);

            curColor.setRed(red);
            curColor.setGreen(green);
            curColor.setBlue(blue);
            newImage.setPixelColor(col, row, curColor);
        }
    }

    return newImage;
}

QImage ContrastInspector::PowerContrast(const QImage &image, float c, float gamma)
{
    QImage newImage(image);

    for(int row = 0; row < newImage.height(); row++){
        for(int col = 0; col < newImage.width(); col++){
            QColor curColor(newImage.pixel(col, row));
            short red = curColor.red();
            short blue = curColor.blue();
            short green = curColor.green();

            red = roundf(c * pow(red, gamma));
            green = roundf(c * pow(green, gamma));
            blue = roundf(c * pow(blue, gamma));


            // clamp value to 0 - 255
            red = std::min(std::max((int)red, 0), 255);
            green = std::min(std::max((int)green, 0), 255);
            blue = std::min(std::max((int)blue, 0), 255);

            curColor.setRed(red);
            curColor.setGreen(green);
            curColor.setBlue(blue);
            newImage.setPixelColor(col, row, curColor);
        }
    }

    return newImage;
}

QImage ContrastInspector::LogContrast(const QImage &image, float c)
{
    QImage newImage(image);

    for(int row = 0; row < newImage.height(); row++){
        for(int col = 0; col < newImage.width(); col++){
            QColor curColor(newImage.pixel(col, row));
            short red = curColor.red();
            short blue = curColor.blue();
            short green = curColor.green();

            red = roundf(c * log(red + 1));
            green = roundf(c * log(green + 1));
            blue = roundf(c * log(blue + 1));

            // clamp value to 0 - 255
            red = std::min(std::max((int)red, 0), 255);
            green = std::min(std::max((int)green, 0), 255);
            blue = std::min(std::max((int)blue, 0), 255);

            curColor.setRed(red);
            curColor.setGreen(green);
            curColor.setBlue(blue);
            newImage.setPixelColor(col, row, curColor);
        }
    }

    return newImage;
}

void ContrastInspector::on_applyPushButton_clicked()
{
    ResetImage(currentImage);
}

void ContrastInspector::on_revertPushButton_clicked()
{
    ResetImage(originalImage);
}

void ContrastInspector::ProcessButtonClicked()
{
    QPushButton* from = dynamic_cast<QPushButton*>(QObject::sender());
    if(from == nullptr) return;

    if(from == ui->linearButton){
        QString text = ui->linearLineEdit->text();
        bool ok = false;
        float k = text.toFloat(&ok);
        if(!ok){
            k = 1.0;
            ui->linearLineEdit->setText(QString::number(k));
        }
        currentImage = LinearContrast(originalImage, k);

        emit ImageModified(currentImage);

    } else if(from == ui->logButton){
        QString text = ui->logLineEdit->text();
        bool ok = false;
        float c = text.toFloat(&ok);
        if(!ok){
            c = 1.0;
            ui->logLineEdit->setText(QString::number(c));
        }
        currentImage = LogContrast(originalImage, c);

        emit ImageModified(currentImage);

    } else if(from == ui->powerButton){
        QString text = ui->powerCLineEdit->text();
        bool ok = false;

        float c = text.toFloat(&ok);
        if(!ok){
            c = 1.0;
            ui->powerCLineEdit->setText(QString::number(c));
        }

        text = ui->powerGammaLineEdit->text();
        float gamma = text.toFloat(&ok);
        if(!ok){
            gamma = 1.0;
            ui->powerGammaLineEdit->setText(QString::number(gamma));
        }

        currentImage = PowerContrast(originalImage, c, gamma);

        emit ImageModified(currentImage);
    }
}
