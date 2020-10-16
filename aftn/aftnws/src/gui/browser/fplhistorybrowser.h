#ifndef FPLHISTORYBROWSER_H
#define FPLHISTORYBROWSER_H

#include "fplhistorybrowser.h"
#include "ui_fplhistorybrowser.h"
#include "abstractbrowser.h"
#include "fplviewer.h"

#include <QtSql/QSqlRelationalTableModel>

class QSplitter;
class QTextBrowser;
class QtSqlTableView;

    /// Using for displaying fpls history from TBL_HIST_FPL table
  class FplHistoryBrowser : public AbstractBrowser, public Ui::FplHistoryBrowser
  {
    Q_OBJECT
  public:
    FplHistoryBrowser(QWidget* pParent);

  protected slots:
    virtual void updateModel(); /*!< Update model with DB data according filters*/
    void onTableCurrentChanged(const QModelIndex&, const QModelIndex&); /*!< Call this slot when current row changet to update action buttons state*/
    void onDateFilterChanged(const QDate&); /*!< Called when date filter changed to update displaing data*/
    void onSecondTableCurrentChanged(const QModelIndex&, const QModelIndex&);
    void onTextFilterChanged(const QString& text); /*!< Call this when text filter changed to update displaing data*/
    void onDisplayItem(const QModelIndex& index); /*!< Shows detailed information about selected FPL */

  signals:
    void statusChanged(const QString& status); /*!< Emitted when status string changed*/

  protected:

    QSqlRelationalTableModel* model;
    QSqlRelationalTableModel* secondModel;

    FplViewer* viewer(); /*! Lazy initialization */
    virtual void changeEvent(QEvent *event);

    virtual void closeEvent(QCloseEvent *event);

  private:
    QScopedPointer<FplViewer>  fplViewer;
  };

#include "objectsmanagerhelper.h"
class FplHistoryBrowserHelper : public ObjectsManagerHelper
{
  Q_OBJECT
public:
  FplHistoryBrowserHelper(QObject *parent = 0);

  virtual QString id() const;
  virtual QString path() const;
  virtual QString name() const;
  virtual QString description() const;
  virtual QIcon icon() const {return QIcon();}

  virtual QWidget *widget(QWidget* parent = 0) const;
  virtual QAction* action() const ;
  virtual void retranslateUi();
private:
  QAction* mAction;
};
#endif // FPLHISTORYBROWSER_H
