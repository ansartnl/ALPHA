#ifndef ABONENT_DLG_H
#define ABONENT_DLG_H

#include "ui_abonentdlg.h"
#include <QtGui/QDialog>

//! Class for abonents
class AbonentDlg : public QDialog, public Ui::AbonentDlg
{
    Q_OBJECT
public:
    enum TypeDlg { ALL, Abonents, Groups, AbonentInGroup, AbonentOutGroup };

    //! Constructor
    /*!
     \param pWidget a parent widget
     */
    AbonentDlg(TypeDlg type = ALL, int idGroup = 0, QWidget *parent = 0);
    virtual ~AbonentDlg();

    QList<QPair<int, QString> > selectedAbonents() const;
    QList<QPair<int, QString> > selectedGroups() const;
    QStringList selectedAddresses() const;

public slots:
    virtual void accept();

protected:
    //! Inherited from QDialog
    virtual void changeEvent(QEvent *event);

private slots:
//    void onOperButtons(QAbstractButton *pButton);
//    void addGroup();
//    void deleteGroup();
//    void addAbonent();
//    void deleteAbonent();
//    void addGroupAbonent();
//    void deleteGroupAbonent();
//    void groupSelected(const QModelIndex &qIndex);
//    void tableDataChanged();

private:
    void Translate();
//    bool Save();
//    void SetGroupID(int idGroup);
};

#endif // ABONENT_DLG_H
