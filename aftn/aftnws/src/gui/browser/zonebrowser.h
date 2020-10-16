#ifndef ZONEBROWSER_H
#define ZONEBROWSER_H

#include "ui_zonebrowser.h"
#include "abstractbrowser.h"

#include <QtCore/QObject>
#include <QtSql/QSqlTableModel>

class ZoneBrowser: public AbstractBrowser, public Ui::ZoneBrowser
{
  Q_OBJECT
  public:
    ZoneBrowser(QWidget* pParent);

    protected slots:
    virtual void updateModel();

    /*! Большая процедура импортирования aring файла.
     Создается подключение к базе. Загружается файл с помощью библиотеки aringtodb.
     Отображается диалог выбора нужного РПИ, и если ok, то данный РПИ загружается в базу
     */
    //void on_importButton_clicked();
    void on_deleteButton_clicked();
    void on_filterButton_clicked();
    void on_filterEdit_textChanged(const QString&);

  protected:
    virtual void changeEvent(QEvent *event);
    virtual void closeEvent(QCloseEvent *event);

  private:

    QSqlTableModel* model;
};

#include "objectsmanagerhelper.h"
class ZoneBrowserHelper: public ObjectsManagerHelper
{
  Q_OBJECT
  public:
    ZoneBrowserHelper(QObject *parent = 0);

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

#endif // ZONEBROWSER_H
