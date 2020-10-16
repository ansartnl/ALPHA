#include "CnlWidget.h"

#include "main.h"
#include "replies.h"

#include "aftnmessageprocessor.h"

#include <QMessageBox>

CnlWidget::CnlWidget(AFTNMessageProcessor *p, QWidget *parent) :
    QWidget(parent), mProcessor(p)
{
    setupUi(this);

    setVersion(ParseVersion::n_4444);

    invalidValue = "background-color: #FF637F;";

    connect(acid, SIGNAL(textChanged(QString)), SLOT(onTextChangedACID(QString)));
    connect(adep, SIGNAL(textChanged(QString)), SLOT(onEditTextChangeAdep(QString)));
    connect(ades, SIGNAL(textChanged(QString)), SLOT(onEditTextChangeAdes(QString)));

#ifndef DUSHANBE_BUILD
    connect(acid,SIGNAL(textChanged(QString)),SLOT(validateCnl()));
    connect(adep,SIGNAL(textChanged(QString)),SLOT(validateCnl()));
    connect(ades,SIGNAL(textChanged(QString)),SLOT(validateCnl()));

    validateCnl();
#endif
}

void CnlWidget::setReadOnly(bool r)
{
    acid->setEnabled(!r);
    adep->setEnabled(!r);
    ades->setEnabled(!r);
}

void CnlWidget::setCnl(const CnlStruct &c)
{
    setVersion(c.version);
    acid->setText(c.ACID);
    adep->setText(c.ADEP);
    ades->setText(c.ADES);

#ifndef DUSHANBE_BUILD
    validateCnl();
#endif
}

void CnlWidget::setVersion(int v)
{
    version = v;
    if (v == ParseVersion::n_TC95)
        telegramType->setText(Ats::CNL1);
    else
        telegramType->setText(Ats::CNL);
}

CnlStruct CnlWidget::cnl() const
{
    CnlStruct c;

    c.version = version;
    c.ACID = acid->text();
    c.ADEP = adep->text();
    c.ADES = ades->text();

    return c;
}

void CnlWidget::changeEvent(QEvent *e)
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

void CnlWidget::setFieldDefaultBackground()
{
    QString defaultStyle = "";

    acid->setStyleSheet(defaultStyle);
    adep->setStyleSheet(defaultStyle);
    ades->setStyleSheet(defaultStyle);
}

bool CnlWidget::validateCnl(bool showMessageBox)
{
    setFieldDefaultBackground();

    CnlMessage msg(cnl());

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
        default:
            break;
        }
    }
    return true;
}

void CnlWidget::onTextChangedACID(const QString &s)
{
    int pos = acid->cursorPosition();
    acid->setText(s.toUpper());
    acid->setCursorPosition(pos);
}

void CnlWidget::onEditTextChangeAdep(const QString &s)
{
    int pos = adep->cursorPosition();
    adep->setText(s.toUpper());
    adep->setCursorPosition(pos);
}

void CnlWidget::onEditTextChangeAdes(const QString &s)
{
    int pos = ades->cursorPosition();
    ades->setText(s.toUpper());
    ades->setCursorPosition(pos);
}

bool CnlWidget::validate()
{
    return validateCnl(true);
}
