#ifndef BINARYINSPECTOR_H
#define BINARYINSPECTOR_H

#include <QWidget>
#include "baseinspector.h"

namespace Ui {
class BinaryInspector;
}

class BinaryInspector : public BaseInspector
{
    Q_OBJECT

public:
    explicit BinaryInspector(const QImage& _image, QWidget *parent = 0);
    ~BinaryInspector();

    void ResetImage(const QImage& newImage) override;

public:
    static QImage ApplyOtsu(const QImage& original);
    QImage ApplyManual(const QImage& original, int minVal, int maxVal);
    static int OtsuThreshold(const QImage& image);

private slots:
    void on_applyPushButton_clicked();
    void on_revertPushButton_clicked();

    void ProcessOtsu();

    void ProcessManualClicked();
    void ProcessManualValueChange();
    void ProcessManualString();

private:
    Ui::BinaryInspector *ui;
};

#endif // BINARYINSPECTOR_H
