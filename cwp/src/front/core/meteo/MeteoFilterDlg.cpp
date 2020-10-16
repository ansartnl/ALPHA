#include "MeteoFilterDlg.h"
#include "main.h"
#include "Master.h"

extern QString currentUserName;

MeteoFilterDlg::MeteoFilterDlg(QWidget *pParent, Qt::WindowFlags f)
    : QDialog(pParent, f)
{
    setupUi(this);

    QSettings &settings = *globalSettings();
    settings.beginGroup(currentUserName);
    QVariant var = settings.value("MeteoFilter");
    settings.endGroup();
    if(!var.isNull() && var.isValid())
    {
        QDataStream ds(var.toByteArray());
        ds >> m_FilterMap;
    }

    treeWidget->setRootIsDecorated(false);
    treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    treeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    treeWidget->sortItems(0, Qt::AscendingOrder);
}

MeteoFilterDlg::~MeteoFilterDlg()
{

}

bool MeteoFilterDlg::isFilter()
{
    QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(master()->GetView());
    if(master() && view && view->GetMainFIR())
    {
        return m_FilterMap.contains(view->GetMainFIR()->GetIDName());
    }
    return false;
}

void MeteoFilterDlg::accept()
{
    if(comboBox->currentIndex() != -1)
    {
        QString sAirport = comboBox->currentText();
        if(!sAirport.isEmpty())
        {
            TAirportMap airportMap;
            TRunwayList runwayList;
            for(int i = 0; i < treeWidget->topLevelItemCount(); ++i)
            {
                QTreeWidgetItem* pRunwayItem = treeWidget->topLevelItem(i);
                if(pRunwayItem && (pRunwayItem->checkState(0) == Qt::Checked))
                {
                    runwayList.insert(pRunwayItem->text(0));
                }
            }
            airportMap.insert(sAirport, runwayList);
            QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(master()->GetView());
            if(master() && view && view->GetMainFIR())
            {
                m_FilterMap.insert(view->GetMainFIR()->GetIDName(), airportMap);
            }

            QByteArray arr;
            QDataStream ds(&arr, QIODevice::WriteOnly);
            ds << m_FilterMap;
            QSettings &settings = *globalSettings();
            settings.beginGroup(currentUserName);
            settings.setValue("MeteoFilter", arr);
            settings.endGroup();
        }
    }

    QDialog::accept();
}

Qt::CheckState MeteoFilterDlg::RunwayCheckState(QString sFIR, QString sAirport, QString sRunway)
{
    TFirMap::const_iterator itFIR = m_FilterMap.find(sFIR);
    if(itFIR != m_FilterMap.constEnd())
    {
        TAirportMap::const_iterator itAirport = itFIR->find(sAirport);
        if(itAirport != itFIR->constEnd())
        {
            return itAirport->contains(sRunway) ? Qt::Checked : Qt::Unchecked;
        }
        else
        {
            return Qt::Unchecked;
        }
    }
    return Qt::Checked;
}

bool MeteoFilterDlg::isRunwayEnable(QString sAirport, QString sRunway)
{
    Qt::CheckState chState = Qt::Checked;
    QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(master()->GetView());
    if(master() && view && view->GetMainFIR())
    {
        chState = RunwayCheckState(view->GetMainFIR()->GetIDName(), sAirport, sRunway);
    }
    return chState == Qt::Checked;
}


QString MeteoFilterDlg::GetAirport()
{
    QString sAirport;
    QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(master()->GetView());
    if(master() && view && view->GetMainFIR())
    {
        TFirMap::const_iterator itFIR = m_FilterMap.find(view->GetMainFIR()->GetIDName());
        if(itFIR != m_FilterMap.constEnd())
        {
            TAirportMap::const_iterator itAirport = itFIR->constBegin();
            if(itAirport != itFIR->constEnd())
            {
                sAirport = itAirport.key();
            }
        }
    }
    return sAirport;
}

void MeteoFilterDlg::init(QString sAirport)
{
    comboBox->clear();
    QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(master()->GetView());
    if(master() && view && view->GetMainFIR())
    {
        QSharedPointer<Scene> pFIR = view->GetMainFIR();
        Scene::TAirportsMap::const_iterator itAirport = pFIR->GetAirports().begin();
        for(; itAirport != pFIR->GetAirports().end(); ++itAirport)
        {
            comboBox->addItem((*itAirport)->GetName(), (*itAirport)->GetID());
        }
    }
    int index = comboBox->findText(sAirport);
    if(index != comboBox->currentIndex())
        comboBox->setCurrentIndex(index);
    else
        on_comboBox_currentIndexChanged(index);

}

void MeteoFilterDlg::on_comboBox_currentIndexChanged(int index)
{
    treeWidget->clear();
    if(index != -1)
    {
        QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(master()->GetView());
        if(master() && view && view->GetMainFIR())
        {
            QString sAirport = comboBox->itemText(index);
            QSharedPointer<Scene> pFIR = view->GetMainFIR();
            Scene::TAirportsMap::const_iterator itAirport = pFIR->GetAirports().constFind(CAirportKey(sAirport));
            if(itAirport != pFIR->GetAirports().end())
            {
                CAirport::TRunways::const_iterator itRunway = (*itAirport)->GetRunways().begin();
                for(; itRunway != (*itAirport)->GetRunways().end(); ++itRunway)
                {
                    QTreeWidgetItem* pRunwayItem = new QTreeWidgetItem(treeWidget);
                    pRunwayItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);
                    pRunwayItem->setCheckState(0, RunwayCheckState(pFIR->GetIDName(), (*itAirport)->GetName(), (*itRunway)->GetName()));
                    pRunwayItem->setText(0, (*itRunway)->GetName());
                    pRunwayItem->setData(0, Qt::UserRole, (*itRunway)->GetID());
                }
            }
        }
    }
}
