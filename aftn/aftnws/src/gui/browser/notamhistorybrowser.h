#ifndef NOTAMHISTORYBROWSER_H
#define NOTAMHISTORYBROWSER_H

#include "ui_notamhistorybrowser.h"
#include "abstractbrowser.h"
#include "notamviewer.h"

class QSqlTableModel;

/// Use for displaying history of Notams
class NotamHistoryBrowser: public AbstractBrowser,
  public Ui::NotamHistoryBrowser
{
  Q_OBJECT
  public:
    NotamHistoryBrowser(QWidget* pParent);

  protected slots:
    virtual void updateModel(); /*!< Update model with DB data according filters*/
    void onTableCurrentChanged(const QModelIndex&, const QModelIndex&); /*!< Call this slot when current row changet to update action buttons state*/
    void onFromTimeFilterChanged(const QDateTime& dtFrom); /*!< Called when date filter changed to update displaing data*/
    void onUntilTimeFilterChanged(const QDateTime& dtUntil); /*!< Called when date filter changed to update displaing data*/
    void onTextFilterChanged(const QString& text); /*!< Call this when text filter changed to update displaing data*/
    void onDisplayItem(const QModelIndex& index); /*!< Shows detailed information about selected Notam */

  signals:
    void statusChanged(const QString& status); /*!< Emitted when status string changed*/

  protected:
    NotamViewer* viewer(); /*! Lazy initialization*/
    virtual void changeEvent(QEvent *event);

    virtual void closeEvent(QCloseEvent *event);

  protected:
    QScopedPointer<NotamViewer> notamViewer;

    QSqlTableModel* model;
    QSqlTableModel* secondModel;
    QString prevSequenceId;
};

#include "objectsmanagerhelper.h"
class NotamHistoryBrowserHelper: public ObjectsManagerHelper
{
  Q_OBJECT
  public:
    NotamHistoryBrowserHelper(QObject *parent = 0);

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

#endif /*NOTAMHISTORYBROWSER_H*/
