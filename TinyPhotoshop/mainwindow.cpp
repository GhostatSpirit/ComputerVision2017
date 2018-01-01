#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include "colorinspector.h"
#include "binaryinspector.h"
#include "arithmeticinspector.h"
#include "geometryinspector.h"
#include "contrastinspector.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->image = new QImage();
    this->pixmapItem = nullptr;

    this->inspector = new ColorInspector(*(this->image));
    ui->inspectorLayout->addWidget(inspector);

    // connect(ui->openImageButton, SIGNAL(clicked()), this, SLOT(on_openImageButton_clicked()));
    connect(this->inspector, &BaseInspector::ImageModified, this, &MainWindow::DisplayImage);
}

MainWindow::~MainWindow()
{
    delete image;
    delete ui;
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


void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, "open image file",
                ".",
                "Image files (*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
    if(fileName != "")
    {
        if(image->load(fileName))
        {
            QGraphicsScene *scene = new QGraphicsScene;
            pixmapItem = scene->addPixmap(QPixmap::fromImage(*image));
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
