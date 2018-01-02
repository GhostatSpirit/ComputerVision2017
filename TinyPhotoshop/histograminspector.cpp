#include "histograminspector.h"
#include "ui_histograminspector.h"

#include "imagehistogram.h"
#include <cmath>

HistogramInspector::HistogramInspector(const QImage &_original, QWidget *parent) :
    BaseInspector(_original, parent),
    ui(new Ui::HistogramInspector)
{
    ui->setupUi(this);

    histogramPixmapItem = nullptr;
    histogramScene = new QGraphicsScene;

    RefreshHistogram();

    connect(ui->equalizeButton, &QPushButton::clicked, this, &HistogramInspector::ProcessEqualize);
    connect(this, &HistogramInspector::ImageModified, this, &HistogramInspector::RefreshHistogram);
}

HistogramInspector::~HistogramInspector()
{
    delete ui;
    delete histogramScene;
}

void HistogramInspector::ResetImage(const QImage &newImage)
{
    BaseInspector::ResetImage(newImage);

    RefreshHistogram();
}

void HistogramInspector::DrawHistogram(QPixmap *pixmap, const QVector<int> &histogram)
{
    QPainter painter(pixmap);
    painter.setRenderHints(QPainter::Antialiasing);

    int width = pixmap->width();
    int height = pixmap->height();

    if(histogram.isEmpty()){
        painter.fillRect(0, 0, width, height, QColor::fromRgb(0, 0, 0));
        return;
    }

    qreal maxValue = 0.0;
    // find maximum value
    for(int i = 0; i < histogram.size(); ++i){
        if(histogram[i] > maxValue){
            maxValue = histogram[i];
        }
    }

    qreal barWidth = width / (qreal)histogram.size();

    for(int i = 0; i < histogram.size(); ++i){
        qreal h = histogram[i] / maxValue * height;
        // draw level
        painter.fillRect(barWidth * i, height - h, barWidth * (i + 1), height, Qt::red);
        // clear the rest the control
        painter.fillRect(barWidth * i, 0, barWidth * (i + 1), height - h, Qt::black);
    }

}

void HistogramInspector::RefreshHistogram()
{
    QGraphicsView* view = ui->histogramGraphicsView;
    QPixmap pixmap(view->width(), view->height());


    QVector<int> histogram = ImageHistogram::GetHistogram(currentImage, ImageHistogram::GRAY);
    DrawHistogram(&pixmap, histogram);

    if(histogramPixmapItem == nullptr){
        histogramPixmapItem = histogramScene->addPixmap(pixmap);
    } else {
        histogramPixmapItem->setPixmap(pixmap);
    }

    ui->histogramGraphicsView->setScene(histogramScene);
    // ui->imageGraphicsView->resize(image->width() + 10, image->height() + 10);

    ui->histogramGraphicsView->fitInView(histogramScene->itemsBoundingRect(), Qt::IgnoreAspectRatio);
    ui->histogramGraphicsView->show();
}

QImage HistogramInspector::HistogramEqualization(const QImage &original)
{
    QImage image = original;

    QVector<int> redHistogram = ImageHistogram::GetHistogram(image, ImageHistogram::RED);
    QVector<int> greenHistogram = ImageHistogram::GetHistogram(image, ImageHistogram::GREEN);
    QVector<int> blueHistogram = ImageHistogram::GetHistogram(image, ImageHistogram::BLUE);

    QVector<int> redSum = ConstructSum(redHistogram);
    QVector<int> greenSum = ConstructSum(greenHistogram);
    QVector<int> blueSum = ConstructSum(blueHistogram);

    int pixelCount = image.height() * image.width();

    for(int row = 0; row < image.height(); row++){
        for(int col = 0; col < image.width(); col++){
            QColor curColor(image.pixel(col, row));

            int red = qRed(image.pixel(col, row));
            int green = qGreen(image.pixel(col, row));
            int blue = qBlue(image.pixel(col, row));

            red = roundf(redSum[red] * 255.0 / (float) pixelCount);
            green = roundf(greenSum[green] * 255.0 / (float) pixelCount);
            blue = roundf(blueSum[blue] * 255.0 / (float) pixelCount);

            QColor newColor(red, green, blue);
            image.setPixelColor(col, row, newColor);
        }
    }

    return image;
}

QVector<int> HistogramInspector::ConstructSum(const QVector<int> &histogram)
{
    QVector<int> sums(histogram.size(), 0);
    int sum = 0;
    for(int i = 0; i < histogram.size(); ++i){
        sum += histogram[i];
        sums[i] = sum;
    }
    return sums;
}


void HistogramInspector::showEvent(QShowEvent *) {
    ui->histogramGraphicsView->fitInView(histogramScene->itemsBoundingRect(), Qt::IgnoreAspectRatio);
}

void HistogramInspector::on_applyPushButton_2_clicked()
{
    ResetImage(currentImage);
}

void HistogramInspector::on_revertPushButton_2_clicked()
{
    ResetImage(originalImage);
}

void HistogramInspector::ProcessEqualize()
{
    currentImage = HistogramEqualization(originalImage);
    emit ImageModified(currentImage);
}

