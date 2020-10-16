#ifndef FPLONFUTUREBROWSER_H
#define FPLONFUTUREBROWSER_H

#include "ui_fplonfuturebrowser.h"
#include "abstractbrowser.h"
#include "fplviewer.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMap>

class QSqlTableModel;


namespace gui
{
  /// Using for modelling FPL on given date and displaying it
  class FplOnFutureBrowser : public AbstractBrowser, public Ui::FplOnFutureBrowser
  {
    Q_OBJECT
  public:
    FplOnFutureBrowser(QWidget* pParent);

  protected slots:
    virtual void updateModel(); /*!< Update model with DB data according filters*/
    void onDateFilterChanged(const QDate&); /*!< Called when date filter changed to update displaing data*/
    void onTextFilterChanged(const QString& text); /*!< Called when text filter changed to update displaing data*/
    void onDisplayItem(const QModelIndex& index); /*!< Shows detailed information about selected FPL */

  signals:
    void statusChanged(const QString& status); /*!< Emitted when status string changed */

  protected:
    void prepareTable(const QDate& date); /*!< Generate global temporary table TBL_FPL_ON_FUTURE in the DB with FPL on given date */

    FplViewer* viewer(); /*! Lazy initialization */
    virtual void changeEvent(QEvent *event);
    virtual void closeEvent(QCloseEvent *event);

  private:
    QScopedPointer<FplViewer> fplViewer;
    QSqlTableModel* model;
  };
}

#include "objectsmanagerhelper.h"
class FplOnFutureBrowserHelper : public ObjectsManagerHelper
{
  Q_OBJECT
public:
  FplOnFutureBrowserHelper(QObject* parent = 0);

  virtual QString id() const;
  virtual QString path() const;
  virtual QString name() const;
  virtual QString description() const;
  virtual QIcon icon() const {
    return QIcon();
  }

  virtual QWidget* widget(QWidget* parent = 0) const;
  virtual QAction* action() const;
  virtual void retranslateUi();
private:
  QAction* mAction;
};

#endif // FPLONFUTUREBROWSER_H
