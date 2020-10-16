#ifndef AFTNSERVICELOG_H
#define AFTNSERVICELOG_H

#include <QtGui/QWidget>

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>

#include "ui_aftnservicelog.h"

class AFTNServiceLog: public QWidget, public Ui::AFTNServiceLogClass
{
    Q_OBJECT

public:
    AFTNServiceLog(QWidget *parent = 0);
    ~AFTNServiceLog();

public slots:
    void appendMessage(const QString& message);

protected slots:
    void clearClicked();
    void saveClicked();

protected:
    virtual void changeEvent(QEvent *event);
};

// -----------------------------------------------------------
#include "objectsmanagerhelper.h"

#include "signalappender.h"
class AFTNServiceLogHelper: public ObjectsManagerHelper
{
    Q_OBJECT

    Q_INTERFACES( ObjectsManagerHelper)
public:
    AFTNServiceLogHelper(QObject* parent = 0);
    virtual ~AFTNServiceLogHelper();

    virtual QString id() const;
    virtual QString path() const;
    virtual QString name() const;
    virtual QString description() const;
    virtual QIcon icon() const
    {
        return QIcon();
    }

    virtual QWidget* widget(QWidget* parent = 0) const;

protected slots:
    void appended(const QString& mesage);

protected:
    QAction *action() const;
    virtual void retranslateUi();


signals:
    void message(const QString);

private:
    QAction *mAction;
    int manualMessagesCount;

    Log4Qt::SignalAppender *signalAppender;
};
// -----------------------------------------------------------

#endif // AFTNSERVICELOG_H
