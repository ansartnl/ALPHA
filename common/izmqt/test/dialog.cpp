#include "dialog.h"
#include "ui_dialog.h"

#include <QHostAddress>
#include <QTcpSocket>

#include "pubsub/PublishQueue.h"
#include "pubsub/SubscribeQueue.h"

#include "reqrep/ReplyQueue.h"
#include "reqrep/RequestQueue.h"

#include "pushpull/PushQueue.h"
#include "pushpull/PullQueue.h"

#include "asyncreqrep/RouterServer.h"
#include "asyncreqrep/DealerClient.h"

#include "autoconnector/autoconnector.h"
#include "autoconnector/asyncconnector.h"

#include "Log.h"
#include "TextEditAppender.h"
#include "logmanager.h"
#include "patternlayout.h"
#include "rollingfileappender.h"
#include "consoleappender.h"

static QObject *g_server = 0;
static QObject *g_client = 0;

static izmqt::ConnectionPoint *g_connectionPoint = 0;
static izmqt::AutoConnector *g_autoConnector = 0;

static izmqt::RouterPonger *g_routerPonger = 0;
static izmqt::DealerPinger *g_dealerPinger = 0;


// Log to file/console initialization.
void initLog4Qt(QPlainTextEdit *plainTextEditLog)
{
    // INITIALISATION OF LOG4QT
    using namespace Log4Qt;
    LogManager::setHandleQtMessages(true);

    // Format log message "date time [thread] logger level - message \newline"
    PatternLayout *patternLayout = new PatternLayout("%d{locale:short} [%t] %p %c - %m%n", 0);
    patternLayout->setName("patternLayout");
    patternLayout->activateOptions();

    Level logLevel = Level::fromString("ALL");
    // Define log level for all loggers.
    LogManager::rootLogger()->setLevel( logLevel );
    qRootLogger()->setLevel( logLevel );

    // Add console appender ...

    ConsoleAppender *consoleAppender = new ConsoleAppender(patternLayout,
                                                           ConsoleAppender::STDERR_TARGET,
                                                           0);
    consoleAppender->activateOptions();
    LogManager::rootLogger()->addAppender(consoleAppender);

    // Create 'text edit' appender.
    {
        using namespace Log4Qt;
        TextEditAppender *textEditAppender = new TextEditAppender(plainTextEditLog, patternLayout);
        textEditAppender->activateOptions();
        LogManager::rootLogger()->addAppender(textEditAppender);
    }
}


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    connect(ui->radioButtonPubSub, SIGNAL(toggled(bool)), SLOT(onPatternToggled(bool)));
    connect(ui->radioButtonReqRep, SIGNAL(toggled(bool)), SLOT(onPatternToggled(bool)));
    connect(ui->radioButtonPushPull, SIGNAL(toggled(bool)), SLOT(onPatternToggled(bool)));
    connect(ui->radioButtonDealerRouter, SIGNAL(toggled(bool)), SLOT(onPatternToggled(bool)));

    initLog4Qt(ui->plainTextEditLog);
    qDebug() << QString(" SESSION STARTED !!! ");
}

Dialog::~Dialog()
{
    on_pushButtonStop_clicked();
    delete ui;
}

void Dialog::onPatternToggled(bool)
{
    if ( ui->radioButtonDealerRouter->isChecked() )
        ui->labelTopic->setText( tr("Identity") );
    else
        ui->labelTopic->setText( tr("Topic") );

    bool isTopicEnabled = ui->radioButtonPubSub->isChecked() || ui->radioButtonDealerRouter->isChecked();
    ui->labelTopic->setEnabled( isTopicEnabled );
    ui->lineEditTopic->setEnabled( isTopicEnabled );
}

