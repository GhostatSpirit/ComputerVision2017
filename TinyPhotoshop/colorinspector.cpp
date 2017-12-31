#include "colorinspector.h"
#include "ui_colorinspector.h"
#include <algorithm>
#include <math.h>

#include <QDebug>

static int round_float(float number)
{
    return (number > 0.0) ? (number + 0.5) : (number - 0.5);
}


ColorInspector::ColorInspector(const QImage& _originalImage, QWidget *parent) :
    BaseInspector(_originalImage, parent),
    ui(new Ui::ColorInspector),
    hsvInterval(0.25)
{
    ui->setupUi(this);

    connect(ui->redCheckBox, &QCheckBox::toggled, this, &ColorInspector::ProcessColorCheckbox);
    connect(ui->greenCheckBox, &QCheckBox::toggled, this, &ColorInspector::ProcessColorCheckbox);
    connect(ui->blueCheckBox, &QCheckBox::toggled, this, &ColorInspector::ProcessColorCheckbox);

    connect(ui->grayscaleButton, &QPushButton::clicked, this, &ColorInspector::ProcessGrayscale);

    connect(ui->hueSlider, &QSlider::valueChanged, this, &ColorInspector::ProcessHSV);
    connect(ui->saturationSlider, &QSlider::valueChanged, this, &ColorInspector::ProcessHSV);
    connect(ui->valueSlider, &QSlider::valueChanged, this, &ColorInspector::ProcessHSV);

    // TODO: use ImageModified() signal to disable check boxes & grayscale button
    // when the image is already in graysacle

}

ColorInspector::~ColorInspector()
{
    delete ui;
}

void ColorInspector::ResetImage(const QImage &newImage)
{
    BaseInspector::ResetImage(newImage);

    ui->redCheckBox->setChecked(true);
    ui->greenCheckBox->setChecked(true);
    ui->blueCheckBox->setChecked(true);

    int min, max, mean;
    min = ui->hueSlider->minimum();
    max = ui->hueSlider->maximum();
    mean = (min + max) / 2;
    ui->hueSlider->setValue(mean);

    min = ui->saturationSlider->minimum();
    max = ui->saturationSlider->maximum();
    mean = (min + max) / 2;
    ui->saturationSlider->setValue(mean);

    min = ui->valueSlider->minimum();
    max = ui->valueSlider->maximum();
    mean = (min + max) / 2;
    ui->valueSlider->setValue(mean);
}



QImage ColorInspector::SplitChannel(const QImage &original, bool r, bool g, bool b)
{
    QImage newImage = original.copy();

    for(int row = 0; row < original.height(); row++){
        for(int col = 0; col < original.width(); col++){
            QColor curColor(original.pixel(col, row));
            if(!r) curColor.setRed(0);
            if(!g) curColor.setGreen(0);
            if(!b) curColor.setBlue(0);
            newImage.setPixelColor(col, row, curColor);
        }
    }
    return newImage;
}


// if >=2 channels are in use, gray = (r * 11 + g * 16 + b * 5)/32.
// else, use the active channel to perform conversion
QImage ColorInspector::ConvertToGrayscale(const QImage &original, bool r, bool g, bool b)
{
    int channel = GetActiveChannel(r, g, b);
    QImage newImage = QImage(original.width(), original.height(), QImage::Format_Grayscale8);

    for(int row = 0; row < original.height(); row++){
        for(int col = 0; col < original.width(); col++){
            QColor curColor(original.pixel(col, row));
            int gray = 0;

            if(channel == 0){
                gray = curColor.red() * 11 + curColor.green() * 16 + curColor.red() * 5;
                gray = gray / 32;
            } else {
                if(r)  gray = curColor.red();
                if(g)  gray = curColor.green();
                if(b)  gray = curColor.blue();
            }

            QColor newColor(gray, gray, gray);
            newImage.setPixelColor(col, row, newColor);
        }
    }

    return newImage;
}

int ColorInspector::GetActiveChannel(bool r, bool g, bool b)
{
    int channel = 0;
    int active = 0;
    if(r){
        active++;
        channel = 1;
    }
    if(g){
        active++;
        channel = 2;
    }
    if(b){
        active++;
        channel = 3;
    }
    if(active > 1){
        return 0;
    } else {
        return channel;
    }
}

// H: 0 - 360
// S: 0 - 255
// V: 0 - 255
void ColorInspector::QColortoHSV(const QColor &color, int *ph, int *ps, int *pv)
{
    float r = color.redF();
    float g = color.greenF();
    float b = color.blueF();

    float h = 0.0, s = 0.0, v = 0.0;
    v = std::max(std::max(r, g), b);
    float min = std::min(std::min(r, g), b);
    if(v > 0.0){
        s = (v - min) / v;
    } else {
        s = 0.0;
    }
    if(s <= 0.0){
        *ph = -1;
        *ps = round_float(s * 255.0);
        *pv = round_float(v * 255.0);
        return;
    }
    float diff = v - min;

    if(r == v){
        h = 60.0 * (g - b) / diff;
    } else if(g == v){
        h = 120.0 + 60.0 * (b - r)/diff;
    } else if(b == v){
        h = 240.0 + 60.0 * (r - g)/diff;
    }

    if(h < 0.0){
        h = h + 360.0;
    }

    *ph = round_float(h);
    *ps = round_float(s * 255.0);
    if(*ps > 255) *ps = 255;
    *pv = round_float(v * 255.0);
    if(*pv > 255) *pv = 255;

    return;
}

