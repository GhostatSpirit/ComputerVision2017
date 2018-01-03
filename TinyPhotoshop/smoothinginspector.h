#ifndef SMOOTHINGINSPECTOR_H
#define SMOOTHINGINSPECTOR_H

#include <QWidget>
#include "baseinspector.h"
#include <QLineEdit>

namespace Ui {
class SmoothingInspector;
}

class SmoothingInspector : public BaseInspector
{
    Q_OBJECT

public:
    explicit SmoothingInspector(const QImage& original, QWidget *parent = 0);
    ~SmoothingInspector();

    void ResetImage(const QImage& newImage) override;

private:
    Ui::SmoothingInspector *ui;

    // interpret the lineEdit text as a float,
    // if failed, set the lineEdit to "0" and return 0
    float GetLineEditFloat(QLineEdit* lineEdit);

private slots:
    void ProcessMedian();
    void ProcessMean();
    void ProcessGaussian();

    void ProcessManual();

    void on_applyPushButton_clicked();
    void on_revertPushButton_clicked();
};

#endif // SMOOTHINGINSPECTOR_H
