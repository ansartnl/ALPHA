#include "ORCAMModel.h"
#include "Master.h"

#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QMessageBox>


const QString kOrcamFileName("orcam_list.txt");

ORCAMModel::ORCAMModel(const QString &minCode, const QString &maxCode, QObject *parent) :
    QAbstractTableModel(parent)
{
    bool validCode;

    mMinCode = minCode.toInt(&validCode, 8);
    if (!validCode)
        mMinCode = 0;

    mMaxCode = maxCode.toInt(&validCode, 8);
    if (!validCode)
        mMaxCode = 4095; // code 7777

    if (mMinCode > mMaxCode)
        qSwap(mMinCode, mMaxCode);

    //loadOrcamFile();

}

ORCAMModel::ORCAMModel(const QStringList &_lst, int _minutes, QObject *parent) :
    QAbstractTableModel(parent)
{
    lst = _lst;
    minutes = _minutes;
    loadOrcamFileFromRedis();
}

void ORCAMModel::checkTimeout(int minutes)
{
    bool changed = false;
    QDateTime current = QDateTime::currentDateTime();
    QList<QSharedPointer<CSFPL> > list;
    foreach (const ModelItem &item, mItems.values())
    {
        if (item.lastUpdate.secsTo(current) / 60 > minutes)
        {
            QString code = item.code;
            mItems.remove(code);
            codeChanged(code);

            changed = true;
            //////
            QUuid uid;
            if (master()->getRedisDB())
            {
                bool ok = master()->getRedisDB()->loadSFPLs(list);
                if(ok)
                {
                    QList<QSharedPointer<CSFPL> >::iterator itList = list.begin();
                    for(; itList != list.end(); ++itList)
                    {
                          QString coderedis = (*itList)->GetCODEtoString();
                          if(code == coderedis)
                          {
                            uid = (*itList)->GetUid();
                            break;
                          }
                    }
                }
                sfpl_ = QSharedPointer<CSFPL>(new CSFPL);
                ok = master()->getRedisDB()->loadSFPL(sfpl_,uid);
                #ifdef QT_DEBUG
                if (!ok)
                  logOperation(tr("Fail load from Redis record with code = '%1': and uid = '%2'").arg(code,uid.toString()));
                else
                {
                  ok = master()->getRedisDB()->deleteSFPL(sfpl_);
                  if (ok)
                  logOperation(tr("Deleted from Redis record with code = '%1': and uid = '%2'").arg(code,uid.toString()));
                }
                #endif
                if(ok)
                    master()->getRedisDB()->deleteSFPL(sfpl_);
             }
            //////
        }
    }

    if (changed)
        saveOrcamFile();
}

void ORCAMModel::clear()
{
    beginResetModel();
    mItems.clear();
    saveOrcamFile();
    endResetModel();
}

int ORCAMModel::findCode(const QString &filter, int startWith)
{
    int res = -1;

    if(filter.isEmpty())
        return -1;

    if(startWith < 0)
        startWith = 0;

    for (int i = startWith; i < rowCount() + startWith; ++i)
    {
        int row = i % rowCount();
        if (data(index(row, 0)).toString().contains(filter))
        {
            res = row;
            break;
        }
    }

    return res;
}

int ORCAMModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //return mMaxCode - mMinCode + 1;
    return lst.size();
}

int ORCAMModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant ORCAMModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    switch(role)
    {
    case Qt::TextAlignmentRole:
        return int(Qt::AlignLeft|Qt::AlignVCenter);
    case Qt::DisplayRole:
        return GetItemText(index);
    }
    return QVariant();
}

bool ORCAMModel::setData(const QModelIndex &index, const QVariant &value, int role)
{//sValues.at(0).rightJustified(4, '0')
    return QAbstractTableModel::setData(index, value, role);
}

