#ifndef UIUTILITY_H
#define UIUTILITY_H

#include <QLineEdit>

class UIUtility
{
public:
    UIUtility();
    static float GetLineEditFloat(QLineEdit *lineEdit, float fallback = 0);
    static int GetLineEditInt(QLineEdit *lineEdit, int fallback = 0);

    static int GetLineEditIntClamped(QLineEdit *lineEdit, int min, int max, int fallback);

    static bool isCoordValid(int x, int y, int width, int height)
    {
        return (y >= 0 && y < height && x >= 0 && x < width);
    }

};

#endif // UIUTILITY_H
