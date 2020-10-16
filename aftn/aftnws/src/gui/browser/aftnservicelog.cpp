#include "aftnservicelog.h"

#include "main.h"


#include <logger.h>
#include <signalappender.h>


#include <QtCore/QTimer>
#include <QtCore/QDateTime>

#include <QtGui/QFileDialog>

AFTNServiceLog::AFTNServiceLog(QWidget *parent) :
    QWidget(parent), Ui::AFTNServiceLogClass()

{
    setupUi(this);

    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearClicked()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));
}

AFTNServiceLog::~AFTNServiceLog()
{
}

void AFTNServiceLog::clearClicked()
{
    logEdit->clear();
}

void AFTNServiceLog::saveClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save log"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(logEdit->toPlainText().toUtf8());
        }
    }
}
void AFTNServiceLog::appendMessage(const QString& message)
{
    logEdit->appendHtml(message);
}

void AFTNServiceLog::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);

    switch (event->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

// --------------------------------------------------------------------

#include "patternlayout.h"
AFTNServiceLogHelper::AFTNServiceLogHelper(QObject* parent) :
    ObjectsManagerHelper(parent), mAction(0), manualMessagesCount(0)
{
    mAction = new QAction(icon(), name(), this);
    mAction->setObjectName(id());
    mAction->setToolTip(description());
    mAction->setStatusTip(description());

    mAction->setProperty("color", 0xFF0000);
    mAction->setText(tr("AFTN Service: unconnected"));
    mAction->setVisible(false);

    signalAppender = new Log4Qt::SignalAppender(Log4Qt::Logger::logger("aftnservice"));
    signalAppender->setName("signalAppender");
    Log4Qt::PatternLayout *patternLayout = new Log4Qt::PatternLayout("%d{locale:short} [%t] %p %c %x - %m%n", signalAppender);

    patternLayout->setName("patternLayout");
    patternLayout->activateOptions();
    signalAppender->setLayout(patternLayout);

    Log4Qt::Logger::logger("aftnservice")->addAppender(signalAppender);

    connect(signalAppender, SIGNAL(appended(QString)), this, SLOT(appended(QString)));
}

AFTNServiceLogHelper::~AFTNServiceLogHelper()
{
    // DELETED by main remove all appenders
    //delete signalAppender;
}

QString AFTNServiceLogHelper::id() const
{
    return AFTNServiceLog::staticMetaObject.className();
}

QString AFTNServiceLogHelper::path() const
{
    return tr("/Administration/AFTN Service log");
}

QString AFTNServiceLogHelper::name() const
{
    return tr("AFTN Service log");
}

QString AFTNServiceLogHelper::description() const
{
    return tr("AFTN Service log");
}

QWidget* AFTNServiceLogHelper::widget(QWidget* parent) const
{
    AFTNServiceLog* editor = new AFTNServiceLog(parent);

    connect(signalAppender, SIGNAL(appended(QString)), editor, SLOT(appendMessage(QString)));
    return editor;
}

void AFTNServiceLogHelper::appended(const QString &mesage)
{
    //if (mesage.contains("client: receiving"))
    //{
//        mAction->setProperty("color", 0x10AA10);
//        mAction->setText(tr("AFTN Service: connected"));
//    } else if (mesage.contains("client: unconnected"))
//    {
//        mAction->setProperty("color", 0xFF0000);
//        mAction->setText(tr("AFTN Service: unconnected"));
//    }
}

QAction* AFTNServiceLogHelper::action() const
{
    return mAction;
}

void AFTNServiceLogHelper::retranslateUi()
{
    mAction->setText(name());
    mAction->setIcon(icon());
    mAction->setToolTip(description());
    mAction->setStatusTip(description());
}

// --------------------------------------------------------------------
