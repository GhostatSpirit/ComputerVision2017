#ifndef BWMORPHINSPECTOR_H
#define BWMORPHINSPECTOR_H

#include <QWidget>
#include "baseinspector.h"
#include "binaryimage.h"
#include "signedimage.h"

namespace Ui {
class BWMorphInspector;
}

class BWMorphInspector : public BaseInspector
{
    Q_OBJECT

public:
    explicit BWMorphInspector(const QImage& original, QWidget *parent = 0);
    ~BWMorphInspector();
    void ResetImage(const QImage& newImage) override;

private:
    Ui::BWMorphInspector *ui;

    BinaryImage GetElement();

    SignedImage m_skeleton;
    BinaryImage m_skeletonElement;

    BinaryImage m_mask;


private slots:
    void ProcessBasicOperation();
    void ProcessHitOrMiss();
    void ProcessThinning();
    void ProcessThickening();

    void ProcessDistanceTransform();
    void ProcessSkeletonization();
    void ProcessReconstruct();
    void ProcessMorphReconstruct();

    void on_maskButton_clicked();
    void on_applyPushButton_clicked();
    void on_revertPushButton_clicked();
};

#endif // BWMORPHINSPECTOR_H
