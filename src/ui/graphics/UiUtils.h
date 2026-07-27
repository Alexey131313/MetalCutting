#pragma once
#include <QString>

namespace UiUtils
{
inline QString formatLengthMeters(double valueCm)
{
    return QString::number(valueCm / 100.0, 'f', 2);
}
}
