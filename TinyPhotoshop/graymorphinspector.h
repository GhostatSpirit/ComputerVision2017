#ifndef GRAYMORPHINSPECTOR_H
#define GRAYMORPHINSPECTOR_H

#include <QWidget>
#include <baseinspector.h>

#include "binaryimage.h"
#include "grayscaleimage.h"

namespace Ui {
class GrayMorphInspector;
}

class GrayMorphInspector : public BaseInspector
{
    Q_OBJECT

public:
    explicit GrayMorphInspector(const QImage& original, QWidget *parent = 0);
    ~GrayMorphInspector();
    void ResetImage(const QImage& newImage) override;

private:
    Ui::GrayMorphInspector *ui;
    GrayscaleImage m_mask;

    BinaryImage GetElement();

private slots:
    void ProcessBasicOperation();
    void ProcessMorphReconstruct();
    void ProcessWatershed();

    void on_applyPushButton_clicked();
    void on_revertPushButton_clicked();
    void on_maskButton_clicked();
};

#endif // GRAYMORPHINSPECTOR_H
