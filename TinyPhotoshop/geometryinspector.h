#ifndef GEOMETRYINSPECTOR_H
#define GEOMETRYINSPECTOR_H

#include <QWidget>
#include <QElapsedTimer>
#include <baseinspector.h>

namespace Ui {
class GeometryInspector;
}

class GeometryInspector : public BaseInspector
{
    Q_OBJECT

public:
    explicit GeometryInspector(const QImage& _image, QWidget *parent = 0);
    ~GeometryInspector();

    enum Interpolation{
        NEAREST,
        BILINEAR
    };

    void ResetImage(const QImage& newImage) override;

    QImage ZoomImage(const QImage& original, qreal zoomFactor, Interpolation method);
    QImage RotateImage(const QImage& original, qreal rotateAngle, Interpolation method);

    QVector2D GetZoomCoordinates(int width, int height, int newX, int newY, qreal factor);
    // QVector2D GetRotateCoordinates(const QImage& original, int newX, int newY, qreal angle);

    void GetRotatedImageSize(int width, int height, qreal angle, int* newWidth, int* newHeight);

    static constexpr float maxZoomFactor = 3.0f;

    QColor BlendColor(const QColor& l, const QColor& r, const float lfactor);

private:
    Ui::GeometryInspector *ui;

    float zoomInterval;
    QElapsedTimer zoomTimer;

private slots:
    void on_applyPushButton_clicked();
    void on_revertPushButton_clicked();

    void ProcessZoom();
    void ProcessRotate();
};

#endif // GEOMETRYINSPECTOR_H
