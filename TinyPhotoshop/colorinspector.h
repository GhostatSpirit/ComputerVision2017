#ifndef COLORINSPECTOR_H
#define COLORINSPECTOR_H

#include <QWidget>
#include <QElapsedTimer>

namespace Ui {
class ColorInspector;
}

class ColorInspector : public QWidget
{
    Q_OBJECT

public:
    explicit ColorInspector(const QImage& _originalImage, QWidget *parent = 0);
    ~ColorInspector();
    void ApplyImage(const QImage& newImage);

private:
    Ui::ColorInspector *ui;

    QImage originalImage;
    QImage currentImage;

    QImage SplitChannel(const QImage& original, bool r, bool g, bool b);

    // if >=2 channels are in use, gray = (r * 11 + g * 16 + b * 5)/32.
    // else, use the active channel to perform conversion
    QImage ConvertToGrayscale(const QImage& original, bool r, bool g, bool b);

    // return 0 if >= 2 channels are in use
    // else return 1 = red, 2 = green, 3 = blue
    int GetActiveChannel(bool r, bool g, bool b);

    // H: 0 - 360
    // S: 0 - 255
    // V: 0 - 255
    void QColortoHSV(const QColor& color, int *h, int *s, int *v);
    QColor HSVtoQColor(int h, int s, int v);

    float hsvInterval;
    QElapsedTimer timer;
    // h, s, vfactor: [-1, 1], 0 means no change
    QImage TuneHSV(const QImage& original, double hfactor, double sfactor, double vfactor);

signals:
    void ImageModified(const QImage& modified);

private slots:
    void ProcessColorCheckbox();
    void ProcessGrayscale();
    void ProcessHSV();
    void on_applyPushButton_clicked();
    void on_revertPushButton_clicked();
};

#endif // COLORINSPECTOR_H
