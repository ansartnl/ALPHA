#ifndef XLOADFRONDB_H
#define XLOADFRONDB_H
#include "ui_LoadFromDB.h"
#include "Dao.h"

#include <QDialog>

class QSqlTableModel;
//! Class for loading FIR from DB
class LoadFromDB: public QDialog, public Ui::LoadFromDB
{
    Q_OBJECT
    public:
        //! Constructor
        /*!
         \param Zones a list of CZone
         \param parent a parent widget
         */
        LoadFromDB(const QString& sTitle, bool bSectorInclude, QWidget* parent = 0);
        ~LoadFromDB();

        //! Get selected zones identifiers
        QVariantList results() const
        {
            return results_;
        }
        //! Get type of zone
        int GetResultType() const
        {
            return m_iResultType;
        }
        //! Get parent id
        QVariant GetParentID() const
        {
            return m_ResultParent;
        }

    protected:
        void changeEvent(QEvent* event);

    private:
        void loadSettings();
        void saveSettings();

    private slots:
        void OnFIRSelect(int iIndex);
        void OnButtonBox(QAbstractButton* pButton);
        void OnDblClick(const QModelIndex& qIndex);

    private:
        static const char DB_ZONE_ID[];
        static const char DB_ZONE_NAME[];
        static const char DB_ZONE_TYPE[];
        static const char DB_ZONE_IDNAME[];
        static const char DB_ZONE_PARENTID[];
        static const char DB_ZONE_LOWLIMIT[];
        static const char DB_ZONE_UPLIMIT[];

        QSqlTableModel* m_pTMFIR;
        QSqlTableModel* m_pTMSector;

        QVariantList results_;
        int m_iResultType;
        QVariant m_ResultParent;
};

#endif // XLOADFRONDB_H
