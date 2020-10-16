#ifndef ADDRESS_BOOK_H
#define ADDRESS_BOOK_H

#include "ui_addressbook.h"

#include <QtGui/QDialog>

#ifndef DUSHANBE_BUILD
#include "autotypeswidget.h"
#endif

class QSqlTableModel;
class QSqlQueryModel;

//! Class for editing address book
class AddressBook : public QDialog, public Ui::AddressBook
{
    Q_OBJECT

    static const QString tableAbonent;
    static const QString tableAbonentID;
    static const QString tableAbonentNAME;
    static const QString tableAbonentADDRESS;

    static const QString tableGroup;
    static const QString tableGroupID;
    static const QString tableGroupNAME;

public:
    //! Constructor
    /*!
     \param pWidget a parent widget
     */
    AddressBook(QWidget *parent = 0);
    virtual ~AddressBook();

protected:
    //! Inherited from QDialog
    virtual void closeEvent(QCloseEvent *pEvent);
    virtual void changeEvent(QEvent *event);

private slots:
    void onOperButtons(QAbstractButton *pButton);
    void addGroup();
    void deleteGroup();
    void addAbonent();
    void deleteAbonent();
    void addGroupAbonent();
    void deleteGroupAbonent();
    void groupSelected(const QModelIndex &qIndex);
    void abonentSelected(const QModelIndex &qIndex);
    void tableDataChanged();
    void tabChanged(int index);

#ifndef DUSHANBE_BUILD
    void onTypesChanged(const AutoTypesWidget::Types &t);
#endif

private:
    void Translate();
    bool Save();
    void SetGroupID(int idGroup);

    QMap<QString, int> groupIndexes;
    QMap<QString, int> abonentIndexes;
    QSqlTableModel *groupTableModel;
    QSqlTableModel *abonentTableModel;
    QSqlQueryModel *groupAbonentModel;
    bool bModified;
    int iGroupID;

#ifndef DUSHANBE_BUILD
    AutoTypesWidget *mAutoTypesWidget;
#endif
};

#endif // ADDRESS_BOOK_H
