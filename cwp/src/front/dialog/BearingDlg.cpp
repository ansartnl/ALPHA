#include "BearingDlg.h"
#include "QtCheckHeaderView.h"
#include "Dao.h"
#include "main.h"
#include <QTableWidgetItem>
#include "coordedit.h"

CBearingDlg::CBearingDlg(const QString &sFIR, QList<CBearingPoint> &list, QWidget *parent)
    : QDialog(parent), m_listBearing(list)
{
    setupUi(this);

    tableWidget->setColumnCount(7);
    tableWidget->setHorizontalHeaderLabels(
            QStringList() << tr("Name") << tr("FIR") << tr("Latitude") << tr("Longitude") << tr("SAC") << tr("SIC") << tr("Channel"));
    QtCheckHeaderView* pHeader = new QtCheckHeaderView(Qt::Horizontal, tableWidget);
    pHeader->setClickable(true);
    tableWidget->setHorizontalHeader(pHeader);
    pHeader->addCheckable(0);

    TVA_CATCHALL_TRY
    {
        dao()->LoadBearingList(sFIR, m_listAllBearing);
    }TVA_CATCHALL(err){
    LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}

    tableWidget->setSortingEnabled(false);
    tableWidget->setRowCount(m_listAllBearing.size());
    for(int i = 0; i < m_listAllBearing.size(); ++i)
    {
        const CBearingPoint& bearing = m_listAllBearing[i];
        QTableWidgetItem* item = new QTableWidgetItem(bearing.GetName());
        item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        item->setData(Qt::UserRole, i);
        tableWidget->setItem(i, 0, item);

        //item = new QTableWidgetItem(bearing.GetFIR_ID_NAME());
        item = new QTableWidgetItem(bearing.GetAirportID());
        item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        tableWidget->setItem(i, 1, item);

        item = new QTableWidgetItem(CoordEdit::toString(CoordEdit::Latitude, bearing.GetCoord().GetLatitude().GetAngle()));
        item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        tableWidget->setItem(i, 2, item);

        item = new QTableWidgetItem(CoordEdit::toString(CoordEdit::Longitude, bearing.GetCoord().GetLongitude().GetAngle()));
        item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        tableWidget->setItem(i, 3, item);

        item = new QTableWidgetItem(QString::number(bearing.GetSAC()));
        item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        tableWidget->setItem(i, 4, item);

        item = new QTableWidgetItem(QString::number(bearing.GetSIC()));
        item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        tableWidget->setItem(i, 5, item);

        item = new QTableWidgetItem(QString::number(bearing.GetChannel()));
        item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        tableWidget->setItem(i, 6, item);
    }
    for(int i = 0; i < m_listAllBearing.size(); ++i)
    {
        const CBearingPoint& bearing = m_listAllBearing[i];
        for(int j = 0; j < m_listBearing.size(); ++j)
        {
            if(m_listBearing[j].GetSAC() == bearing.GetSAC()
                    && m_listBearing[j].GetSIC() == bearing.GetSIC()
                    && m_listBearing[j].GetChannel() == bearing.GetChannel()
                    && m_listBearing[j].GetAirportID() == bearing.GetAirportID())
                    //&& m_listBearing[j].GetFIR_ID_NAME() == bearing.GetFIR_ID_NAME())
            {
                tableWidget->item(i, 0)->setCheckState(Qt::Checked);
                break;
            }
        }
    }
    tableWidget->setSortingEnabled(true);

    tableWidget->resizeColumnToContents(0);
}

void CBearingDlg::accept()
{
    m_listBearing.clear();
    for(int i = 0; i < tableWidget->rowCount(); ++i)
    {
        QTableWidgetItem* pItem = tableWidget->item(i, 0);
        if(Qt::Checked == pItem->checkState())
        {
            m_listBearing.append(m_listAllBearing[pItem->data(Qt::UserRole).toInt()]);
        }
    }
    QDialog::accept();
}
