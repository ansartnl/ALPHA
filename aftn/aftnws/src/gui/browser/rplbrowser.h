#ifndef RPLBROWSER_H
#define RPLBROWSER_H

#include "ui_rplbrowser.h"
#include "abstractbrowser.h"
#include "rpleditor.h"

#include <QtCore/QString>
#include <QtCore/QModelIndex>

/// Use for displaying list of RPLs
class RplBrowser: public AbstractBrowser, public Ui::RplBrowser
{
  Q_OBJECT
  public:
    RplBrowser(QWidget* parent);

  protected slots:
    virtual void updateModel(); /*!< Update model with DB data according filters*/
    void onTableCurrentChanged(const QModelIndex&, const QModelIndex&); /*!< Call this slot when current row changet to update action buttons state*/
    void onTextFilterChanged(const QString& text); /*!< Call this when text filter changed to update displaing data*/
    void onCreateItem(); /*!< Create a new RPL*/
    void onCloneItem(); /*!< Clone current RPL*/
    void onEditItem(); /*!< Edit current RPL*/
    void onDeleteItem(); /*!< Delete current RPL*/

  signals:
    void statusChanged(const QString& status); /*!< Emitted when status string changed*/

  protected:
    RplEditor* editor(); /*!< RplEditor lazy initialization*/
    virtual void changeEvent(QEvent *event);

    virtual void closeEvent(QCloseEvent *event);

  private:

    QSqlTableModel* model;
    QScopedPointer<RplEditor> rplEditor;
};

#include "objectsmanagerhelper.h"
class RplBrowserHelper: public ObjectsManagerHelper
{
  Q_OBJECT
  public:
    RplBrowserHelper(QObject *parent = 0);

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

#endif /*RPLBROWSER_H*/
