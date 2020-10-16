#include "ListConstructor.h"
#include "main.h"
#include <QBuffer>

CListConstructor::CListConstructor():
    sObjectName(QString::null)
{
}

CListConstructor::CListConstructor(const QString &ObjectName):
    sObjectName(ObjectName)
{
}

void CListConstructor::loadItems()
{
    if (!sObjectName.isNull()){
        globalSettings()->beginGroup(sObjectName);
        m_Items.clear();
        foreach (const QString &item, globalSettings()->childKeys()){
            if (item.left(4) == QString("data"))
            {
                QDataStream stream(
                            QByteArray::fromHex(
                                globalSettings()->value(item).toByteArray()));
                EditHeaderItem it;
                stream >> it;
                m_Items.append(it);
            }
        }        
        globalSettings()->endGroup();

        if (m_Items.isEmpty()){
            for (int i = 0; i < 41; ++i)
            {
                EditHeaderItem it;
                it.number = i;
                it.visible = true;
                switch (i)
                {
				case 0:  it.texts = QStringList() << QString("Eq")           << QString(""); break;
                case 1:  it.texts = QStringList() << QString("ACID")         << QString(""); break;
                case 2:  it.texts = QStringList() << QString("CODE")         << QString(""); break;
                case 3:  it.texts = QStringList() << QString("COPin")        << QString(""); break;
                case 4:  it.texts = QStringList() << QString("SID")          << QString(""); break;
                case 5:  it.texts = QStringList() << QString("COPin_SID")    << QString(""); break;
                case 6:  it.texts = QStringList() << QString("ETOin")        << QString(""); break;
                case 7:  it.texts = QStringList() << QString("ETO_ATOin")    << QString(""); break;
                case 8:  it.texts = QStringList() << QString("ETD_ATD")      << QString(""); break;
                case 9:  it.texts = QStringList() << QString("ETO_ATO_ETD")  << QString(""); break;
                case 10: it.texts = QStringList() << QString("PEL")          << QString(""); break;
                case 11: it.texts = QStringList() << QString("PEL_CFL")      << QString(""); break;
                case 12: it.texts = QStringList() << QString("COPout")       << QString(""); break;
                case 13: it.texts = QStringList() << QString("STAR")         << QString(""); break;
                case 14: it.texts = QStringList() << QString("COPout_STAR")  << QString(""); break;
                case 15: it.texts = QStringList() << QString("ETOout")       << QString(""); break;
                case 16: it.texts = QStringList() << QString("ETO_ATOout")   << QString(""); break;
                case 17: it.texts = QStringList() << QString("ETO_ATO_ATA")  << QString(""); break;
                case 18: it.texts = QStringList() << QString("RFL_TFL")      << QString(""); break;
                case 19: it.texts = QStringList() << QString("Type")         << QString(""); break;
                case 20: it.texts = QStringList() << QString("Wct")          << QString(""); break;
                case 21: it.texts = QStringList() << QString("Stand")        << QString(""); break;
                case 22: it.texts = QStringList() << QString("REM")          << QString(""); break;
                case 23: it.texts = QStringList() << QString("RVSM")         << QString(""); break;
                case 24: it.texts = QStringList() << QString("Type_AC")      << QString(""); break;
                case 25: it.texts = QStringList() << QString("ADEP")         << QString(""); break;
                case 26: it.texts = QStringList() << QString("DEST")         << QString(""); break;
                case 27: it.texts = QStringList() << QString("ALTN")         << QString(""); break;
                case 28: it.texts = QStringList() << QString("Course Angle") << QString(""); break;
                case 29: it.texts = QStringList() << QString("Speed")        << QString(""); break;
                case 30: it.texts = QStringList() << QString("Scroll")       << QString(""); break;
                case 31: it.texts = QStringList() << QString("STATUS")       << QString(""); break;
                case 32: it.texts = QStringList() << QString("LIT")          << QString(""); break;
                case 33: it.texts = QStringList() << QString("FPLEqf1")      << QString(""); break;
                case 34: it.texts = QStringList() << QString("FPLEqf2")      << QString(""); break;
                case 35: it.texts = QStringList() << QString("FPLEqf3")      << QString(""); break;
                case 36: it.texts = QStringList() << QString("ETD")          << QString(""); break;
                case 37: it.texts = QStringList() << QString("ETA")          << QString(""); break;
                case 38: it.texts = QStringList() << QString("FType")        << QString(""); break;
                case 39: it.texts = QStringList() << QString("MesType")      << QString(""); break;
                case 40: it.texts = QStringList() << QString("DATE")         << QString(""); break;
                }
                it.identifier = it.texts.at(0);
                m_Items.append(it);
            }
        }
    }
}

QStringList CListConstructor::languages() const
{
    return QStringList() << QObject::tr("Name") << QObject::tr("Comment");
}

EditHeaderItems CListConstructor::items() const
{
    return m_Items;
}

void CListConstructor::setItems(const EditHeaderItems &Items)
{
    m_Items = Items;    
    globalSettings()->beginGroup(sObjectName);
    int i = 1;
    foreach(EditHeaderItem it, m_Items){
        QByteArray bytes;
        QDataStream stream(&bytes, QIODevice::ReadWrite);
        stream << it;
        globalSettings()->setValue(QString("data_%1").arg(i++), bytes.toHex());
    }
    globalSettings()->endGroup();
}
