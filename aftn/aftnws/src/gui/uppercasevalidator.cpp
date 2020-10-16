#include "uppercasevalidator.h"

UppercaseValidator::UppercaseValidator(QObject *parent) :
    QValidator(parent)
{
}

UppercaseValidator::State UppercaseValidator::validate(QString &input, int &pos) const
{
  input.remove(QRegExp("\\s*"));
  input = input.toUpper();
  return Acceptable;
}
