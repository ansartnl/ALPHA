#ifndef UPPERCASEVALIDATOR_H
#define UPPERCASEVALIDATOR_H

#include <QtGui/QValidator>

class UppercaseValidator : public QValidator
{
Q_OBJECT
public:
    explicit UppercaseValidator(QObject *parent = 0);

    virtual State validate(QString &input, int &pos) const;
signals:

public slots:

};


#endif // UPPERCASEVALIDATOR_H
