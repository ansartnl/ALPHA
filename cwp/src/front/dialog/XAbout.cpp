#include "XAbout.h"
#include <QGraphicsPixmapItem>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>

#include <QtDebug>

#include "HardwareProvider.h"
#include "License.h"

XAbout::XAbout(bool registered, QWidget* pParent)
    : QDialog(pParent)
{
    setupUi(this);
//    VerLabel->setText(VerLabel->text() + QCoreApplication::applicationVersion());
    //ImageLabel->setPixmap(QPixmap(QL1S(":/Images/PlaneImage")));

    QFile versionFile (qApp->applicationDirPath() + "/version");

     if (versionFile.open(QIODevice::ReadOnly |QIODevice::Text))
       {
         QByteArray verString = versionFile.readLine(50);
         VerLabel->setText(VerLabel->text() + verString);
         versionFile.close();
        }

    HardwareId hardwareId;
    if (!registered)
    {
        hardwareId = HardwareProvider().getHardwareId();
    }


    if (hardwareId.isValid())
    {
        mActivateButton = buttonBox->addButton(tr("Activate"), QDialogButtonBox::ActionRole);
        mActivateButton->setEnabled(false);

        regLineEdit->setText(hardwareId.toRegKey());

        connect(licLineEdit, SIGNAL(textChanged(QString)), SLOT(onLicTextChanged(QString)));
        connect(mActivateButton, SIGNAL(clicked(bool)), SLOT(onActivate()));
    }
    else
    {
        regLabel->hide();
        regLineEdit->hide();
        licLabel->hide();
        licLineEdit->hide();
    }
}

XAbout::~XAbout()
{

}

void XAbout::onActivate()
{
    HardwareId id = HardwareProvider().getHardwareId();
    QByteArray licKey = licLineEdit->text().remove('-').toLatin1();

    bool ok = License::verifyLicense(id.toRegKey(), licKey, SECRET_KEY);

    if (ok)
    {
        QFile hardwareIdFile(qApp->applicationDirPath() + "/key.hwid");
        hardwareIdFile.open(QIODevice::WriteOnly | QIODevice::Truncate);

        QFile licKeyFile(qApp->applicationDirPath() + "/key.lic");
        licKeyFile.open(QIODevice::WriteOnly | QIODevice::Truncate);

        QByteArray ba = id.toByteArray();
        if (hardwareIdFile.write(ba) == ba.size() && licKeyFile.write(licKey) == licKey.size())
        {
            accept();
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), tr("Unable to save keys"));
        }
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Invalid license key"));
    }
}

void XAbout::onLicTextChanged(const QString &text)
{
    mActivateButton->setEnabled(licLineEdit->maxLength() == text.length());
}
