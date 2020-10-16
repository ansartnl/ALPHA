#ifndef XSQUAWKEDITOR_H
#define XSQUAWKEDITOR_H

#include <QWidget>
#include "ui_SquawkEditor.h"
#include "Dao.h"
#include "ViewDelegate.h"
#include "XAppSettings.h"

class QSqlRelationalTableModel;

//! Class for editing squawks
class SquawkEditor: public QWidget, public Ui::SquawkEditor, public AppSettings
{
    Q_OBJECT
        static const char DB_SQUAWK_ID[];
        static const char DB_SQUAWK_SQUAWK[];
        static const char DB_SQUAWK_ZONE_ID[];

    public:
        //! Constructor
        /*!
         \param pDB a pointer to Dao
         \param parent a parent widget
         */
        explicit SquawkEditor(QWidget *parent = 0);
        //! Destructor
        virtual ~SquawkEditor();

    protected:
        //! Inherited from QWidget
        virtual void closeEvent(QCloseEvent* pEvent);

        virtual void changeEvent(QEvent* event);
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
        virtual void OnAddRecord();
        virtual void OnDeleteRecord();
        virtual void OnRuleButtons(QAbstractButton* pButton);
        void OnFIRChanged(int iIndex);

        void OnViewActivate();

    private:
        void ActivateButtons(bool bActive);

        QSqlRelationalTableModel* m_pTableModel;
        QSharedPointer<CRelModifyDelegate> m_pModDelegate;
        quint32 m_qZoneID;
        QMap<QString, int> m_mapModelIndex;
};

#endif // XSQUAWKEDITOR_H
