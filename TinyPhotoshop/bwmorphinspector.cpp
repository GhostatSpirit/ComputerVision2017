#include "bwmorphinspector.h"
#include "ui_bwmorphinspector.h"

#include "binaryimage.h"
#include "bwmorph.h"
#include "signedimage.h"
#include "binaryinspector.h"

#include "uiutility.h"
#include <QFileDialog>

BWMorphInspector::BWMorphInspector(const QImage& original, QWidget *parent) :
    BaseInspector(original, parent),
    ui(new Ui::BWMorphInspector)
{
    ui->setupUi(this);

    connect(ui->dilateButton, &QPushButton::clicked, this, &BWMorphInspector::ProcessBasicOperation);
    connect(ui->erodeButton, &QPushButton::clicked, this, &BWMorphInspector::ProcessBasicOperation);
    connect(ui->opeingButton, &QPushButton::clicked, this, &BWMorphInspector::ProcessBasicOperation);
    connect(ui->closingButton, &QPushButton::clicked, this, &BWMorphInspector::ProcessBasicOperation);

    connect(ui->hitOrMissButton, &QPushButton::clicked, this, &BWMorphInspector::ProcessHitOrMiss);
    connect(ui->thinningButton, &QPushButton::clicked, this, &BWMorphInspector::ProcessThinning);
    connect(ui->thickeningButton, &QPushButton::clicked, this, &BWMorphInspector::ProcessThickening);

    connect(ui->distanceTransformButton, &QPushButton::clicked, this, &BWMorphInspector::ProcessDistanceTransform);
    connect(ui->skeletonizationButton, &QPushButton::clicked, this, &BWMorphInspector::ProcessSkeletonization);
    connect(ui->reconstructButton, &QPushButton::clicked, this, &BWMorphInspector::ProcessReconstruct);
    connect(ui->morphReconstructButton, &QPushButton::clicked, this, &BWMorphInspector::ProcessMorphReconstruct);

    // init pLineEdit2D
    QVector<QVector<QLineEdit*>> newPLineEdit(3,  QVector<QLineEdit*>(3));
    pLineEdit2D = newPLineEdit;
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            int num = i * 3 + j;
            switch(num){
            case 0:
                pLineEdit2D[i][j] = ui->element00Line;
                break;
            case 1:
                pLineEdit2D[i][j] = ui->element01Line;
                break;
            case 2:
                pLineEdit2D[i][j] = ui->element02Line;
                break;
            case 3:
                pLineEdit2D[i][j] = ui->element10Line;
                break;
            case 4:
                pLineEdit2D[i][j] = ui->element11Line;
                break;
            case 5:
                pLineEdit2D[i][j] = ui->element12Line;
                break;
            case 6:
                pLineEdit2D[i][j] = ui->element20Line;
                break;
            case 7:
                pLineEdit2D[i][j] = ui->element21Line;
                break;
            case 8:
                pLineEdit2D[i][j] = ui->element22Line;
                break;
            }
        }
    }


}

BWMorphInspector::~BWMorphInspector()
{
    delete ui;
}

void BWMorphInspector::ResetImage(const QImage &newImage)
{
    BaseInspector::ResetImage(newImage);
}

