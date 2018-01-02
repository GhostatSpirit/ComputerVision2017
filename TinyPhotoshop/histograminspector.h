#ifndef HISTOGRAMINSPECTOR_H
#define HISTOGRAMINSPECTOR_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "baseinspector.h"

namespace Ui {
class HistogramInspector;
}

class HistogramInspector : public BaseInspector
{
    Q_OBJECT

public:
    explicit HistogramInspector(const QImage& _original, QWidget *parent = 0);
    ~HistogramInspector();

    void ResetImage(const QImage& newImage) override;

    void DrawHistogram(QPixmap* pixmap, const QVector<int>& histogram);


    // equalize histogram in each channel
    QImage HistogramEqualization(const QImage& original);
    QVector<int> ConstructSum(const QVector<int>& histogram);

    void showEvent(QShowEvent *);
private:
    Ui::HistogramInspector *ui;
    QGraphicsPixmapItem* histogramPixmapItem;
    QGraphicsScene* histogramScene;

public slots:
    void RefreshHistogram();

private slots:
    void on_applyPushButton_2_clicked();
    void on_revertPushButton_2_clicked();

    void ProcessEqualize();
};



#endif // HISTOGRAMINSPECTOR_H
