#include "uiutility.h"

#include <QString>

UIUtility::UIUtility()
{

}

float UIUtility::GetLineEditFloat(QLineEdit *lineEdit, float fallback)
{
    QString text = lineEdit->text();
    bool ok = false;
    float f = text.toFloat(&ok);
    if(!ok){
        f = fallback;
        lineEdit->setText(QString::number(fallback));
    }

    return f;
}

int UIUtility::GetLineEditInt(QLineEdit *lineEdit, int fallback)
{
    QString text = lineEdit->text();
    bool ok = false;
    int i = text.toInt(&ok);
    if(!ok){
        i = fallback;
        lineEdit->setText(QString::number(fallback));
    }

    return i;
}

int UIUtility::GetLineEditIntClamped(QLineEdit *lineEdit, int min, int max, int fallback)
{
    QString text = lineEdit->text();
    bool ok = false;
    int i = text.toInt(&ok);
    if(!ok){
        i = fallback;
        lineEdit->setText(QString::number(fallback));
    }

    if(i > max) i = max;
    if(i < min) i = min;
    lineEdit->setText(QString::number(i));

    return i;
}
