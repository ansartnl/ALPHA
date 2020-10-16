#ifndef USERACTIONSBROWSER_H
#define USERACTIONSBROWSER_H

#include "abstractbrowser.h"
#include "ui_useractionsbrowser.h"

class QSqlQueryModel;
class OciQueryModel;

class UserActionsBrowser : public AbstractBrowser, public Ui::UserActionsBrowserClass
{
    Q_OBJECT

public:
    UserActionsBrowser(QWidget *parent = 0);
    ~UserActionsBrowser();
protected:
    virtual void closeEvent(QCloseEvent* event);
    virtual void changeEvent(QEvent *event);

protected slots:
    void updateModel(); /*!< Update model with DB data according filters*/

private:
    QSqlQueryModel *users;
    QSqlQueryModel *types;
    OciQueryModel *fpls;
    OciQueryModel *rpls;
    OciQueryModel *mans;
    OciQueryModel *telegerams;
};

// --------------------------------------------
#include "objectsmanagerhelper.h"
class UserActionsBrowserHelper : public ObjectsManagerHelper
{
  Q_OBJECT

  Q_INTERFACES(ObjectsManagerHelper)
public:
  UserActionsBrowserHelper(QObject *parent = 0);

  virtual QString id() const;
  virtual QString path() const;
  virtual QString name() const;
  virtual QString description() const;

  virtual QIcon icon() const {return QIcon();}

  virtual QWidget *widget(QWidget* parent = 0) const;
  virtual QAction* action() const;
  virtual void retranslateUi();
private:
  QAction* mAction;
};
#endif // USERACTIONSBROWSER_H
