#ifndef FPLDLG_H
#define FPLDLG_H
#include "ui_FormularFPL.h"

#include "SFPL.h"
#include "GUIControlDlg.h"

#include <QWidget>

class CAirplane;

//! Class for showing FPL in CGUIControlDlg
class CFPLDlg : public QWidget
        , public Ui::FPLDlg
{
    Q_OBJECT
public:
    //! Constructor
    explicit CFPLDlg(const QSharedPointer<CSFPL>& FPL, QWidget *parent = 0, Qt::WindowFlags f = 0);
    //! Destructor
    virtual ~CFPLDlg();

    //! Set pointer CAirplane, current airplane
    /*! \param pPlane a pointer CAirplane */
    void SetAirplane(CAirplane* pPlane);

protected:
    //@{
    //! Inherited from QDialog
    virtual bool eventFilter(QObject* obj, QEvent* ev);
    //@}

signals:
    //! Create new CGUIControlDlg such type
    /*!
      \param pNewDlg a returned pointer to CGUIControlDlg
      \param type a dialog type
    */
    void getControlDlg(CGUIControlDlg*& pNewDlg, CGUIControlDlg::Type type);
    //! ACID change request.
    void changeACID(quint16);

protected slots:
    //! Call after edit
    void OnEndEdit(const QStringList sValues);
    //! Activation
    void OnButtonActivate();

    //! ACID (aircraft id) set.
    void on_pushButtonACID_clicked();

    //! Show more information about SN
    void OnSNinfo();

private:
    QSharedPointer<CSFPL>	m_FPL;
    CAirplane*				m_pAirplane;
};

#endif // FPLDLG_H
