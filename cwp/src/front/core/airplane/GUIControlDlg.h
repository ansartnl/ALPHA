#ifndef GUICONTROLDLG_H
#define GUICONTROLDLG_H

#include <QWidget>
#include <QGraphicsProxyWidget>
#include <QModelIndex>
#include <QStringListModel>
#include "front/common/XAppSettings.h"

class QListView;
class QLineEdit;
class QPushButton;
class QTabBar;
class QTimeEdit;
class QGraphicsView;

//! Class for editing airplane's data
class CGUIControlDlg: public QWidget, public AppSettings
{
    Q_OBJECT

    public:
        enum Type
        {
                ListType,
                ListWithEditType,
                RelativeType,
                ListMachType,
                ListFootType,
                ListPointType,
                ListPointWithHoldType,
                FPLType,
                NSAssume,
                NSTransfer,
                NSTransfer_out,
                NSROF,
                TimeType,
                NumEditType,
                EchelonsWidgetType,
                Spd1Type,
                Spd2Type,
                speedWidget,
                verticalSpeedWidget,
                NSCalculate_sector,
                NSDailyPlan
        };

        //! Constructor
        CGUIControlDlg(const Type type, QWidget *parent = 0, Qt::WindowFlags wFlags = Qt::Popup);
        //! Destructor
        virtual ~CGUIControlDlg();

        //! Initialization function
        /*! \param sVal a current value */
        void Init(const QString &sVal);
        void SetACID(const QString &v) {m_ACID = v;}
        void SetCODE(const QString &v) {m_code = v;}
        //! Initialization function
        /*!
         \param sValues1 a first set of values
         \param sValues2 a second set of values
         */
        void SetValues(const QStringList& sValues1, const QStringList& sValues2)
        {
            m_sValues1 = sValues1;
            m_sValues2 = sValues2;
        }

        void Show(const QPoint &pos, const QRect &rect = QRect());

    signals:
        //! Signal is sent after editing
        /*! \param sValues a set of values */
        void itemChanged(const QStringList sValues);
        void rejected();

    protected slots:
        //! Selected item on list widget
        void SelectNewItem(const QModelIndex& item);
        //! Selected item on edit widget
        void SelectEditItem();
        //! Selected azimuth
        void OnAzimuth(int degrees);
        //! Selected azimuth
        void SelectExecBut();
        //! Selected toggle button
        void SelectToggBut();
        //! Selected tab widget
        void SelectTab(int index);
        //! Selected item on time widget
        void SelectTimeEdit();
        //! Create new CGUIControlDlg such type
        /*!
         \param pNewDlg a returned pointer to CGUIControlDlg
         \param type a dialog type
         */
        void OnGetControlDlg(CGUIControlDlg*& pNewDlg, CGUIControlDlg::Type type);
        //! Selected FL
        void onEchelonValueChanged(double valF, double valM, int measure);
        //! Selected landing
        void onEchelonValueLandChanged(QString sValue);
        //@{
	    //! Inherited from QGraphicsProxyWidget
        virtual void showEvent(QShowEvent *event);
    	virtual void hideEvent(QHideEvent *event);
	    //@}
        //! Slot for update()
        void OnSingleUpdate();

        void onValueChanged(const QString &new_spd);
        void onDailyPlanChoosed();
        void onRejected();

    protected:
        //@{
        //! Interface CXAppSettings
        virtual void Translate()
        {
        }
        virtual void UpdateXFont();
        virtual void SI()
        {
        }
        //@}

    private:
        void setWidget(QWidget *widget);

    private:
        QWidget *m_widget;
        Type m_type;
        bool m_butState;
        QStringListModel m_sModel;
        QString m_ACID;
        QString m_code;
        QStringList m_sValues1;
        QStringList m_sValues2;
        QListView* m_pListView;
        QLineEdit* m_pEdit;
        QPushButton* m_pButton;
        QTabBar* m_pTabBar;
        QTimeEdit* m_timeEdit;

};

#endif // GUICONTROLDLG_H
