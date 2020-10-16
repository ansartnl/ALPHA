#include "ArrWidget.h"

#include "main.h"
#include "replies.h"

#include "aftnmessageprocessor.h"

#include <QMessageBox>

ArrWidget::ArrWidget(AFTNMessageProcessor *p, QWidget *parent) :
    QWidget(parent), mProcessor(p)
{
    setupUi(this);

    setVersion(ParseVersion::n_4444);

    invalidValue = "background-color: #FF637F;";

    connect(acid, SIGNAL(textChanged(QString)), SLOT(onTextChangedACID(QString)));
    connect(adep, SIGNAL(textChanged(QString)), SLOT(onEditTextChangeAdep(QString)));
    connect(ades, SIGNAL(textChanged(QString)), SLOT(onEditTextChangeAdes(QString)));
    connect(arrivalAirport, SIGNAL(textChanged(QString))
            , SLOT(onEditTextChangeArrivalAirport(QString)));
    connect(arrivalAirportName, SIGNAL(textChanged(QString))
            , SLOT(onTaxtChangedArrivalAirportName(QString)));

#ifndef DUSHANBE_BUILD
    connect(acid, SIGNAL(textChanged(QString)), SLOT(validateArr()));
    connect(adep, SIGNAL(textChanged(QString)), SLOT(validateArr()));
    connect(ades, SIGNAL(textChanged(QString)), SLOT(validateArr()));
    connect(arrivalAirport, SIGNAL(textChanged(QString)), SLOT(validateArr()));
    connect(arrivalTime, SIGNAL(timeChanged(QTime)), SLOT(validateArr()));
    connect(arrivalAirportName, SIGNAL(textChanged(QString)), SLOT(validateArr()));

    validateArr();
#endif
}

void ArrWidget::setReadOnly(bool r)
{
    acid->setEnabled(!r);
    adep->setEnabled(!r);
    ades->setEnabled(!r);
    arrivalAirport->setEnabled(!r);
    arrivalTime->setEnabled(!r);
    arrivalAirportName->setEnabled(!r);
}

void ArrWidget::setArr(const ArrStruct &a)
{
    setVersion(a.version);

    acid->setText(a.ACID);
    adep->setText(a.ADEP);
    ades->setText(a.ADES);
    arrivalAirport->setText(a.arrivalAirport);
    arrivalTime->setTime(a.arrivalTime.time());
//    arrivalTime->setTime(QTime::currentTime());
    arrivalAirportName->setText(a.arrivalAirportName);

#ifndef DUSHANBE_BUILD
    validateArr();
#endif
}

void ArrWidget::setVersion(int v)
{
    version = v;
    if (v == ParseVersion::n_TC95)
        telegramType->setText(Ats::ARR1);
    else
        telegramType->setText(Ats::ARR);
}

ArrStruct ArrWidget::arr() const
{
    ArrStruct a;

    a.version = version;
    a.ACID = acid->text();
    a.ADEP = adep->text();
    a.ADES = ades->text();
    a.arrivalAirport = arrivalAirport->text();
    a.arrivalTime = QDateTime(QDateTime::currentDateTimeUtc().date(), arrivalTime->time(), Qt::UTC);
    a.arrivalAirportName = arrivalAirportName->text();

    return a;
}

void ArrWidget::changeEvent(QEvent *e)
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
void ArrWidget::setFieldDefaultBackground()
{
    QString defaultStyle = "";

    acid->setStyleSheet(defaultStyle);
    adep->setStyleSheet(defaultStyle);
    ades->setStyleSheet(defaultStyle);
    arrivalAirport->setStyleSheet(defaultStyle);
    arrivalTime->setStyleSheet(defaultStyle);
    arrivalAirportName->setStyleSheet(defaultStyle);
}

bool ArrWidget::validateArr(bool showMessageBox)
{
    setFieldDefaultBackground();

    ArrMessage msg(arr());

    AFTNMessageProcessor::AftnMessageErrors errorCodes;
    mProcessor->processMessage(msg.toAftn(), errorCodes);

    if (adep->text().isEmpty())
        errorCodes.append(AFTNErrorInfo(AFTNMessageProcessor::InvalidField13Format, QString()));
    if (arrivalAirport->text().isEmpty())
        errorCodes.append(AFTNErrorInfo(AFTNMessageProcessor::InvalidField17Format, QString()));

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
        case AFTNMessageProcessor::InvalidField17Format:
#ifndef DUSHANBE_BUILD
            arrivalAirport->setStyleSheet(invalidValue);
            arrivalTime->setStyleSheet(invalidValue);
            arrivalAirportName->setStyleSheet(invalidValue);
#endif
            if (showMessageBox)
            {
                arrivalAirport->setFocus();
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

void ArrWidget::onEditTextChangeAdep(const QString &s)
{
    int pos = adep->cursorPosition();
    adep->setText(s.toUpper());
    adep->setCursorPosition(pos);
}

void ArrWidget::onEditTextChangeAdes(const QString &s)
{
    int pos = ades->cursorPosition();
    ades->setText(s.toUpper());
    ades->setCursorPosition(pos);
}

void ArrWidget::onEditTextChangeArrivalAirport(const QString &s)
{
    int pos = arrivalAirport->cursorPosition();
    arrivalAirport->setText(s.toUpper());
    arrivalAirport->setCursorPosition(pos);
}

void ArrWidget::onTextChangedACID(const QString &s)
{
    int pos = acid->cursorPosition();
    acid->setText(s.toUpper());
    acid->setCursorPosition(pos);
}

void ArrWidget::onTaxtChangedArrivalAirportName(const QString &s)
{
    int pos = arrivalAirportName->cursorPosition();
    arrivalAirportName->setText(s.toUpper());
    arrivalAirportName->setCursorPosition(pos);
}

bool ArrWidget::validate()
{
    return validateArr(true);
}
