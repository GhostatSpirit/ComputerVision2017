#ifndef BWMORPHINSPECTOR_H
#define BWMORPHINSPECTOR_H

#include <QWidget>
#include "baseinspector.h"

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

private slots:
    void on_applyPushButton_clicked();
    void on_revertPushButton_clicked();

    void ProcessDilate();
    void ProcessErode();
};

#endif // BWMORPHINSPECTOR_H
