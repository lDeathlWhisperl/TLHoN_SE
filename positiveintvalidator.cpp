#include "positiveintvalidator.h"

QValidator::State PositiveIntValidator::validate(QString &input, int &pos) const
{
    if (input.isEmpty()) return QValidator::Intermediate;

    bool ok;
    int value = input.toInt(&ok);
    if (ok && value > 0) return QValidator::Acceptable;

    return QValidator::Invalid;
}
