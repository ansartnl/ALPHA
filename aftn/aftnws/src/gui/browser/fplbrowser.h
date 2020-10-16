#ifndef FPLBROWSER_H
#define FPLBROWSER_H

#include "ui_fplbrowser.h"

#include "abstractbrowser.h"

#include "fpleditor.h"

#include <QtCore/QObject>
#include <QtCore/QString>

#include <QtCore/QMap>

class QWidget;
class QAction;
class QModelIndex;

/// Using for displaying active fpls
class FplBrowser: public AbstractBrowser, public Ui::FplBrowser
{
  Q_OBJECT
  public:

    /** Constructor*/
    FplBrowser(QWidget* pParent);

    //! Id of selected object
    QString SelectedObject(void);

    //! Set current selection
    void SetCurSelByID(const QString& sID);

    void setCanOtherInfoEmpty(bool bCan);

  protected slots:
    virtual void updateModel(); /*!< Update model with DB data according filter string*/
    void onTableCurrentChanged(const QModelIndex&, const QModelIndex&); /*!< Call this slot when current row changet to update action buttons state*/
    void onTextFilterChanged(const QString& text); /*!< Call this when text filter changed to update displaing data*/
    void onCreateItem(); /*!< Create a new FPL*/
    void onCloneItem(); /*!< Clone current FPL*/
    void onEditItem(); /*!< Edit current FPL*/
    void onDeleteItem(); /*!< Delete current FPL*/

  signals:
    void statusChanged(const QString& status); /*!< Emitted when status string changed*/
    void selectionItemChanged(const QString& sSelectedId); /*!< Emitted when selection changed*/

  protected:

    FplEditor* editor(); /*!< FplEditor lazy initialization*/
    virtual void changeEvent(QEvent *event);
    virtual void closeEvent(QCloseEvent *event);
    QSqlTableModel* getModel()
    { return model; }

  private:
    QScopedPointer<FplEditor> fplEditor;
    QSqlTableModel* model;
    QString statusBrowser;
    bool canOtherInfoEmpty;
};

#ifdef AFTNWS_PRJ_APP

#include "objectsmanagerhelper.h"

class FplBrowserHelper: public ObjectsManagerHelper
{
  Q_OBJECT
  public:
    FplBrowserHelper(QObject *parent = 0);

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

#endif

#endif
