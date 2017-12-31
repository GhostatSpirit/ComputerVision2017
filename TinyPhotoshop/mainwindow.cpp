#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

#include "colorinspector.h"

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
    connect(this->inspector, &ColorInspector::ImageModified, this, &MainWindow::DisplayImage);
}

MainWindow::~MainWindow()
{
    delete image;
    delete ui;
}

void MainWindow::DisplayImage(const QImage &newImage)
{
    *image = newImage.copy();

    if(pixmapItem != nullptr){
        pixmapItem->setPixmap(QPixmap::fromImage(*image));
        ui->imageGraphicsView->show();
    }
    //ui->imageGraphicsView->scene()->addP
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

    inspector->ApplyImage(*image);
}