QColor ColorInspector::HSVtoQColor(int ih, int is, int iv)
{
    // H: 0 - 360
    // S: 0 - 255
    // V: 0 - 255
    float s = is / 255.0;
    float v = iv / 255.0;

    int hi = (ih / 60) % 6;
    float f = ih / 60.0 - hi;

    float p = v * (1.0 - s);
    float q = v * (1.0 - f * s);
    float t = v * (1.0f - (1.0 - f) * s);

    int ip = p * 255.0;
    int iq = q * 255.0;
    int it = t * 255.0;

    QColor newColor;
    switch(hi){
    case 0:
        newColor.setRgb(iv, it, ip);
        break;
    case 1:
        newColor.setRgb(iq, iv, ip);
        break;
    case 2:
        newColor.setRgb(ip, iv, it);
        break;
    case 3:
        newColor.setRgb(ip, iq, iv);
        break;
    case 4:
        newColor.setRgb(it, ip, iv);
        break;
    case 5:
        newColor.setRgb(iv, ip, iq);
        break;
    default:
        Q_ASSERT(0);
    }

    return newColor;

}

QImage ColorInspector::TuneHSV(const QImage &original, double hfactor, double sfactor, double vfactor)
{
    QImage newImage = original.copy();

    for(int row = 0; row < original.height(); row++){
        for(int col = 0; col < original.width(); col++){
            QColor curColor(original.pixel(col, row));

            int h = 0, s = 0, v = 0;
            QColortoHSV(curColor, &h, &s, &v);
            // curColor.getHsv(&h, &s, &v);
//            if(h == -1){
//                continue;
//            }

            if(h != -1){
                h = h + 180 * hfactor;
                if(h >= 360) h = h - 360;
                if(h < 0) h = h + 360;
            }

            s = s + s * sfactor;
            if(s > 255) s = 255;
            if(s < 0) s = 0;

            v = v + v * vfactor;
            if(v > 255) v = 255;
            if(v < 0) v = 0;


            // QColor newColor = HSVtoQColor(h, s, v);
            QColor newColor(curColor);
            newColor = HSVtoQColor(h, s, v);
            newImage.setPixelColor(col, row, newColor);
        }
    }

    return newImage;
}


void ColorInspector::ProcessColorCheckbox()
{
    bool red = ui->redCheckBox->isChecked();
    bool green = ui->greenCheckBox->isChecked();
    bool blue = ui->blueCheckBox->isChecked();

    currentImage = SplitChannel(originalImage, red, green, blue);

    emit ImageModified(currentImage);
}

void ColorInspector::ProcessGrayscale()
{
    bool red = ui->redCheckBox->isChecked();
    bool green = ui->greenCheckBox->isChecked();
    bool blue = ui->blueCheckBox->isChecked();

    currentImage = ConvertToGrayscale(originalImage, red, green, blue);

    emit ImageModified(currentImage);
}

void ColorInspector::ProcessHSV()
{
    float min, max, cur;
    float hfactor, sfactor, vfactor;

    min = ui->hueSlider->minimum();
    max = ui->hueSlider->maximum();
    cur = ui->hueSlider->value();
    hfactor = (((cur - min) / (max - min)) - 0.5) * 2;

    min = ui->saturationSlider->minimum();
    max = ui->saturationSlider->maximum();
    cur = ui->saturationSlider->value();
    sfactor = (((cur - min) / (max - min)) - 0.5) * 2;

    min = ui->valueSlider->minimum();
    max = ui->valueSlider->maximum();
    cur = ui->valueSlider->value();
    vfactor = (((cur - min) / (max - min)) - 0.5) * 2;

//    qDebug() << hfactor << " " << sfactor << " " << vfactor;

    if(!timer.isValid()){
        timer.start();
        currentImage = TuneHSV(originalImage, hfactor, sfactor, vfactor);
        emit ImageModified(currentImage);
    }
    if(timer.elapsed() >= hsvInterval * 1000.0){
        timer.restart();
        currentImage = TuneHSV(originalImage, hfactor, sfactor, vfactor);
        emit ImageModified(currentImage);
    }
}

void ColorInspector::on_applyPushButton_clicked()
{
    ResetImage(currentImage);
}

void ColorInspector::on_revertPushButton_clicked()
{
    ResetImage(originalImage);
}
