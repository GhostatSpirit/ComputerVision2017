#include "baseinspector.h"

BaseInspector::BaseInspector(QWidget *parent) : QWidget(parent)
{

}

void BaseInspector::ApplyImage(const QImage &newImage)
{
    originalImage = newImage.copy();
    currentImage = newImage.copy();
}
