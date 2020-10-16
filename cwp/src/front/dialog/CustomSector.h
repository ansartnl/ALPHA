#ifndef CUSTOM_SECTOR_H
#define CUSTOM_SECTOR_H

#include <QDialog>
#include "ui_CustomSector.h"

class QSqlTableModel;

//! Class for create custom sector
class CustomSector: public QDialog, public Ui::CustomSector
{
    Q_OBJECT

public:
    //! Constructor
    /*!
     \param idFir id FIR
     \param pParent a parent widget
     \param f a widget's flags
     */
    CustomSector(uint idFir, QWidget* pParent = 0, Qt::WindowFlags f = 0);
    //! Destructor
    virtual ~CustomSector();

    uint GetIdFIR() const;
    QString GetPrivileges() const;
    QString GetSectorType() const;
    QList<uint> GetSectors() const;

public slots:
    virtual void accept();

private slots:
    void OnComboFir(int index);
    void OnItemChanged(QTableWidgetItem* current, QTableWidgetItem* previous);
    void OnCheckMain(int state);

private:
    QSqlTableModel*     m_pTypeModel;
    bool                m_bChanging;
};

#endif // CUSTOM_SECTOR_H