void Dialog::on_pushButtonStart_clicked()
{
    QString address = QString("tcp://%1:%2").arg(ui->lineEdit->text()).arg(ui->spinBox->value());

    if ( g_server || g_client
         || g_connectionPoint || g_autoConnector )
    {
        qDebug() << tr("Previous process was not stopped yet!");
        return;
    }

    // Auto connection test ...

    if ( ui->checkBoxAutoConnector->isChecked() )
    {
        if ( ui->radioButtonServer->isChecked() )
        {
            if ( !g_connectionPoint )
            {
                izmqt::ConnectionPoint *connectionPoint = new izmqt::ConnectionPoint(this);
                if ( connectionPoint->start( address ) )
                {
                    qDebug() << tr("Connection point bound to socket: '%1:%2'")
                                .arg(ui->lineEdit->text()).arg(ui->spinBox->value());
                }
                g_connectionPoint = connectionPoint;
            }
        }
        else if ( ui->radioButtonClient->isChecked() )
        {
            if ( !g_autoConnector )
            {
                izmqt::AutoConnector *autoConnector = new izmqt::AutoConnector(this);
                connect(autoConnector, SIGNAL(connected()), SLOT(onAutoConnected()));
                connect(autoConnector, SIGNAL(disconnected()), SLOT(onAutoDisconnected()));
                if ( autoConnector->start(address) )
                {
                    qDebug() << tr("Auto connector connects to socket: '%1:%2'")
                                .arg(ui->lineEdit->text()).arg(ui->spinBox->value());
                }
                g_autoConnector = autoConnector;
            }
        }

        return;
    }

    // Zeromq patterns test ...

    if ( ui->radioButtonPubSub->isChecked() )
    {
        if ( ui->radioButtonServer->isChecked() )
        {
            izmqt::PublishQueue *server = new izmqt::PublishQueue( address, ui->lineEditTopic->text(), izmqt::SERVER, this );
            g_server = server;
        }
        else if ( ui->radioButtonClient->isChecked() )
        {
            izmqt::SubscribeQueue *client = new izmqt::SubscribeQueue( address, QStringList(ui->lineEditTopic->text()), izmqt::CLIENT, this );
            g_client = client;
        }
    }
    else if ( ui->radioButtonReqRep->isChecked() )
    {
        if ( ui->radioButtonServer->isChecked() )
        {
            izmqt::ReplyQueue *server = new izmqt::ReplyQueue( address, izmqt::SERVER, this );
            g_server = server;
        }
        else if ( ui->radioButtonClient->isChecked() )
        {
            izmqt::RequestQueue *client = new izmqt::RequestQueue( address, izmqt::CLIENT, this );
            g_client = client;
        }
    }
    else if ( ui->radioButtonPushPull->isChecked() )
    {
        if ( ui->radioButtonServer->isChecked() )
        {
            izmqt::PullQueue *server = new izmqt::PullQueue( address, izmqt::SERVER, this );
            g_server = server;
        }
        else if ( ui->radioButtonClient->isChecked() )
        {
            izmqt::PushQueue *client = new izmqt::PushQueue( address, izmqt::CLIENT, this );
            g_client = client;
        }
    }
    else if ( ui->radioButtonDealerRouter->isChecked() )
    {
        if ( ui->radioButtonServer->isChecked() )
        {
            izmqt::RouterServer *server = new izmqt::RouterServer( address, this );
            g_server = server;

            g_routerPonger = new izmqt::RouterPonger(server, this);
        }
        else if ( ui->radioButtonClient->isChecked() )
        {
            izmqt::DealerClient *client = new izmqt::DealerClient( address, ui->lineEditTopic->text(), this );
            g_client = client;

            g_dealerPinger = new izmqt::DealerPinger(client, this);
            connect(g_dealerPinger, SIGNAL(connected()), SLOT(onAutoConnected()));
            connect(g_dealerPinger, SIGNAL(disconnected()), SLOT(onAutoDisconnected()));
        }
    }

    if ( g_server )
    {
        if ( qobject_cast<izmqt::ReplyQueue *>(g_server) )
        {
            connect(g_server, SIGNAL(requestReceived(QVariant)), SLOT(onMessageReceived(QVariant)));

            if ( 0 == qobject_cast<izmqt::ReplyQueue *>(g_server)->start() )
                qDebug() << tr("Queue bound to socket: '%1:%2'")
                            .arg(ui->lineEdit->text()).arg(ui->spinBox->value());
        }
        else if ( qobject_cast<izmqt::PublishQueue *>(g_server) )
        {
            if ( 0 == qobject_cast<izmqt::PublishQueue *>(g_server)->start() )
                qDebug() << tr("Queue '%1' bound to socket: '%2:%3'")
                            .arg(ui->lineEditTopic->text())
                            .arg(ui->lineEdit->text()).arg(ui->spinBox->value());
        }
        else if ( qobject_cast<izmqt::PullQueue *>(g_server) )
        {
            connect(g_server, SIGNAL(readyRead(QVariant)), SLOT(onMessageReceived(QVariant)));

            if ( 0 == qobject_cast<izmqt::PullQueue *>(g_server)->start() )
                qDebug() << tr("Queue bound to socket: '%1:%2'")
                            .arg(ui->lineEdit->text()).arg(ui->spinBox->value());
        }
        else if ( qobject_cast<izmqt::RouterServer *>(g_server) )
        {
            connect(g_server, SIGNAL(requestReceived(QVariant, QString)), SLOT(onMessageReceived(QVariant, QString)));

            if ( 0 == qobject_cast<izmqt::RouterServer *>(g_server)->start() )
                qDebug() << tr("Router queue bound to socket: '%1:%2'")
                            .arg(ui->lineEdit->text()).arg(ui->spinBox->value());
        }
    }

    if ( qobject_cast<izmqt::SubscribeQueue *>(g_client) )
    {
        connect(g_client, SIGNAL(readyRead(QString,QVariant)), SLOT(onMessageReceived(QString,QVariant)));

        if ( 0 == qobject_cast<izmqt::SubscribeQueue *>(g_client)->start() )
            qDebug() << tr("Connect to the queue '%1' at socket: '%2:%3'")
                        .arg(ui->lineEditTopic->text())
                        .arg(ui->lineEdit->text()).arg(ui->spinBox->value());
    }
    else if ( qobject_cast<izmqt::RequestQueue *>(g_client) )
    {
        connect(g_client, SIGNAL(replyReceived(QVariant)), SLOT(onMessageReceived(QVariant)));

        if ( 0 == qobject_cast<izmqt::RequestQueue *>(g_client)->start() )
            qDebug() << tr("Connect to the queue at socket: '%1:%2'")
                        .arg(ui->lineEdit->text()).arg(ui->spinBox->value());
    }
    else if ( qobject_cast<izmqt::PushQueue *>(g_client) )
    {
        if ( 0 == qobject_cast<izmqt::PushQueue *>(g_client)->start() )
            qDebug() << tr("Connect to the queue at socket: '%1:%2'")
                        .arg(ui->lineEdit->text()).arg(ui->spinBox->value());
    }
    else if ( qobject_cast<izmqt::DealerClient *>(g_client) )
    {
        connect(g_client, SIGNAL(replyReceived(QVariant)), SLOT(onMessageReceived(QVariant)));

        if ( 0 == qobject_cast<izmqt::DealerClient *>(g_client)->start() )
        {
            qDebug() << tr("Connect to the dealer queue at socket: '%1:%2'")
                        .arg(ui->lineEdit->text()).arg(ui->spinBox->value());

            if ( g_dealerPinger )
                g_dealerPinger->start();
        }
    }
}

