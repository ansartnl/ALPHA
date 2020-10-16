#ifndef ACTDLG_DEP_H
#define ACTDLG_DEP_H

#include "ui_ACTDlgDEP.h"
#include "SFPL.h"
#include "Dao.h"
#include "fplbrowser.h"

class CMaster;

//! ACT FPL for departure
class CACTDlgDEP: public QDialog, public Ui::ACTDialogDEP
{
    Q_OBJECT
public:
    //! Constructor
    explicit CACTDlgDEP(CMaster* pMaster, int code = 0, quint32 prevSFPLID = 0,
                     QWidget *parent = 0);

    //! Inserted SFPL
    QSharedPointer<CSFPL> GetSFPL() const
    {
        return m_pSFPL;
    }

private slots:
    void accept();
    //! change ACID
    void OnChangeACID(const QString& text);
    //! show FPLs
    void OnButFPL();
    //! changed FPL in view
    void OnSelFPLinView(const QString& sID);
    //! activation
    void OnButAct();

private:
    void SelFPLinView(QString sACID);

private:
    typedef QMap<QString, CFPL> TMapFPL;
    TMapFPL                 m_mapFPL;
    QSharedPointer<CSFPL>   m_pSFPL;
    quint32                 m_qPrevSFPLID;
    QString                 m_sEmptyType;
    CMaster*                m_pMaster;

    FplBrowser*             m_pViewFPL;
    QHBoxLayout*            m_pHBox;
    QPushButton*            m_pButAct;
};

#endif // ACTDLG_DEP_H
