#ifndef RECEIVEDTELEGRAMBROWSER_H
#define RECEIVEDTELEGRAMBROWSER_H

#include "ui_receivedtelegrambrowser.h"
#include "abstractbrowser.h"
#include "AFTNDao.h"

#include <QtCore/QModelIndex>

class QSqlTableModel;

/// Using for displaying received messages
class ReceivedTelegramBrowser: public AbstractBrowser,
  public Ui::ReceivedTelegramBrowser
{
  Q_OBJECT
  public:
    ReceivedTelegramBrowser(QWidget* pParent);

    const QStringList dataList() const;
    PrintDataDialog *printDialog();

  protected slots:
    virtual void updateModel(); /*!< Update model with DB data according filters*/
    void onTableCurrentChanged(const QModelIndex&, const QModelIndex&); /*!< Call this slot when current row changet to update action buttons state*/
    void onFromTimeFilterChanged(const QDateTime& dtFrom); /*!< Called when From Date filter changed to update displaing data*/
    void onUntilTimeFilterChanged(const QDateTime& dtUntil); /*!< Called when Until Date filter changed to update displaing data*/
    void onTextFilterChanged(const QString& text); /*!< Called when text filter changed to update displaing data*/

    void setStatus(const QString& status); /*!< Set status strig for displayin on the status bar*/

  signals:
    void statusChanged(const QString& status); /*!< Emitted when status string changed */

  protected:
    QSqlTableModel* model;

    QString browserStatus;
    virtual void changeEvent(QEvent *event);
    virtual void closeEvent(QCloseEvent *event);
};

#include "objectsmanagerhelper.h"

class ReceivedTelegramBrowserHelper: public ObjectsManagerHelper
{
  Q_OBJECT
  public:
    ReceivedTelegramBrowserHelper(QObject *parent = 0);

    virtual QString id() const;
    virtual QString path() const;
    virtual QString name() const;
    virtual QString description() const;

    virtual QIcon icon() const
    {
      return QIcon();
    }

    virtual QWidget *widget(QWidget* parent = 0) const;
    virtual QAction* action() const;
    virtual void retranslateUi();
  private:
    QAction* mAction;
};

#include "printdatadialog.h"

class ReceivedTelegramPrintDialog : public PrintDataDialog
{
    Q_OBJECT
public:
    ReceivedTelegramPrintDialog(QWidget *parent = 0);

    const QString documentName() const;
    const QString securityLevel() const;
};

#endif // RECEIVEDTELEGRAMBROWSER_H
