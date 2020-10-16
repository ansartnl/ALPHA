#include "DlaWidget.h"

#include "replies.h"
#include "aftnmessageprocessor.h"
#include "main.h"

#include <QMessageBox>

DlaWidget::DlaWidget(AFTNMessageProcessor *p, QWidget *parent) :
    QWidget(parent), mProcessor(p)
{
    setupUi(this);

    setVersion(ParseVersion::n_4444);

    invalidValue = "background-color: #FF637F;";

    connect(acid, SIGNAL(textChanged(QString)), SLOT(onTextChangedACID(QString)));
    connect(adep, SIGNAL(textChanged(QString)), SLOT(onEditTextChangeAdep(QString)));
    connect(ades, SIGNAL(textChanged(QString)), SLOT(onEditTextChangeAdes(QString)));

#ifndef DUSHANBE_BUILD
    connect(acid, SIGNAL(textChanged(QString)), SLOT(validateDla()));
    connect(adep, SIGNAL(textChanged(QString)), SLOT(validateDla()));
    connect(ades, SIGNAL(textChanged(QString)), SLOT(validateDla()));
    connect(departureTime, SIGNAL(editingFinished()), SLOT(validateDla()));

    validateDla();
#endif
}

void DlaWidget::setReadOnly(bool r)
{
    acid->setEnabled(!r);
    adep->setEnabled(!r);
    departureTime->setEnabled(!r);
    ades->setEnabled(!r);
}

void DlaWidget::setDla(const DlaStruct &d)
{
    setVersion(d.version);
    acid->setText(d.ACID);
    adep->setText(d.ADEP);
    ades->setText(d.ADES);
    departureTime->setTime(d.departureTime.time());

#ifndef DUSHANBE_BUILD
    validateDla();
#endif
}

void DlaWidget::setVersion(int v)
{
    version = v;
    if (v == ParseVersion::n_TC95)
        telegramType->setText(Ats::DLA1);
    else
        telegramType->setText(Ats::DLA);
}

DlaStruct DlaWidget::dla() const
{
    DlaStruct d;

    d.version = version;
    d.ACID = acid->text();
    d.ADEP = adep->text();
    d.ADES = ades->text();
    d.departureTime = QDateTime(QDateTime::currentDateTimeUtc().date(), departureTime->time(), Qt::UTC);
    d.dtADEP = d.departureTime.toString("hh:mm");

    return d;
}

void DlaWidget::changeEvent(QEvent *e)
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

void DlaWidget::setFieldDefaultBackground()
{
    QString defaultStyle = "";

    acid->setStyleSheet(defaultStyle);
    adep->setStyleSheet(defaultStyle);
    ades->setStyleSheet(defaultStyle);
    departureTime->setStyleSheet(defaultStyle);
}

bool DlaWidget::validateDla(bool showMessageBox)
{
    setFieldDefaultBackground();

    DlaMessage msg(dla());

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

void DlaWidget::onEditTextChangeAdep(const QString &s)
{
    int pos = adep->cursorPosition();
    adep->setText(s.toUpper());
    adep->setCursorPosition(pos);
}

void DlaWidget::onEditTextChangeAdes(const QString &s)
{
    int pos = ades->cursorPosition();
    ades->setText(s.toUpper());
    ades->setCursorPosition(pos);
}

void DlaWidget::onTextChangedACID(const QString &s)
{
    int pos = acid->cursorPosition();
    acid->setText(s.toUpper());
    acid->setCursorPosition(pos);
}

bool DlaWidget::validate()
{
    return validateDla();
}
