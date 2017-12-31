#ifndef BASEINSPECTOR_H
#define BASEINSPECTOR_H

#include <QWidget>

class BaseInspector : public QWidget
{
    Q_OBJECT
public:
    explicit BaseInspector(QWidget *parent = nullptr);
    virtual void ApplyImage(const QImage& newImage);

signals:
    void ImageModified(const QImage& modified);
public slots:


protected:
    QImage originalImage;
    QImage currentImage;
};

#endif // BASEINSPECTOR_H
