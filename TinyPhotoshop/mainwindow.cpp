#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QMouseEvent>

#include "colorinspector.h"
#include "binaryinspector.h"
#include "arithmeticinspector.h"
#include "geometryinspector.h"
#include "contrastinspector.h"
#include "histograminspector.h"
#include "smoothinginspector.h"
#include "edgedetectioninspector.h"
#include "bwmorphinspector.h"
#include "graymorphinspector.h"
#include "houghinspector.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qApp->installEventFilter(this);

    this->image = new QImage();
    this->pixmapItem = nullptr;

    this->inspector = new ColorInspector(*(this->image));

    this->scene = new QGraphicsScene;

    ui->inspectorLayout->addWidget(inspector);

    // connect(ui->openImageButton, SIGNAL(clicked()), this, SLOT(on_openImageButton_clicked()));
    connect(this->inspector, &BaseInspector::ImageModified, this, &MainWindow::DisplayImage);
}

MainWindow::~MainWindow()
{
    delete scene;
    delete image;
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(image->isNull()){
        return false;
    }

    if(event->type() == QEvent::MouseMove){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
//        QString string = QString("Mouse move (%1, %2)").arg(mouseEvent->pos().x()).arg(mouseEvent->pos().y());
//        qDebug() << string;

        QPoint local_pt = ui->imageGraphicsView->mapFromGlobal(mouseEvent->globalPos());
        QPointF scene_coord_pt = ui->imageGraphicsView->mapToScene(local_pt);
        QPointF img_coord_pt = pixmapItem->mapFromScene(scene_coord_pt);

        // qDebug() << img_coord_pt;

        SetImageLabels(img_coord_pt.x(), img_coord_pt.y());
    }
    return false;
}

void MainWindow::DisplayImage(const QImage &newImage)
{
    *image = newImage;

    if(pixmapItem != nullptr){
        pixmapItem->setPixmap(QPixmap::fromImage(*image));
        QGraphicsScene *scene = ui->imageGraphicsView->scene();
        ui->imageGraphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
        ui->imageGraphicsView->show();
    }
    //ui->imageGraphicsView->scene()->addP
}

void MainWindow::DeleteInspector()
{
    if(inspector != nullptr){
        ui->inspectorLayout->removeWidget(inspector);
        inspector->setParent(0);
        delete inspector;
        inspector = nullptr;
    }
}

void MainWindow::SetImageLabels(qreal fx, qreal fy)
{
    int x = round(fx);
    int y = round(fy);

    int width = image->width();
    int height = image->height();

    if(x < 0 || x >= width || y < 0 || y >= height){
        SetImageInvalidLabels();
        return;
    } else {
        ui->xLabel->setText(QString::number(x));
        ui->yLabel->setText(QString::number(y));

        QColor color(image->pixelColor(x, y));
        int r = color.red();
        int g = color.green();
        int b = color.blue();

        ui->rLabel->setText(QString::number(r));
        ui->gLabel->setText(QString::number(g));
        ui->bLabel->setText(QString::number(b));
    }
}

void MainWindow::SetImageInvalidLabels()
{
    ui->xLabel->setText("-");
    ui->yLabel->setText("-");

    ui->rLabel->setText("-");
    ui->gLabel->setText("-");
    ui->bLabel->setText("-");
}


void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, "open image file",
                ".",
                "Image files (*.bmp *.jpg *.JPG *.gif *.GIF *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
    if(fileName != "")
    {
        if(image->load(fileName))
        {
            *image = image->convertToFormat(QImage::Format_RGBA8888);

            if(pixmapItem == nullptr){
                pixmapItem = scene->addPixmap(QPixmap::fromImage(*image));
            } else {
                pixmapItem->setPixmap(QPixmap::fromImage(*image));
            }
            ui->imageGraphicsView->setScene(scene);
            // ui->imageGraphicsView->resize(image->width() + 10, image->height() + 10);
            ui->imageGraphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
            ui->imageGraphicsView->show();
        }
    }

    inspector->ResetImage(*image);
}

void MainWindow::on_actionColor_triggered()
{
    if(dynamic_cast<ColorInspector*>(inspector) == nullptr){
        // the current inspector is not color inspector
        qDebug() << "not color inspector";

        DeleteInspector();
        this->inspector = new ColorInspector(*(this->image));
        ui->inspectorLayout->addWidget(inspector);
        connect(this->inspector, &BaseInspector::ImageModified, this, &MainWindow::DisplayImage);
    }
}



