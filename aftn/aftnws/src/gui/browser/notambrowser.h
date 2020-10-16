#ifndef NOTAMBROWSER_H
#define NOTAMBROWSER_H

#include "ui_notambrowser.h"
#include "abstractbrowser.h"
#include "notamviewer.h"

#include <QtCore/QModelIndex>

class QSqlTableModel;
class QSqlQueryModel;

/// Use for displaying active notams
class NotamBrowser: public AbstractBrowser, public Ui::NotamBrowser
{
  Q_OBJECT
  public:
    NotamBrowser(QWidget* pParent);

  protected slots:
    virtual void updateModel(); /*!< Update model with DB data according filters*/
    void onTableCurrentChanged(const QModelIndex&, const QModelIndex&); /*!< Call this slot when current row changet to update action buttons state*/
    void onTextFilterChanged(const QString& text); /*!< Call this when text filter changed to update displaing data*/
    void onDisplayItem(const QModelIndex& index); /*!< Shows detailed information about selected Notam */

    void onFromTimeFilterChanged(const QDateTime& dtFrom); /*!< Called when date filter changed to update displaing data*/
    void onUntilTimeFilterChanged(const QDateTime& dtUntil); /*!< Called when date filter changed to update displaing data*/

  signals:
    void statusChanged(const QString& status); /*!< Emitted when status string changed. Depricate?*/

  protected:
    NotamViewer* viewer(); /*!< Lazy initialization */
    virtual void changeEvent(QEvent *event);
    virtual void closeEvent(QCloseEvent *event);
  protected:
    QScopedPointer<NotamViewer> notamViewer;
    QSqlTableModel* model;

    QSqlQueryModel* natureFilterModel;
    QSqlQueryModel* firFilterModel;

};

#include "objectsmanagerhelper.h"
class NotamBrowserHelper: public ObjectsManagerHelper
{
  Q_OBJECT
  public:
    NotamBrowserHelper(QObject *parent = 0);

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

#endif /*NOTAMBROWSER_H*/
