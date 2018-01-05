#include "geometryinspector.h"
#include "ui_geometryinspector.h"
#include <cmath>

#include <QVector2D>
#include <QDebug>

#include "uiutility.h"

GeometryInspector::GeometryInspector(const QImage& _image, QWidget *parent) :
    BaseInspector(_image, parent),
    ui(new Ui::GeometryInspector),
    zoomInterval(0.5)
{
    ui->setupUi(this);

    connect(ui->zoomSlider, &QSlider::valueChanged, this, &GeometryInspector::ProcessZoom);
    connect(ui->bilinearRadioButton, &QRadioButton::clicked, this, &GeometryInspector::ProcessZoom);
    connect(ui->nearestRadioButton, &QRadioButton::clicked, this, &GeometryInspector::ProcessZoom);



    connect(ui->rotateSlider, &QSlider::valueChanged, this, &GeometryInspector::ProcessRotate);
    connect(ui->bilinearRadioButton, &QRadioButton::clicked, this, &GeometryInspector::ProcessRotate);
    connect(ui->nearestRadioButton, &QRadioButton::clicked, this, &GeometryInspector::ProcessRotate);
}

GeometryInspector::~GeometryInspector()
{
    delete ui;
}

void GeometryInspector::ResetImage(const QImage &newImage)
{
    BaseInspector::ResetImage(newImage);
    ui->rotateSlider->setValue(0);
    ui->zoomSlider->setValue(0);


}

QImage GeometryInspector::ZoomImage(const QImage &original, qreal zoomFactor, GeometryInspector::Interpolation method)
{
    QImage newImage = original.copy();
    int width = original.width();
    int height = original.height();

    // zoomFactor = 0.5;

    QColor black(0, 0, 0);
    switch (method) {
    case NEAREST:
        for(int row = 0; row < newImage.height(); row++){
            for(int col = 0; col < newImage.width(); col++){
                QVector2D oldPoint = GetZoomCoordinates(width, height, col, row, zoomFactor);
                int oldX = (int)roundf(oldPoint.x());
                int oldY = (int)roundf(oldPoint.y());

                if(UIUtility::isCoordValid(oldX, oldY, width, height)){
                    QColor oldColor(original.pixel(oldX, oldY));
                    newImage.setPixelColor(col, row, oldColor);
                } else {
                    newImage.setPixelColor(col, row, black);
                }
            }
        }
        break;
    case BILINEAR:
        for(int row = 0; row < newImage.height(); row++){
            for(int col = 0; col < newImage.width(); col++){
                QVector2D oldPoint = GetZoomCoordinates(width, height, col, row, zoomFactor);
                int x0 = (int)floorf(oldPoint.x());
                int x1 = (int)ceilf(oldPoint.x());
                float x0factor = x1 - oldPoint.x();

                int y0 = (int)floorf(oldPoint.y());
                int y1 = (int)ceilf(oldPoint.y());
                float y0factor = y1 - oldPoint.y();

                QColor c00(0, 0, 0);
                QColor c01(0, 0, 0);
                QColor c10(0, 0, 0);
                QColor c11(0, 0, 0);

                if(UIUtility::isCoordValid(x0, y0, width, height)){
                    c00.setRgb(original.pixel(x0, y0));
                }
                if(UIUtility::isCoordValid(x0, y1, width, height)){
                    c01.setRgb(original.pixel(x0, y1));
                }
                if(UIUtility::isCoordValid(x1, y0, width, height)){
                    c10.setRgb(original.pixel(x1, y0));
                }
                if(UIUtility::isCoordValid(x1, y1, width, height)){
                    c11.setRgb(original.pixel(x1, y1));
                }

//                QColor c00(original.pixel(x0, y0));
//                QColor c01(original.pixel(x0, y1));
//                QColor c10(original.pixel(x1, y0));
//                QColor c11(original.pixel(x1, y1));

                QColor c0 = BlendColor(c00, c10, x0factor);
                QColor c1 = BlendColor(c01, c11, x0factor);

                QColor final = BlendColor(c0, c1, y0factor);

                newImage.setPixelColor(col, row, final);
            }
        }
        break;
    }

    return newImage;
}

