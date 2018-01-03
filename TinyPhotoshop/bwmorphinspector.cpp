#include "bwmorphinspector.h"
#include "ui_bwmorphinspector.h"

#include "binaryimage.h"
#include "bwmorph.h"

BWMorphInspector::BWMorphInspector(const QImage& original, QWidget *parent) :
    BaseInspector(original, parent),
    ui(new Ui::BWMorphInspector)
{
    ui->setupUi(this);

    connect(ui->dilateButton, &QPushButton::clicked, this, &BWMorphInspector::ProcessDilate);
    connect(ui->erodeButton, &QPushButton::clicked, this, &BWMorphInspector::ProcessErode);
}

BWMorphInspector::~BWMorphInspector()
{
    delete ui;
}

void BWMorphInspector::ResetImage(const QImage &newImage)
{
    BaseInspector::ResetImage(newImage);
}

void BWMorphInspector::on_applyPushButton_clicked()
{
    ResetImage(currentImage);
}

void BWMorphInspector::on_revertPushButton_clicked()
{
    ResetImage(originalImage);
}

void BWMorphInspector::ProcessDilate()
{
    if(currentImage.isNull()){
        return;
    }

    BinaryImage bwImage(currentImage);

    BinaryImage element(3);
    element.raw[0][1] = 1;
    element.raw[1][0] = element.raw[1][1] = element.raw[1][2] = 1;
    element.raw[2][1] = 1;

    BinaryImage final = BWMorph::dilate(bwImage, element);

    currentImage = final.toQImage();

    emit ImageModified(currentImage);
}

void BWMorphInspector::ProcessErode()
{
    if(currentImage.isNull()){
        return;
    }

    BinaryImage bwImage(currentImage);

    BinaryImage element(3);
    element.raw[0][1] = 1;
    element.raw[1][0] = element.raw[1][1] = element.raw[1][2] = 1;
    element.raw[2][1] = 1;

    BinaryImage final = BWMorph::erode(bwImage, element);

    currentImage = final.toQImage();

    emit ImageModified(currentImage);
}
