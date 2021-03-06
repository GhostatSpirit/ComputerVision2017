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

    QGraphicsScene *scene;

    BaseInspector* inspector;

    bool eventFilter(QObject *obj, QEvent *event);

private slots:
   void on_actionOpen_triggered();
   void DisplayImage(const QImage& newImage);
   void DeleteInspector();

   void SetImageLabels(qreal fx, qreal fy);
   void SetImageInvalidLabels();

   void on_actionColor_triggered();

   void on_actionBinary_Operation_triggered();
   void on_actionArithmetic_triggered();
   void on_actionGeometric_triggered();
   void on_actionConstrast_triggered();
   void on_actionHistogram_triggered();
   void on_actionSmoothing_triggered();
   void on_actionEdge_Detection_triggered();
   void on_actionBinaryMorphology_triggered();
   void on_actionGrayscale_Morphology_triggered();
   void on_actionSave_triggered();
   void on_actionHough_Transform_triggered();
};

#endif // MAINWINDOW_H
