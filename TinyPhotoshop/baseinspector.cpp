#include "baseinspector.h"

BaseInspector::BaseInspector(const QImage& _originalImage, QWidget *parent) : QWidget(parent)
{
    originalImage = _originalImage.copy();
    currentImage = _originalImage.copy();
}

void BaseInspector::ResetImage(const QImage &newImage)
{
    originalImage = newImage.copy();
    currentImage = newImage.copy();
    emit ImageModified(currentImage);
}