void Dialog::on_pushButtonStop_clicked()
{
    if ( g_connectionPoint )
    {
        g_connectionPoint->stop();
        g_connectionPoint->deleteLater();
        g_connectionPoint = 0;
    }

    if ( g_autoConnector )
    {
        g_autoConnector->stop();
        g_autoConnector->deleteLater();
        g_autoConnector = 0;
    }

    if ( g_routerPonger )
    {
        g_routerPonger->deleteLater();
        g_routerPonger = 0;
    }
    if ( g_dealerPinger )
    {
        g_dealerPinger->stop();
        g_dealerPinger->deleteLater();
        g_dealerPinger = 0;
    }

    if ( g_server )
    {
        QMetaObject::invokeMethod(g_server, "stop");

        g_server->deleteLater();
        g_server = 0;
    }
    if ( g_client )
    {
        QMetaObject::invokeMethod(g_client, "stop");

        g_client->deleteLater();
        g_client = 0;
    }

    qDebug() << tr("Stop pressed!");
}

void Dialog::on_pushButtonSend_clicked()
{
    QString msg = ui->plainTextEditMessage->toPlainText();

    if ( qobject_cast<izmqt::RequestQueue *>(g_client) )
        qobject_cast<izmqt::RequestQueue *>(g_client)->request( msg );

    else if ( qobject_cast<izmqt::PushQueue *>(g_client) )
        qobject_cast<izmqt::PushQueue *>(g_client)->push( msg );

    else if ( qobject_cast<izmqt::DealerClient *>(g_client) )
        qobject_cast<izmqt::DealerClient *>(g_client)->request( msg );

    else if ( qobject_cast<izmqt::PublishQueue *>(g_server) )
        qobject_cast<izmqt::PublishQueue *>(g_server)->publish( msg );

    else if ( qobject_cast<izmqt::ReplyQueue *>(g_server) )
        qobject_cast<izmqt::ReplyQueue *>(g_server)->reply( msg );

    else if ( qobject_cast<izmqt::RouterServer *>(g_server) )
        qobject_cast<izmqt::RouterServer *>(g_server)->reply( msg, ui->lineEditTopic->text() );

    else
        qDebug() << tr("This client does not send messages!");
}

void Dialog::onConnected()
{
    qDebug() << tr("Connected to queue at socket: '%1:%2'")
                .arg(ui->lineEdit->text()).arg(ui->spinBox->value());
}

void Dialog::onAutoConnected()
{
    qDebug() << tr("Connected to socket: '%1:%2'")
                .arg(ui->lineEdit->text()).arg(ui->spinBox->value());
}

void Dialog::onAutoDisconnected()
{
    qDebug() << tr("Connection lost for socket: '%1:%2'")
                .arg(ui->lineEdit->text()).arg(ui->spinBox->value());

    if ( g_dealerPinger )
    {
        // Must to reset the dealer client.
        g_client->deleteLater();
        g_client = g_dealerPinger->dealerClient();

        connect(g_client, SIGNAL(replyReceived(QVariant)), SLOT(onMessageReceived(QVariant)));
    }
}

void Dialog::onMessageReceived(QVariant v)
{
    if ( v.canConvert<QString>() )
        qDebug() << tr("Message recv: ") << v.toString();
}

void Dialog::onMessageReceived(QVariant v, QString client)
{
    if ( v.canConvert<QString>() )
    {
        qDebug() << tr("Message recv '%1': ").arg(client) << v.toString();
    }
}

void Dialog::onMessageReceived(QString topic, QVariant v)
{
    if ( v.canConvert<QString>() )
    {
        qDebug() << tr("Message recv #'%1': ").arg(topic) << v.toString();
    }
}
