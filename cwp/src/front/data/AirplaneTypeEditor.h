#ifndef XAIRPLANETYPEEDITOR_H
#define XAIRPLANETYPEEDITOR_H

#include <QWidget>
#include "ui_AirplaneTypeEditor.h"
#include "Dao.h"
#include "front/common/ViewDelegate.h"
#include "front/common/XAppSettings.h"

class QSqlRelationalTableModel;

//! Class for editing types of airplane
class AirplaneTypeEditor: public QWidget, public Ui::AirplaneTypeEditor, public AppSettings
{
    Q_OBJECT
        static const char DB_AIR_TYPE_ID[];
        static const char DB_AIR_TYPE_LEVEL_MAX[];
        static const char DB_AIR_TYPE_BANK_MAX[];
        static const char DB_AIR_TYPE_BANK_DEF[];

        static const char DB_AIR_SPEED_ID[];
        static const char DB_AIR_SPEED_FLEVEL[];
        static const char DB_AIR_SPEED_V_CLIMB[];
        static const char DB_AIR_SPEED_V_CLIMB_MAX[];
        static const char DB_AIR_SPEED_DESCEND[];
        static const char DB_AIR_SPEED_DESCEND_MAX[];
        static const char DB_AIR_SPEED_IAS[];
        static const char DB_AIR_SPEED_IAS_MIN[];
        static const char DB_AIR_SPEED_IAS_MAX[];
        static const char DB_AIR_SPEED_AIR_ID[];

    public:
        //! Constructor
        /*!
         \param pDB a pointer to Dao
         \param parent a parent widget
         */
        explicit AirplaneTypeEditor(QWidget *parent = 0);
        //! Destructor
        virtual ~AirplaneTypeEditor();

    protected:
        //! Inherited from QWidget
        virtual void changeEvent(QEvent* event);
        virtual void closeEvent(QCloseEvent* pEvent);
        //@{
        //! Interface CXAppSettings
        virtual void Translate();
        virtual void UpdateXFont()
        {
        }
        virtual void SI()
        {
        }
        //@}


    private slots:
        virtual void OnAddTypeRecord();
        virtual void OnDeleteTypeRecord();
        void OnTypeChanged(const QModelIndex& ModInd);

        virtual void OnAddSpeedRecord();
        virtual void OnDeleteSpeedRecord();

        virtual void OnRuleButtons(QAbstractButton*);
        void OnViewActivate();

    private:
        void ActivateButtons(bool bActive);

        QSqlRelationalTableModel* m_pTMType;
        QSqlRelationalTableModel* m_pTMSpeed;
        QSharedPointer<CRelModifyDelegate> m_pModDelegateType;
        QSharedPointer<CRelModifyDelegate> m_pModDelegateSpeed;
        QString m_sCurAirType;
        QMap<QString, int> m_mapModelIndexType;
        QMap<QString, int> m_mapModelIndexSpeed;
};

#endif // XAIRPLANETYPEEDITOR_H
