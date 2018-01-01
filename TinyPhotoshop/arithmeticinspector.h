#ifndef ARITHMETICINSPECTOR_H
#define ARITHMETICINSPECTOR_H

#include <QWidget>
#include "baseinspector.h"

namespace Ui {
class ArithmeticInspector;
}

class ArithmeticInspector : public BaseInspector
{
    Q_OBJECT

public:
    explicit ArithmeticInspector(const QImage& _image, QWidget *parent = 0);
    ~ArithmeticInspector();

    void ResetImage(const QImage& newImage) override;

    QImage ImageAdd(const QImage& left, const QImage& right);
    QImage ImageMinus(const QImage& left, const QImage& right);
    QImage ImageMultiply(const QImage& left, const QImage& right);

private:
    bool CreateResizedImage();

    Ui::ArithmeticInspector *ui;

    QImage operationImage;
    QImage resizedImage;

private slots:
    void on_applyPushButton_clicked();
    void on_revertPushButton_clicked();

    void LoadOperationImage();
    void ProcessOperationButton();
};

#endif // ARITHMETICINSPECTOR_H
