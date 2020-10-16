
#ifndef NAVIGATEACTIONSMANAGERHELPER
#define NAVIGATEACTIONSMANAGERHELPER

#include <QtGui/QAction>

/**
* @brief
*
* @class NavigateActionsManagerHelper navigateactionsmanagerhelper.h
*/
class NavigateActionsManagerHelper : public QObject
{
  Q_OBJECT
public:
  explicit NavigateActionsManagerHelper(QObject* parent = 0);
  virtual ~NavigateActionsManagerHelper();

  virtual unsigned char approach(QWidget *widget) const {return 0;};
  virtual void activateFor(QWidget* widget){};
  virtual void deactivate() {};


  virtual bool canFirst() const {return false;};
  virtual bool canPrevious() const {return false;};
  virtual bool canTop() const {return false;};
  virtual bool canUp() const {return false;};
  virtual bool canDown() const {return false;};
  virtual bool canBottom() const {return false;};
  virtual bool canNext() const {return false;};
  virtual bool canLast() const {return false;};

public slots:
  virtual void first(){};
  virtual void previous(){};
  virtual void top(){};
  virtual void up(){};
  virtual void down(){};
  virtual void bottom(){};
  virtual void next(){};
  virtual void last(){};

signals:
  void canFirstChanged(bool enable);
  void canPreviousChanged(bool enable);

  void canTopChanged(bool enable);
  void canUpChanged(bool enable);

  void canBottomChanged(bool enable);
  void canDownChanged(bool enable);

  void canNextChanged(bool enable);
  void canLastChanged(bool enable);
};

#include <QtCore/QModelIndex>
class QAbstractItemView;
class ItemViewNavigateActionsManagerHelper : public NavigateActionsManagerHelper
{
  Q_OBJECT
public:
  explicit ItemViewNavigateActionsManagerHelper(QObject* parent = 0);
  virtual ~ItemViewNavigateActionsManagerHelper();

    virtual unsigned char approach(QWidget *widget) const;
  virtual void activateFor(QWidget* widget);
  virtual void deactivate();

  virtual bool canFirst() const ;
  virtual bool canPrevious() const ;
  virtual bool canTop() const ;
  virtual bool canUp() const ;
  virtual bool canDown() const ;
  virtual bool canBottom() const ;
  virtual bool canNext() const ;
  virtual bool canLast() const ;

public slots:
  virtual void first();
  virtual void previous();
  virtual void top();
  virtual void up();
  virtual void down();
  virtual void bottom();
  virtual void next();
  virtual void last();

private slots:
  void currentChanged(const QModelIndex& now, const QModelIndex& old);
private:
  QAbstractItemView *itemView;
  QPersistentModelIndex currentIndex;
};

class QTableView;
class TableViewNavigateActionsManagerHelper : public NavigateActionsManagerHelper
{
  Q_OBJECT
public:
  explicit TableViewNavigateActionsManagerHelper(QObject* parent = 0);
  virtual ~TableViewNavigateActionsManagerHelper();

  virtual unsigned char approach(QWidget *widget) const;
  virtual void activateFor(QWidget* widget);
  virtual void deactivate();

  virtual bool canFirst() const ;
  virtual bool canPrevious() const ;
  virtual bool canTop() const ;
  virtual bool canUp() const ;
  virtual bool canDown() const ;
  virtual bool canBottom() const ;
  virtual bool canNext() const ;
  virtual bool canLast() const ;

public slots:
  virtual void first();
  virtual void previous();
  virtual void top();
  virtual void up();
  virtual void down();
  virtual void bottom();
  virtual void next();
  virtual void last();

private slots:
  void currentChanged(const QModelIndex& now, const QModelIndex& old);
private:
  QTableView *tableView;
  QPersistentModelIndex currentIndex;
};

class QTreeView;
class TreeViewNavigateActionsManagerHelper : public NavigateActionsManagerHelper
{
  Q_OBJECT
public:
  explicit TreeViewNavigateActionsManagerHelper(QObject* parent = 0);
  virtual ~TreeViewNavigateActionsManagerHelper();

  virtual unsigned char approach(QWidget *widget) const;
  virtual void activateFor(QWidget* widget);
  virtual void deactivate();

  virtual bool canFirst() const ;
  virtual bool canPrevious() const ;
  virtual bool canTop() const ;
  virtual bool canUp() const ;
  virtual bool canDown() const ;
  virtual bool canBottom() const ;
  virtual bool canNext() const ;
  virtual bool canLast() const ;

public slots:
  virtual void first();
  virtual void previous();
  virtual void top();
  virtual void up();
  virtual void down();
  virtual void bottom();
  virtual void next();
  virtual void last();

private slots:
  void currentChanged(const QModelIndex& now, const QModelIndex& old);
private:
  QTreeView *treeView;
  QPersistentModelIndex currentIndex;
};



class QListView;
class ListViewNavigateActionsManagerHelper : public NavigateActionsManagerHelper
{
  Q_OBJECT
public:
  explicit ListViewNavigateActionsManagerHelper(QObject* parent = 0);
  virtual ~ListViewNavigateActionsManagerHelper();

  virtual unsigned char approach(QWidget *widget) const;
  virtual void activateFor(QWidget* widget);
  virtual void deactivate();

  virtual bool canTop() const ;
  virtual bool canUp() const ;
  virtual bool canDown() const ;
  virtual bool canBottom() const ;

public slots:
  virtual void top();
  virtual void up();
  virtual void down();
  virtual void bottom();

private slots:
  void currentChanged(const QModelIndex& now, const QModelIndex& old);
private:
  QListView *listView;

  QPersistentModelIndex currentIndex;
};

#endif //NAVIGATEACTIONSMANAGERHELPER
