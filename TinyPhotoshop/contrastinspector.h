#ifndef CONTRASTINSPECTOR_H
#define CONTRASTINSPECTOR_H

#include <QWidget>
#include "baseinspector.h"

namespace Ui {
class ContrastInspector;
}

class ContrastInspector : public BaseInspector
{
    Q_OBJECT

public:
    explicit ContrastInspector(const QImage& _original, QWidget *parent = 0);
    ~ContrastInspector();

    void ResetImage(const QImage& newImage) override;
    static QImage LinearContrast(const QImage& image, float k);
    QImage LogContrast(const QImage& image, float c);
    QImage PowerContrast(const QImage& image, float c, float gamma);

private slots:
    void on_applyPushButton_clicked();
    void on_revertPushButton_clicked();
    void ProcessButtonClicked();

private:
    Ui::ContrastInspector *ui;

};

#endif // CONTRASTINSPECTOR_H
