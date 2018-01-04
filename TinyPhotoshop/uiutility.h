#ifndef UIUTILITY_H
#define UIUTILITY_H

#include <QLineEdit>

class UIUtility
{
public:
    UIUtility();
    static float GetLineEditFloat(QLineEdit *lineEdit, float fallback = 0);
    static int GetLineEditInt(QLineEdit *lineEdit, int fallback = 0);
};

#endif // UIUTILITY_H