QVariant ORCAMModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if((orientation == Qt::Horizontal) && (role == Qt::DisplayRole))
    {
        if((section >= 0) && (section < 2))
        {
            switch(section)
            {
            case 0: return tr("SSR code");
            case 1: return tr("Occupied");
            }
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

void ORCAMModel::airplaneData(const CAirplaneState &data)
{
    QString code = codeFromInt(data.GetMode3AReply());

    //if (data.GetMode3AReply() >= mMinCode && data.GetMode3AReply() <= mMaxCode)
    if (lst.contains(code))
    {
        //QString code = codeFromInt(data.GetMode3AReply());

        if (mItems.contains(code))
        {
            mItems[code].lastUpdate = QDateTime::currentDateTime();
        }
        else
        {
            ModelItem item;
            item.code = code;
            item.lastUpdate = QDateTime::currentDateTime();
            item.assigned = QDateTime::currentDateTime();
            mItems.insert(code, item);
            //addToOrcamFile(code, item.assigned);
            if(master() && master()->getRedisDB())
                master()->getRedisDB()->addOrcam(code, item.assigned, minutes);
            //codeChanged(code);
        }
    }
}

void ORCAMModel::Translate()
{
    emit headerDataChanged(Qt::Horizontal, 0, 2);
}

QVariant ORCAMModel::GetItemText(const QModelIndex &index) const
{
    if (!index.isValid())
        return QVariant();

    //QString code = codeFromInt(mMinCode + index.row());

    QString code = lst.at(index.row());

    if (index.column() == 0)
    {
        return code;
    }
    else if (index.column() == 1)
    {
        if (mItems.contains(code))
        {
           return mItems.value(code).assigned.secsTo(QDateTime::currentDateTime()) / 60;
            //if(mItems.value(code).assigned.isNull())
            //    return tr("OCCUPIED");
            //else return mItems.value(code).assigned;

        }
    }

    return QVariant();
}

void ORCAMModel::addToOrcamFile(const QString &code, const QDateTime &assigned_time)
{
    QFile file(kOrcamFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        return;

    QTextStream(&file) << code << " " << assigned_time.toString() << endl;
}

void ORCAMModel::loadOrcamFile()
{
    QFile file(kOrcamFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    TItems items;
    QTextStream in(&file);
    in.skipWhiteSpace();
    while (!in.atEnd()) {
        QStringList fields = in.readLine().split(' ');
        if (2 == fields.size()) {
            ModelItem item;
            item.code = fields.takeFirst();
            item.assigned = QDateTime::fromString(fields.takeFirst());
            item.lastUpdate = QDateTime::currentDateTime();
            items.insert(item.code, item);
        }
    }

    beginResetModel();
    mItems = items;
    endResetModel();
}

void ORCAMModel::saveOrcamFile()
{
    QFile file(kOrcamFileName);
    file.remove();
    if (mItems.isEmpty())
        return;
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    foreach (const ModelItem &item, mItems) {
        out << item.code << " " << item.assigned.toString() << endl;
    }
}

void ORCAMModel::codeChanged(const QString &code)
{
    int row = code.toInt(0, 8) - mMinCode;

    QModelIndex ind = index(row, 1);
    emit dataChanged(ind, ind);
}

QString ORCAMModel::codeFromInt(int value) const
{
    return QString::number(value, 8).rightJustified(4, '0');
}

void ORCAMModel::ChangedOrcamRedis(QString _code, QString assigned_time, QString otType)
{
    static QDateTime dt;
    dt = QDateTime::fromString(assigned_time);

    //int code = _code.toInt(0, 8);
    int code = _code.toInt();
    if(findCode(QString::number(code), 0) == -1)
        return;
    int row = findCode(QString::number(code), 0);
    const QModelIndex ind = index(row, 1);
    if (mItems.contains(QString::number(code))){
        if(otType == "delete")
            foreach (const ModelItem &item, mItems.values()) {
                if(_code == item.code)
                    mItems.remove(item.code);
            }
        else {
            mItems[QString::number(code)].lastUpdate = QDateTime::currentDateTime();
            mItems[QString::number(code)].assigned = dt;
        }
    }
    else {
        ModelItem item;
        item.code = QString::number(code);
        item.lastUpdate = QDateTime::currentDateTime();
        item.assigned = dt;
        mItems.insert(QString::number(code), item);
    }
    emit dataChanged(ind, ind);

}

void ORCAMModel::loadOrcamFileFromRedis()
{
    if (master()->getRedisDB()){
        QMultiMap<QString, QString> map;
        master()->getRedisDB()->loadOrcams(map);
        foreach(QString lst_code, lst){
            QMapIterator <QString, QString> iter(map);
            while (iter.hasNext()){
                iter.next();
                if(lst_code == iter.key())
                    ChangedOrcamRedis(lst_code, iter.value(),"insert");
            }
        }
    }
}

void ORCAMModel::checkTimeOccupied()
{
    foreach (const ModelItem &item, mItems.values())
    {
            QString code = item.code;
            item.assigned.addSecs(60);
            codeChanged(code);
    }
}

