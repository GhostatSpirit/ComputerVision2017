#ifndef HOUGHINSPECTOR_H
#define HOUGHINSPECTOR_H

#include <QWidget>
#include "baseinspector.h"

namespace Ui {
class HoughInspector;
}

class HoughInspector : public BaseInspector
{
    Q_OBJECT

public:
    explicit HoughInspector(const QImage& original, QWidget *parent = 0);
    ~HoughInspector();
    void ResetImage(const QImage& newImage) override;

    QImage houghDetectLine(const QImage& original);

private slots:
    void on_applyPushButton_clicked();
    void on_revertPushButton_clicked();

    void ProcessDetectLine();

    void on_eyeballButton_clicked();

private:
    Ui::HoughInspector *ui;
};

#endif // HOUGHINSPECTOR_H
