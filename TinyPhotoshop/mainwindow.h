#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "colorinspector.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QImage* image;
    QGraphicsPixmapItem* pixmapItem;

    BaseInspector* inspector;

private slots:
   void on_actionOpen_triggered();
   void DisplayImage(const QImage& newImage);
   void DeleteInspector();

   void on_actionColor_triggered();

   void on_actionBinary_Operation_triggered();
   void on_actionArithmetic_triggered();
   void on_actionGeometric_triggered();
};

#endif // MAINWINDOW_H