QImage GeometryInspector::RotateImage(const QImage &original, qreal angle, GeometryInspector::Interpolation method)
{
    int oldWidth = original.width(), oldHeight = original.height();
    int newWidth, newHeight;
    GetRotatedImageSize(oldWidth, oldHeight, angle, &newWidth, &newHeight);

    QTransform transform;
//    transform.translate(-newWidth / 2.0, -newHeight / 2.0);
//    transform.rotate(-angle);
//    transform.translate(oldWidth / 2.0, oldHeight / 2.0);
    //transform.translate(-oldWidth / 2.0, -oldHeight / 2.0);
    transform.rotate(angle);
    //transform.translate(newWidth / 2.0, newHeight / 2.0);

    transform = transform.inverted();

    QImage newImage(newWidth, newHeight, original.format());
    newImage.fill(Qt::black);


    switch (method) {
    case NEAREST:
        for(int row = 0; row < newImage.height(); row++){
            for(int col = 0; col < newImage.width(); col++){
                QPointF newPoint(col - newWidth / 2.0, row - newHeight / 2.0);
                QPointF oldPoint = transform.map(newPoint);
                oldPoint.setX(oldPoint.x() + oldWidth / 2.0);
                oldPoint.setY(oldPoint.y() + oldHeight / 2.0);

                int oldX = (int)roundf(oldPoint.x());
                int oldY = (int)roundf(oldPoint.y());

                if(oldX < 0 || oldX >= oldWidth || oldY < 0 || oldY >= oldHeight){
                    continue;
                } else {
                    QColor oldColor(original.pixel(oldX, oldY));
                    newImage.setPixelColor(col, row, oldColor);
                }
            }
        }
        break;
    case BILINEAR:
        for(int row = 0; row < newImage.height(); row++){
            for(int col = 0; col < newImage.width(); col++){
                QPointF newPoint(col - newWidth / 2.0, row - newHeight / 2.0);
                QPointF oldPoint = transform.map(newPoint);
                oldPoint.setX(oldPoint.x() + oldWidth / 2.0);
                oldPoint.setY(oldPoint.y() + oldHeight / 2.0);

                int oldX = (int)roundf(oldPoint.x());
                int oldY = (int)roundf(oldPoint.y());

                if(oldX < 0 || oldX >= oldWidth || oldY < 0 || oldY >= oldHeight){
                    continue;
                } else {
                    int x0 = (int)floorf(oldPoint.x());
                    int x1 = (int)ceilf(oldPoint.x());
                    float x0factor = x1 - oldPoint.x();

                    int y0 = (int)floorf(oldPoint.y());
                    int y1 = (int)ceilf(oldPoint.y());
                    float y0factor = y1 - oldPoint.y();

                    QColor c00 = Qt::black, c01 = Qt::black, c10 = Qt::black, c11 = Qt::black;
                    if(original.valid(x0, y0))
                        c00 = (original.pixel(x0, y0));
                    if(original.valid(x0, y1))
                        c01 = (original.pixel(x0, y1));
                    if(original.valid(x1, y0))
                        c10 = (original.pixel(x1, y0));
                    if(original.valid(x1, y1))
                        c11 = (original.pixel(x1, y1));

                    QColor c0 = BlendColor(c00, c10, x0factor);
                    QColor c1 = BlendColor(c01, c11, x0factor);

                    QColor final = BlendColor(c0, c1, y0factor);

                    newImage.setPixelColor(col, row, final);
                }
            }
        }
    }



    return newImage;
}

QVector2D GeometryInspector::GetZoomCoordinates(int width, int height, int newix, int newiy, qreal factor)
{
    qreal hwidth = width / 2.0;
    qreal hheight = height / 2.0;

    qreal newX = newix;
    qreal newY = newiy;
    qreal oldX, oldY;
    oldX = (newX - hwidth) / factor + hwidth;
    oldY = (newY - hheight) / factor + hheight;

    float foldX = (float) oldX;
    float foldY = (float) oldY;

    return QVector2D(foldX, foldY);
}