void MainWindow::on_actionBinary_Operation_triggered()
{
    if(dynamic_cast<BinaryInspector*>(inspector) == nullptr){
        // the current inspector is not binary inspector
        qDebug() << "not binary inspector";

        DeleteInspector();
        this->inspector = new BinaryInspector(*(this->image));
        ui->inspectorLayout->addWidget(inspector);
        connect(this->inspector, &BaseInspector::ImageModified, this, &MainWindow::DisplayImage);
    }
}

void MainWindow::on_actionArithmetic_triggered()
{
    if(dynamic_cast<ArithmeticInspector*>(inspector) == nullptr){
        DeleteInspector();
        this->inspector = new ArithmeticInspector(*(this->image));
        ui->inspectorLayout->addWidget(inspector);
        connect(this->inspector, &BaseInspector::ImageModified, this, &MainWindow::DisplayImage);
    }
}

void MainWindow::on_actionGeometric_triggered()
{
    if(dynamic_cast<GeometryInspector*>(inspector) == nullptr){
        DeleteInspector();
        this->inspector = new GeometryInspector(*(this->image));
        ui->inspectorLayout->addWidget(inspector);
        connect(this->inspector, &BaseInspector::ImageModified, this, &MainWindow::DisplayImage);
    }
}

void MainWindow::on_actionConstrast_triggered()
{
    if(dynamic_cast<ContrastInspector*>(inspector) == nullptr){
        DeleteInspector();
        this->inspector = new ContrastInspector(*(this->image));
        ui->inspectorLayout->addWidget(inspector);
        connect(this->inspector, &BaseInspector::ImageModified, this, &MainWindow::DisplayImage);
    }
}

void MainWindow::on_actionHistogram_triggered()
{
    if(dynamic_cast<HistogramInspector*>(inspector) == nullptr){
        DeleteInspector();
        this->inspector = new HistogramInspector(*(this->image));
        ui->inspectorLayout->addWidget(inspector);
        connect(this->inspector, &BaseInspector::ImageModified, this, &MainWindow::DisplayImage);
    }
}

void MainWindow::on_actionSmoothing_triggered()
{
    if(dynamic_cast<SmoothingInspector*>(inspector) == nullptr){
        DeleteInspector();
        this->inspector = new SmoothingInspector(*(this->image));
        ui->inspectorLayout->addWidget(inspector);
        connect(this->inspector, &BaseInspector::ImageModified, this, &MainWindow::DisplayImage);
    }
}

void MainWindow::on_actionEdge_Detection_triggered()
{
    if(dynamic_cast<EdgeDetectionInspector*>(inspector) == nullptr){
        DeleteInspector();
        this->inspector = new EdgeDetectionInspector(*(this->image));
        ui->inspectorLayout->addWidget(inspector);
        connect(this->inspector, &BaseInspector::ImageModified, this, &MainWindow::DisplayImage);
    }
}

void MainWindow::on_actionBinaryMorphology_triggered()
{
    if(dynamic_cast<BWMorphInspector*>(inspector) == nullptr){
        DeleteInspector();
        this->inspector = new BWMorphInspector(*(this->image));
        ui->inspectorLayout->addWidget(inspector);
        connect(this->inspector, &BaseInspector::ImageModified, this, &MainWindow::DisplayImage);
    }
}

void MainWindow::on_actionGrayscale_Morphology_triggered()
{
    if(dynamic_cast<GrayMorphInspector*>(inspector) == nullptr){
        DeleteInspector();
        this->inspector = new GrayMorphInspector(*(this->image));
        ui->inspectorLayout->addWidget(inspector);
        connect(this->inspector, &BaseInspector::ImageModified, this, &MainWindow::DisplayImage);
    }
}

void MainWindow::on_actionSave_triggered()
{
    if(image->isNull()){
        return;
    } else {
        //*.bmp *.jpg *.JPG *.gif *.GIF *.pbm *.pgm *.png
        QString fileName = QFileDialog::getSaveFileName(
                    this, "save image file",
                    ".",
                    "JPEG (*.jpg *.jpeg);;PNG (*.png);;BMP (*.bmp)");
        if(fileName != nullptr){
            image->save(fileName);
        }
    }
}

void MainWindow::on_actionHough_Transform_triggered()
{
    if(dynamic_cast<HoughInspector*>(inspector) == nullptr){
        DeleteInspector();
        this->inspector = new HoughInspector(*(this->image));
        ui->inspectorLayout->addWidget(inspector);
        connect(this->inspector, &BaseInspector::ImageModified, this, &MainWindow::DisplayImage);
    }
}
