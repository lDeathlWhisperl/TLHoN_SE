#ifndef POSITIVEINTVALIDATOR_H
#define POSITIVEINTVALIDATOR_H

#include <QValidator>

class PositiveIntValidator : public QValidator
{
public:
    PositiveIntValidator(QObject *parent = nullptr) : QValidator(parent) {}

    QValidator::State validate(QString &input, int &pos) const override;
};

#endif // POSITIVEINTVALIDATOR_H