void GeometryInspector::GetRotatedImageSize(int width, int height, qreal angle, int *newWidth, int *newHeight)
{
    QTransform transform;
    transform.rotate(angle);

    // rotate four vertex in the rectangle
    QPoint p00(0, 0);
    QPoint p10(width, 0);
    QPoint p01(0, height);
    QPoint p11(width, height);

    QVector<QPoint> points = QVector<QPoint>{p00, p10, p01, p11};
    for(int i = 0; i < points.size(); ++i){
        points[i] = transform.map(points[i]);
    }

    int minx = p00.x(), miny = p00.y(), maxx = p00.x(), maxy = p00.y();
    for(int i = 0; i < points.size(); ++i){
        int curx = points[i].x();
        int cury = points[i].y();
        if(curx < minx) minx = curx;
        if(curx > maxx) maxx = curx;
        if(cury < miny) miny = cury;
        if(cury > maxy) maxy = cury;
    }

    *newWidth = maxx - minx;
    *newHeight = maxy - miny;
}


QColor GeometryInspector::BlendColor(const QColor &lcolor, const QColor &rcolor, const float lfactor)
{
    float redf = lcolor.red() * lfactor + rcolor.red() * (1.0 - lfactor);
    float greenf = lcolor.green() * lfactor + rcolor.green() * (1.0 - lfactor);
    float bluef = lcolor.blue() * lfactor + rcolor.blue() * (1.0 - lfactor);

    int r = (int)roundf(redf);
    int g = (int)roundf(greenf);
    int b = (int)roundf(bluef);

    if(r > 255) r = 255;
    if(g > 255) g = 255;
    if(b > 255) b = 255;

    QColor ret = lcolor;
    ret.setRed(r);
    ret.setGreen(g);
    ret.setBlue(b);

    return ret;
}

void GeometryInspector::on_applyPushButton_clicked()
{
    ResetImage(currentImage);
}

void GeometryInspector::on_revertPushButton_clicked()
{
    ResetImage(originalImage);
}

void GeometryInspector::ProcessZoom()
{
    qreal min, max, cur;

    qreal zfactor;

    min = ui->zoomSlider->minimum();
    max = ui->zoomSlider->maximum();
    cur = ui->zoomSlider->value();



    if(cur == 0){
        return;
    } else if(cur > 0){
        // zooming in
        zfactor = cur / max * maxZoomFactor + 1.0;
    } else {
        // zooming out
        qreal step = 1.0 / (-min);
        qreal now = -cur;
        zfactor = 1.0 - step * now;
    }

    // zfactor = (((cur - min) / (max - min))) * maxZoomFactor + 1.0;

    if(ui->nearestRadioButton->isChecked()){
        currentImage = ZoomImage(originalImage, zfactor, NEAREST);
        emit ImageModified(currentImage);
    } else if(ui->bilinearRadioButton->isChecked()){
        if(!zoomTimer.isValid()){
            zoomTimer.start();
            currentImage = ZoomImage(originalImage, zfactor, BILINEAR);
            emit ImageModified(currentImage);
        }
        if(zoomTimer.elapsed() >= zoomInterval * 1000.0){
            zoomTimer.restart();
            currentImage = ZoomImage(originalImage, zfactor, BILINEAR);
            emit ImageModified(currentImage);
        }
    }

}

void GeometryInspector::ProcessRotate()
{

    int angle = ui->rotateSlider->value();

    if(ui->nearestRadioButton->isChecked()){
        currentImage = RotateImage(originalImage, angle, NEAREST);
        emit ImageModified(currentImage);
    } else if(ui->bilinearRadioButton->isChecked()){
        if(!zoomTimer.isValid()){
            zoomTimer.start();
            currentImage = RotateImage(originalImage, angle, BILINEAR);
            emit ImageModified(currentImage);
        }
        if(zoomTimer.elapsed() >= zoomInterval * 1000.0){
            zoomTimer.restart();
            currentImage = RotateImage(originalImage, angle, BILINEAR);
            emit ImageModified(currentImage);
        }
    }

}
