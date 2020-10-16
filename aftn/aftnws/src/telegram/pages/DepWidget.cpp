#include "DepWidget.h"

#include "replies.h"
#include "aftnmessageprocessor.h"
#include "main.h"

#include <QMessageBox>

DepWidget::DepWidget(AFTNMessageProcessor *p, QWidget *parent) :
    QWidget(parent), mProcessor(p)
{
    setupUi(this);

    setVersion(ParseVersion::n_4444);

    invalidValue = "background-color: #FF637F;";

    connect(acid, SIGNAL(textChanged(QString)), SLOT(onTextChangedACID(QString)));
    connect(adep, SIGNAL(textChanged(QString)), SLOT(onEditTextChangeAdep(QString)));
    connect(ades, SIGNAL(textChanged(QString)), SLOT(onEditTextChangeAdes(QString)));

#ifndef DUSHANBE_BUILD
    connect(acid, SIGNAL(textChanged(QString)), SLOT(validateDep()));
    connect(adep, SIGNAL(textChanged(QString)), SLOT(validateDep()));
    connect(ades, SIGNAL(textChanged(QString)), SLOT(validateDep()));
    connect(departureTime, SIGNAL(editingFinished()), SLOT(validateDep()));

    validateDep();
#endif
}

void DepWidget::setReadOnly(bool r)
{
    acid->setEnabled(!r);
    adep->setEnabled(!r);
    departureTime->setEnabled(!r);
    ades->setEnabled(!r);
}

void DepWidget::setDep(const DepStruct &d)
{
    setVersion(d.version);
    acid->setText(d.ACID);
    adep->setText(d.ADEP);
//    departureTime->setTime(QTime::currentTime());
    departureTime->setTime(QTime::fromString(d.dtADEP, "hh:mm"));
    ades->setText(d.ADES);

#ifndef DUSHANBE_BUILD
    validateDep();
#endif
}

void DepWidget::setVersion(int v)
{
    version = v;
    if (v == ParseVersion::n_TC95)
        telegramType->setText(Ats::DEP1);
    else
        telegramType->setText(Ats::DEP);
}

DepStruct DepWidget::dep() const
{
    DepStruct d;

    d.version = version;
    d.ACID = acid->text();
    d.ADEP = adep->text();
    d.departureTime = QDateTime(QDateTime::currentDateTimeUtc().date(), departureTime->time(), Qt::UTC);
    d.dtADEP = departureTime->time().toString("hh:mm");
    d.ADES = ades->text();

    return d;
}

void DepWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void DepWidget::setFieldDefaultBackground()
{
    QString defaultStyle = "";

    acid->setStyleSheet(defaultStyle);
    adep->setStyleSheet(defaultStyle);
    ades->setStyleSheet(defaultStyle);
    departureTime->setStyleSheet(defaultStyle);
}

bool DepWidget::validateDep(bool showMessageBox)
{
    setFieldDefaultBackground();

    DepMessage msg(dep());

    AFTNMessageProcessor::AftnMessageErrors errorCodes;
    mProcessor->processMessage(msg.toAftn(), errorCodes);

    if (adep->text().isEmpty())
        errorCodes.append(AFTNErrorInfo(AFTNMessageProcessor::InvalidField13Format, QString()));
    if (ades->text().isEmpty())
        errorCodes.append(AFTNErrorInfo(AFTNMessageProcessor::InvalidField16Format, QString()));

    foreach (const AFTNErrorInfo &error, errorCodes)
    {
        switch (AFTNMessageProcessor::ProcessResult(error.first))
        {
        case AFTNMessageProcessor::InvalidField7Format:
#ifndef DUSHANBE_BUILD
            acid->setStyleSheet(invalidValue);
#endif
            if (showMessageBox)
            {
                acid->setFocus();
                QMessageBox::warning(this, tr("Error"), mProcessor->errorString(error));
                return false;
            }
            break;
        case AFTNMessageProcessor::InvalidField13Format:
#ifndef DUSHANBE_BUILD
            adep->setStyleSheet(invalidValue);
            departureTime->setStyleSheet(invalidValue);
#endif
            if (showMessageBox)
            {
                adep->setFocus();
                QMessageBox::warning(this, tr("Error"), mProcessor->errorString(error));
                return false;
            }
            break;
        case AFTNMessageProcessor::InvalidField16Format:
#ifndef DUSHANBE_BUILD
            ades->setStyleSheet(invalidValue);
#endif
            if (showMessageBox)
            {
                ades->setFocus();
                QMessageBox::warning(this, tr("Error"), mProcessor->errorString(error));
                return false;
            }
            break;
        default:
            break;
        }
    }
    return true;
}

void DepWidget::onEditTextChangeAdep(const QString &s)
{
    int pos = adep->cursorPosition();
    adep->setText(s.toUpper());
    adep->setCursorPosition(pos);
}

void DepWidget::onEditTextChangeAdes(const QString &s)
{
    int pos = ades->cursorPosition();
    ades->setText(s.toUpper());
    ades->setCursorPosition(pos);
}

void DepWidget::onTextChangedACID(const QString &s)
{
    int pos = acid->cursorPosition();
    acid->setText(s.toUpper());
    acid->setCursorPosition(pos);
}

bool DepWidget::validate()
{
    return validateDep();
}
