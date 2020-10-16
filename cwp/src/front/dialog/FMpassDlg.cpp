#include "FMpassDlg.h"
#include "main.h"
#include "Master.h"

#include <QRadioButton>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QSettings>
#include <QInputDialog>
#include <QCryptographicHash>

FMpassDlg::FMpassDlg(QWidget *pParent, Qt::WindowFlags f)
    : QDialog(pParent, f), mButGroup(0)
{
    QVBoxLayout* vlayout = new QVBoxLayout(this);
    mButGroup = new QButtonGroup(this);
    QRadioButton* but = new QRadioButton(tr("F"), this);
    mButGroup->addButton(but, 1);
    vlayout->addWidget(but);
    but = new QRadioButton(tr("M"), this);
    mButGroup->addButton(but, 2);
    vlayout->addWidget(but);
    but = new QRadioButton(tr("FM"), this);
    mButGroup->addButton(but, 3);
    vlayout->addWidget(but);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    vlayout->addWidget(buttonBox);
    setLayout(vlayout);

    Translate();
    Init();
}

FMpassDlg::~FMpassDlg()
{

}

void FMpassDlg::Translate()
{
    setWindowTitle("Settings");
}

void FMpassDlg::accept()
{
    int id = mButGroup->checkedId();
    if(id >= 1 && id <= 3)
    {
        bool bOk(false);
        QString sPass = QInputDialog::getText(this, tr("Enter the password"), tr("Password"), QLineEdit::Password, "", &bOk);
        if(bOk && !sPass.isEmpty())
        {
            QString sPassOld = GetPass();
            if(sPassOld.isEmpty())
            {
                bOk = false;
                sPassOld = QInputDialog::getText(this, tr("Enter the password confirmation"), tr("Password"), QLineEdit::Password, "", &bOk);
                if(!bOk)
                    return;
                sPassOld = CryptPass(sPassOld);
            }
            if(CryptPass(sPass) == sPassOld)
            {
                SetFMval(id);
                SetPass(sPass);
                QDialog::accept();
            }
        }
    }
}

void FMpassDlg::getFMvalues(QString& val)
{
    int id = GetFMval();
    if(id == 1)
        val = "F";
    else if(id == 2)
        val = "M";
    else if(id == 3)
        val = "FM";
}

int FMpassDlg::GetFMval()
{
    int iRes = 0;
    iRes = globalSettings()->value("XMasterWindow/Settings/FMvalInt").toInt();
    return iRes;
}

void FMpassDlg::SetFMval(int val)
{
    globalSettings()->setValue("XMasterWindow/Settings/FMvalInt", val);
}

QString FMpassDlg::GetPass()
{
    QString sRes;
    sRes = globalSettings()->value("XMasterWindow/Settings/FMvalPas").toString();
    return sRes;
}

void FMpassDlg::SetPass(QString val)
{
    if(!val.isEmpty())
    {
        QString sRes = CryptPass(val);
        globalSettings()->setValue("XMasterWindow/Settings/FMvalPas", sRes);
    }
}

QString FMpassDlg::CryptPass(QString pass)
{
//    QByteArray data = pass.toUtf8();
//    QString sRes = data.toHex();
    QString sRes = QCryptographicHash::hash(pass.toUtf8().data(), QCryptographicHash::Md5).toHex();
    return sRes;
}

void FMpassDlg::Init()
{
    int id = GetFMval();
    if(!id)
    {
        QRegExp rexp("([F,M]{1,1})([F,M]{0,1})");
        if(rexp.exactMatch(master()->getSI_FL()))
        {
            if(!rexp.cap(1).isEmpty())
            {
                if("F" == rexp.cap(1))
                    id = 1;
                else if("M" == rexp.cap(1))
                    id = 2;

                if(!rexp.cap(2).isEmpty())
                    id = 3;
            }
        }
    }
    if(mButGroup)
    {
        QAbstractButton* but = mButGroup->button(id);
        if(but)
            but->setChecked(true);
    }
}
