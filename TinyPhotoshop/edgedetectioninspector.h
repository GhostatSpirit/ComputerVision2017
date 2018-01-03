#ifndef EDGEDETECTIONINSPECTOR_H
#define EDGEDETECTIONINSPECTOR_H

#include <QWidget>
#include <QLineEdit>
#include "baseinspector.h"

namespace Ui {
class EdgeDetectionInspector;
}

class EdgeDetectionInspector : public BaseInspector
{
    Q_OBJECT

public:
    explicit EdgeDetectionInspector(const QImage& original, QWidget *parent = 0);
    ~EdgeDetectionInspector();

    void ResetImage(const QImage& newImage) override;

    static float GetLineEditFloat(QLineEdit *lineEdit);
private:
    Ui::EdgeDetectionInspector *ui;

private slots:
    void on_applyPushButton_clicked();
    void on_revertPushButton_clicked();

    void ProcessSobel();
    void ProcessLaplacean();
    void ProcessCanny();

};

#endif // EDGEDETECTIONINSPECTOR_H