BinaryImage BWMorphInspector::GetElement()
{
    BinaryImage element(3);

    if(ui->manualRadioButton->isChecked()){
        for(int i = 0; i < 3; ++i){
            for(int j = 0; j < 3; ++j){
                QLineEdit *pline = pLineEdit2D[i][j];
                int value = UIUtility::GetLineEditIntClamped(pline, -1, 1, 0);
                if(value > 0){
                    element.raw[i][j] = true;
                } else {
                    element.raw[i][j] = false;
                }
            }
        }

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

BinaryImage BWMorphInspector::GetMask()
{
    BinaryImage element(3);

    if(ui->manualRadioButton->isChecked()){
        for(int i = 0; i < 3; ++i){
            for(int j = 0; j < 3; ++j){
                QLineEdit *pline = pLineEdit2D[i][j];
                int value = UIUtility::GetLineEditIntClamped(pline, -1, 1, 0);
                if(value >= 0){
                    element.raw[i][j] = true;
                } else {
                    element.raw[i][j] = false;
                }
            }
        }

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

void BWMorphInspector::on_applyPushButton_clicked()
{
    ResetImage(currentImage);
}

void BWMorphInspector::on_revertPushButton_clicked()
{
    ResetImage(originalImage);
}

void BWMorphInspector::ProcessBasicOperation()
{
    if(currentImage.isNull()){
        return;
    }

    QPushButton* from = dynamic_cast<QPushButton*>(QObject::sender());
    if(from == nullptr) return;

    BinaryImage bwImage(currentImage);
    BinaryImage element = GetElement();

    BinaryImage final;

    if(from == ui->dilateButton){
        final = BWMorph::dilate(bwImage, element);
    } else if(from == ui->erodeButton){
        final = BWMorph::erode(bwImage, element);
    } else if(from == ui->opeingButton){
        BinaryImage left = BWMorph::erode(bwImage, element);
        final = BWMorph::dilate(left, element);
    } else if(from == ui->closingButton){
        BinaryImage left = BWMorph::dilate(bwImage, element);
        final = BWMorph::erode(left, element);
    }

    currentImage = final.toQImage();

    emit ImageModified(currentImage);
}

void BWMorphInspector::ProcessHitOrMiss()
{
    BinaryImage bwImage(currentImage);

//    BinaryImage element(3);
//    element.raw[0] = {0, 1, 0};
//    element.raw[1] = {0, 1, 1};
//    element.raw[2] = {0, 0, 0};

//    BinaryImage mask(3);
//    mask.raw[0] = {0, 1, 0};
//    mask.raw[1] = {1, 1, 1};
//    mask.raw[2] = {1, 1, 0};

    BinaryImage element = GetElement();
    BinaryImage mask = GetMask();


    BinaryImage final = BWMorph::hitOrMiss(bwImage, element, mask);

    element = element.rotate90();
    mask = mask.rotate90();

    currentImage = final.toQImage();

    emit ImageModified(currentImage);
}

void BWMorphInspector::ProcessThinning()
{
    BinaryImage bwImage(currentImage);

//    BinaryImage element0(3);
//    element0.raw[0] = {0, 0, 0};
//    element0.raw[1] = {0, 1, 0};
//    element0.raw[2] = {1, 1, 1};

//    BinaryImage mask0(3);
//    mask0.raw[0] = {1, 1, 1};
//    mask0.raw[1] = {0, 1, 0};
//    mask0.raw[2] = {1, 1, 1};

//    BinaryImage element1(3);
//    element1.raw[0] = {0, 0, 0};
//    element1.raw[1] = {1, 1, 0};
//    element1.raw[2] = {0, 1, 0};

//    BinaryImage mask1(3);
//    mask1.raw[0] = {0, 1, 1};
//    mask1.raw[1] = {1, 1, 1};
//    mask1.raw[2] = {0, 1, 0};

//    BinaryImage element(3);
//    element.raw[0] = {1, 1, 1};
//    element.raw[1] = {1, 1, 1};
//    element.raw[2] = {1, 1, 1};

//    BinaryImage mask(3);
//    mask.raw[0] = {1, 1, 1};
//    mask.raw[1] = {1, 1, 1};
//    mask.raw[2] = {1, 1, 1};

    BinaryImage element = GetElement();
    BinaryImage mask = GetMask();

    BinaryImage hitAndMiss = BWMorph::hitOrMiss(bwImage, element, mask);
    BinaryImage final = bwImage - hitAndMiss;

    currentImage = final.toQImage();

    emit ImageModified(currentImage);
}

void BWMorphInspector::ProcessThickening()
{
    BinaryImage bwImage(currentImage);

    BinaryImage element = GetElement();
    BinaryImage mask = GetMask();

//    BinaryImage element(3);
//    element.raw[0] = {1, 1, 0};
//    element.raw[1] = {1, 0, 0};
//    element.raw[2] = {1, 0, 0};

//    BinaryImage mask(3);
//    mask.raw[0] = {1, 1, 0};
//    mask.raw[1] = {1, 1, 0};
//    mask.raw[2] = {1, 0, 1};

//    BinaryImage element(3);
//    element.raw[0] = {0, 0, 0};
//    element.raw[1] = {0, 0, 0};
//    element.raw[2] = {0, 0, 0};

//    BinaryImage mask(3);
//    mask.raw[0] = {1, 1, 1};
//    mask.raw[1] = {1, 1, 1};
//    mask.raw[2] = {1, 1, 1};

    BinaryImage hitAndMiss = BWMorph::hitOrMiss(bwImage, element, mask);
    BinaryImage final = bwImage + hitAndMiss;

    currentImage = final.toQImage();

    emit ImageModified(currentImage);
}

void BWMorphInspector::ProcessDistanceTransform()
{
    if(currentImage.isNull()){
        return;
    }

    BinaryImage bwImage(originalImage);
    BinaryImage element = GetElement();

    SignedImage distanceImage = BWMorph::distanceTransform(bwImage, element);

    currentImage = distanceImage.toQImage();

    emit ImageModified(currentImage);
}

void BWMorphInspector::ProcessSkeletonization()
{
    BinaryImage bwImage(originalImage);
    BinaryImage element = GetElement();

    // SignedImage distanceImage = BWMorph::distanceTransform(bwImage, element);
    SignedImage skeleton = BWMorph::skeletonization(bwImage, element);

    currentImage = skeleton.toBinaryImage().toQImage();
    m_skeleton = skeleton;
    m_skeletonElement = element;

    emit ImageModified(currentImage);
}

void BWMorphInspector::ProcessReconstruct()
{
    if(!m_skeleton.isValid())  return;
    if(!m_skeletonElement.isValid()) return;

    BinaryImage reconstructed = BWMorph::skeletonReconstruction(m_skeleton, m_skeletonElement);
    currentImage = reconstructed.toQImage();

    emit ImageModified(currentImage);
}

void BWMorphInspector::ProcessMorphReconstruct()
{
    if(!m_mask.isValid() || currentImage.isNull()) return;

    BinaryImage mark(currentImage);

    BinaryImage reconstructed = BWMorph::morphReconstruction(m_mask, mark, GetElement());

    currentImage = reconstructed.toQImage();

    emit ImageModified(currentImage);
}


void BWMorphInspector::on_maskButton_clicked()
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
            qImage = BinaryInspector::ApplyOtsu(qImage);
            m_mask = BinaryImage(qImage);
        }
    }
}
