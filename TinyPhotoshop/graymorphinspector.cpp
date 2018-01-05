#include "graymorphinspector.h"
#include "ui_graymorphinspector.h"

#include "uiutility.h"
#include "bwmorph.h"
#include "graymorph.h"
#include "watershed.h"
#include <QFileDialog>

GrayMorphInspector::GrayMorphInspector(const QImage &original, QWidget *parent) :
    BaseInspector(original, parent),
    ui(new Ui::GrayMorphInspector)
{
    ui->setupUi(this);

    connect(ui->erodeButton, &QPushButton::clicked, this, &GrayMorphInspector::ProcessBasicOperation);
    connect(ui->dilateButton, &QPushButton::clicked, this, &GrayMorphInspector::ProcessBasicOperation);
    connect(ui->opeingButton, &QPushButton::clicked, this, &GrayMorphInspector::ProcessBasicOperation);
    connect(ui->closingButton, &QPushButton::clicked, this, &GrayMorphInspector::ProcessBasicOperation);

    connect(ui->morphReconstructButton, &QPushButton::clicked, this, &GrayMorphInspector::ProcessMorphReconstruct);
    connect(ui->watershedButton, &QPushButton::clicked, this, &GrayMorphInspector::ProcessWatershed);

}

GrayMorphInspector::~GrayMorphInspector()
{
    delete ui;
}



void GrayMorphInspector::ResetImage(const QImage &newImage)
{
    BaseInspector::ResetImage(newImage);
}

BinaryImage GrayMorphInspector::GetElement()
{
    BinaryImage element(3);

    if(ui->manualRadioButton->isChecked()){

        element.raw[0][0] = ui->element00CheckBox->isChecked();
        element.raw[0][1] = ui->element01CheckBox->isChecked();
        element.raw[0][2] = ui->element02CheckBox->isChecked();
        element.raw[1][0] = ui->element10CheckBox->isChecked();
        element.raw[1][1] = ui->element11CheckBox->isChecked();
        element.raw[1][2] = ui->element12CheckBox->isChecked();
        element.raw[2][0] = ui->element20CheckBox->isChecked();
        element.raw[2][1] = ui->element21CheckBox->isChecked();
        element.raw[2][2] = ui->element22CheckBox->isChecked();

    } else if(ui->diskRadioButton->isChecked()){

        int radius = UIUtility::GetLineEditInt(ui->radiusLineEdit, 5);
        if(radius <= 0)  radius = 5;
        element = BWMorph::GetDisk(radius);

    } else if(ui->rectRadioButton->isChecked()){

        int width = UIUtility::GetLineEditInt(ui->widthLineEdit, 1);
        int height = UIUtility::GetLineEditInt(ui->widthLineEdit, 1);
        if(width <= 0) width = 1;
        if(height <= 0) height = 1;

        element = BWMorph::GetRect(width, height);
    }

    return element;
}

void GrayMorphInspector::ProcessBasicOperation()
{
    if(currentImage.isNull()){
        return;
    }

    QPushButton* from = dynamic_cast<QPushButton*>(QObject::sender());
    if(from == nullptr) return;

    GrayscaleImage grayImage(currentImage);
    BinaryImage element = GetElement();

    GrayscaleImage final;

    if(from == ui->dilateButton){
        final = GrayMorph::dilate(grayImage, element);
    } else if(from == ui->erodeButton){
        final = GrayMorph::erode(grayImage, element);
    } else if(from == ui->opeingButton){
        GrayscaleImage left = GrayMorph::erode(grayImage, element);
        final = GrayMorph::dilate(left, element);
    } else if(from == ui->closingButton){
        GrayscaleImage left = GrayMorph::dilate(grayImage, element);
        final = GrayMorph::erode(left, element);
    }

    currentImage = final.toQImage();

    emit ImageModified(currentImage);
}

void GrayMorphInspector::ProcessMorphReconstruct()
{
    if(currentImage.isNull()) return;
    GrayscaleImage mark(currentImage);

    if(!m_mask.isValid()) return;
    BinaryImage element = GetElement();
    if(!element.isValid()) return;

    GrayscaleImage reconstruct(mark);
    GrayscaleImage oldReconstruct;

    while(!(oldReconstruct == reconstruct)){
        oldReconstruct = reconstruct;
        reconstruct = GrayMorph::dilateAndMask(m_mask, reconstruct, element);
//        currentImage = reconstruct.toQImage();
//        emit ImageModified(currentImage);
    }

    currentImage = reconstruct.toQImage();

    emit ImageModified(currentImage);
}

void GrayMorphInspector::ProcessWatershed()
{
    if(currentImage.isNull()) return;
    GrayscaleImage gImage(currentImage);

    int width = gImage.width();
    int height = gImage.height();

    // translate grayscale image into linear unsigned char array (RGBA)
    int size = width * height * 4;
    unsigned char imageIn [size];
    for(int i = 0; i < height; ++i){
        for(int j = 0; j < width * 4; j += 4){
            unsigned char value = gImage.raw[i][j / 4];

            imageIn[i * width * 4 + j] = value;
            imageIn[i * width * 4 + j + 1] = value;
            imageIn[i * width * 4 + j + 2] = value;
            imageIn[i * width * 4 + j + 3] = 255;
        }
    }

    unsigned char imageOut [size];
    if(!watershed::Watershed(imageIn, imageOut, (unsigned)width, (unsigned)height)){
        return;
    }

    // translate linear uc array back to grayscale image
    for(int i = 0; i < height; ++i){
        for(int j = 0; j < width; ++j){
            gImage.raw[i][j] = imageOut[i * width * 4 + j * 4];
        }
    }

    currentImage = gImage.toQImage();
    emit ImageModified(currentImage);
}

void GrayMorphInspector::on_applyPushButton_clicked()
{
    ResetImage(currentImage);
}

void GrayMorphInspector::on_revertPushButton_clicked()
{
    ResetImage(originalImage);
}

void GrayMorphInspector::on_maskButton_clicked()
{
    QImage qImage;

    QString fileName = QFileDialog::getOpenFileName(
                this, "open image file",
                ".",
                "Image files (*.bmp *.jpg *.JPG *.gif *.GIF *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
    if(fileName != "")
    {
        if(qImage.load(fileName))
        {
            m_mask = GrayscaleImage(qImage);
        }
    }
}
