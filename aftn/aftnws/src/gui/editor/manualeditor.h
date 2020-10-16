#ifndef MANUALEDITOR_H
#define MANUALEDITOR_H

#include "ui_manualeditor.h"
#include "abstractbrowser.h"

#include "fplviewer.h"
#include "fpl.h"

#include <QtCore/QString>
#include <QtCore/QMap>

#include <QtGui/QDialog>

class QAbstractButton;
class QSqlTableModel;
class QStringListModel;
class QDataWidgetMapper;
class OciTableModel;

class FplEditWidget;

#define TBL_MAN_AFTN         "man_aftn"
#define TBL_MAN_AFTN_ID      "id"
#define TBL_MAN_AFTN_CREATED "created"
#define TBL_MAN_AFTN_TEXT    "text"
#define TBL_MAN_AFTN_HEADER  "header"

/// Display raw messages which parse with error(s) and allows to change it in an effort parse again.
class ManualEditor: public AbstractBrowser, public Ui::ManualEditor
{
    Q_OBJECT
public:
    ManualEditor(QWidget* pParent = 0);
    virtual ~ManualEditor();

signals:
    void countChanged();

public slots:
    virtual void updateModel(); /*!< Update model with DB data according filter string*/

private slots:
    void on_applyMessage_clicked();
    void on_deleteMessage_clicked();
//    void onMessageChanged(const QVariantMap &message);
    void onMessageChanged();

    void on_next_clicked();
    void on_previous_clicked();

    void on_findButton_clicked();

    void currentMessageIndexChanged(int index);

    void onApplyFpl(const FPL &fpl);
    void onMaybeDushanbe(bool& bRes, const FPL& fpl);

private:
    bool tryParseAndInsertCurrentMessage();
    void deleteCurrentMessage();
    void messageToTAN(const QString& sHeader, const QString& sMessage, const QVariantMap& map, bool bError);
    void messageToTAN(const FPL& fpl);

    FplEditWidget* fplWidget;

    OciTableModel* messageModel;

    QDataWidgetMapper *mapper;

    QStringListModel* errorsModel;

    QVariantMap messageMap;

protected:
    virtual void changeEvent(QEvent *event);
    virtual void closeEvent(QCloseEvent *event);
};

// -----------------------------------------------------------
#include "objectsmanagerhelper.h"
class ManualEditorHelper: public ObjectsManagerHelper
{
    Q_OBJECT

    Q_INTERFACES(ObjectsManagerHelper)
public:
    ManualEditorHelper(QObject* parent = 0);

    virtual QString id() const;
    virtual QString path() const;
    virtual QString name() const;
    virtual QString description() const;
    virtual QIcon icon() const
    {
        return QIcon();
    }

    virtual QWidget* widget(QWidget* parent = 0) const;
protected:
    void timerEvent(QTimerEvent *event);

    QAction *action() const;
    virtual void retranslateUi();

signals:
    void changeCount();

protected slots:
    void updateState();
private:
    int timerId;
    QAction *mAction;
    int manualMessagesCount;
};

// -----------------------------------------------------------
class ManualTelegramEditorHelper: public ObjectsManagerHelper
{
    Q_OBJECT

    Q_INTERFACES(ObjectsManagerHelper)
public:
    ManualTelegramEditorHelper(QObject* parent = 0);

    virtual QString id() const;
    virtual QString path() const;
    virtual QString name() const;
    virtual QString description() const;
    virtual QIcon icon() const
    {
        return QIcon();
    }

    virtual QWidget* widget(QWidget* parent = 0) const;
protected:
    void timerEvent(QTimerEvent *event);

    QAction *action() const;
    virtual void retranslateUi();

signals:
    void changeCount();

protected slots:
    void updateState();
private:
    int timerId;
    QAction *mAction;
    int manualMessagesCount;
};
#endif // XMANUALEDITOR_H
