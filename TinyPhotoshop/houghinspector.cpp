#include "houghinspector.h"
#include "ui_houghinspector.h"

#include "grayscaleimage.h"
#include "binaryimage.h"
#include "binaryinspector.h"
#include "binaryimage.h"
#include "bwmorph.h"
#include "colorinspector.h"
#include "histograminspector.h"
#include "contrastinspector.h"
#include "arithmeticinspector.h"


HoughInspector::HoughInspector(const QImage &original, QWidget *parent) :
    BaseInspector(original, parent),
    ui(new Ui::HoughInspector)
{
    ui->setupUi(this);

    connect(ui->detectLineButton, &QPushButton::clicked, this, &HoughInspector::ProcessDetectLine);
}

HoughInspector::~HoughInspector()
{
    delete ui;
}

void HoughInspector::ResetImage(const QImage &newImage)
{
    BaseInspector::ResetImage(newImage);
}

QImage HoughInspector::houghDetectLine(const QImage &original)
{
    int t = ui->lineThresholdSlider->value();

    QImage qbwImage = BinaryInspector::ApplyOtsu(original);
    BinaryImage bwImage(qbwImage);


    BinaryImage outputImage(bwImage.width(), bwImage.height());
    int rows = bwImage.height();
    int cols = bwImage.width();


    std::vector<int>blank(2*int(sqrt(rows * rows + cols * cols)), 0);
    std::vector<std::vector<int>>count(180, blank);	//[-90,90)

    for (int theta = -90; theta < 90; theta++) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (bwImage.raw[i][j]) {
                    double rho = abs(i*cos(theta / 57.3) + j*sin(theta / 57.3));
                    count[theta+90][round(rho)]++;
                }
            }
        }
    }
    //y=kx+b	k=tan(theta)	rho=b*cos(pi-theta)=-b*cos(theta)

    for (int i = 0; i < (int)count.size(); i++) {
        for (int j = 0; j < (int)count[0].size(); j++) {
            if (count[i][j] > t) {
                int theta = i , rho = j;
                double k = tan(theta / 57.3), b = -rho / cos(theta / 57.3);
                for (int x = 0; x < rows; x++) {
                    int y = round(k*x+b);
                    if (y >= 0 && y < cols) {
                        outputImage.raw[x][y] = true;
                    }
                }
                for (int y = 0; y < cols; y++) {
                    int x = round((y - b) / k);
                    if (x >= 0 && x < rows) {
                        outputImage.raw[x][y] = true;
                    }
                }
            }
        }
    }

    QImage marked = original;
    QColor red(255, 0, 0);


    for(int row = 0; row < rows; ++row){
        for(int col = 0; col < cols; ++col){
            if(outputImage.raw[row][col]){
                // mark the original image
                marked.setPixelColor(col, row, red);
            }
        }
    }

    return marked;
}

void HoughInspector::on_applyPushButton_clicked()
{
    ResetImage(currentImage);
}

void HoughInspector::on_revertPushButton_clicked()
{
    ResetImage(originalImage);
}

void HoughInspector::ProcessDetectLine()
{
    currentImage = houghDetectLine(originalImage);

    emit ImageModified(currentImage);
}

void HoughInspector::on_eyeballButton_clicked()
{
    QImage original = originalImage;

    QImage background = BinaryInspector::ApplyOtsu(original);
    background = ContrastInspector::LinearContrast(background, -1);

    QImage gray = ColorInspector::ConvertToGrayscale(original, false, true, false);

    gray = ContrastInspector::LinearContrast(gray, 4.5);
    gray = ArithmeticInspector::ImageAdd(gray, background);

    QImage qBwImage = BinaryInspector::ApplyOtsu(gray);
    qBwImage = ContrastInspector::LinearContrast(qBwImage, -1);

    BinaryImage bwImage(qBwImage);
    BinaryImage element = BWMorph::GetDisk(10);

    BinaryImage left = BWMorph::erode(bwImage, element);
    left = BWMorph::dilate(left, element);


    QImage final = ArithmeticInspector::ImageAdd(original, left.toQImage());

    currentImage = final;


//    gray = HistogramInspector::HistogramEqualization(gray);
//    QImage qBwImage = BinaryInspector::ApplyOtsu(gray);



//    qBwImage = ArithmeticInspector::ImageAdd(qBwImage, background);
//    qBwImage = ContrastInspector::LinearContrast(qBwImage, -1);

//    currentImage = qBwImage;

    return ImageModified(currentImage);
}
