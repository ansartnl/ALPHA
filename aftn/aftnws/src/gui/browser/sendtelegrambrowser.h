#ifndef SENDTELEGRAMBROWSER_H
#define SENDTELEGRAMBROWSER_H

#include "ui_sendtelegrambrowser.h"

#include "abstractbrowser.h"

class QSqlTableModel;

class SendTelegramBrowser : public AbstractBrowser, private Ui::SendTelegramBrowser
{
    Q_OBJECT
public:
    explicit SendTelegramBrowser(QWidget *parent = 0);

protected:
    virtual void changeEvent(QEvent *event);
    virtual void closeEvent(QCloseEvent *event);

protected slots:
  virtual void updateModel(); /*!< Update model with DB data according filters*/
  void onTableCurrentChanged(const QModelIndex&cur, const QModelIndex&); /*!< Call this slot when current row changet to update action buttons state*/
  void onFromTimeFilterChanged(const QDateTime& dtFrom); /*!< Called when From Date filter changed to update displaing data*/
  void onUntilTimeFilterChanged(const QDateTime& dtUntil); /*!< Called when Until Date filter changed to update displaing data*/
  void onTextFilterChanged(const QString& text); /*!< Called when text filter changed to update displaing data*/

protected:
    QSqlTableModel* model;
};

#include "objectsmanagerhelper.h"

class SendTelegramBrowserHelper: public ObjectsManagerHelper
{
  Q_OBJECT
  public:
    SendTelegramBrowserHelper(QObject *parent = 0);

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

#endif // SENDTELEGRAMBROWSER_H
