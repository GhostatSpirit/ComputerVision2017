#ifndef BASEINSPECTOR_H
#define BASEINSPECTOR_H

#include <QWidget>

class BaseInspector : public QWidget
{
    Q_OBJECT
public:
    explicit BaseInspector(const QImage& _originalImage, QWidget *parent = nullptr);
    virtual void ResetImage(const QImage& newImage);

signals:
    void ImageModified(const QImage& modified);
protected slots:

protected:
    QImage originalImage;
    QImage currentImage;
};

#endif // BASEINSPECTOR_H
