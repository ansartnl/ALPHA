#include "MeteoRunwayModel.h"

#include <QtCore/QSet>
#include <QtCore/QStringList>
#include <QtGui/QColor>
#include <QtGui/QFont>
#include <QApplication>


////////////////////////////////////////////////////////////////////////////////////////
// MeteoRunwayModel implementation.

MeteoRunwayModel::MeteoRunwayModel(const QString &currentRunway,
                                   const TMeteoList &meteo,
                                   QObject *parent)
    : QAbstractItemModel(parent), mCurrentRunway(currentRunway),
      mMeteo(meteo), mIsEditable(true)
{
}

QModelIndex MeteoRunwayModel::index(int row, int column,
                                    const QModelIndex &parent) const
{
    if ( !hasIndex(row, column, parent) )
        return QModelIndex();

    Q_ASSERT(row < mMeteo.size());
    MeteoRunway *meteoRunway = const_cast<MeteoRunway *>( &mMeteo.at(row) );
    return createIndex(row, column, reinterpret_cast<void *>(meteoRunway));
}

QModelIndex MeteoRunwayModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

Qt::ItemFlags MeteoRunwayModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    Qt::ItemFlags itemFlags = Qt::ItemIsEnabled;// | Qt::ItemIsSelectable;
    if ( mIsEditable )
        itemFlags |= Qt::ItemIsEditable;
    return itemFlags;
}

int MeteoRunwayModel::rowCount(const QModelIndex &parent) const
{
    return mMeteo.size();
}

int MeteoRunwayModel::columnCount(const QModelIndex &parent) const
{
    return 7;
}

QVariant MeteoRunwayModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const
{
    return QVariant();
}

QVariant MeteoRunwayModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
        return QVariant();

    int column = index.column();

    Q_ASSERT( column >= 0 && column < columnCount() );
    Q_ASSERT( index.internalPointer() );

    const MeteoRunway &meteo = *reinterpret_cast<MeteoRunway *>( index.internalPointer() );

    if ( role == Qt::DisplayRole )
    {
        // 31-0900 - номер ВПП + время наблюдения (час мин)
        if ( column == 0 )
        {
            QString controlTime = meteo.contains(MeteoRunway::ControlTime) ?
                        meteo.value(MeteoRunway::ControlTime).toDateTime().toString("hhmm") : "____";

            return QString("%1-%2").arg(meteo.GetRunwayName()).arg(controlTime);
        }
        // M130-13G15/20.8/+15
        // Магнитное направление ветра, средняя и максимальная скорость,
        // боковая и продольная составляющая ветра
        else if ( column == 1 )
        {
            QString magnitudeWind = meteo.contains(MeteoRunway::MagneticWind) ?
                        QString::number(meteo.value(MeteoRunway::MagneticWind).toUInt()) : "___";
            QString averageSpeed = meteo.contains(MeteoRunway::AverageSpeed) ?
                        QString::number(meteo.value(MeteoRunway::AverageSpeed).toUInt()) : "__";
            QString maxSpeed = meteo.contains(MeteoRunway::MaxSpeed) ?
                        QString::number(meteo.value(MeteoRunway::MaxSpeed).toUInt()) : "__";
            QString sideWind = meteo.contains(MeteoRunway::SideWind) ?
                        QString::number(meteo.value(MeteoRunway::SideWind).toDouble()) : "___";
            QString longWindSign = meteo.contains(MeteoRunway::LongitudialWind) ?
                        (meteo.value(MeteoRunway::LongitudialWind).toDouble() >= 0. ? "+" : "") : "_";
            QString longWind = longWindSign + (meteo.contains(MeteoRunway::LongitudialWind) ?
                        QString::number(meteo.value(MeteoRunway::LongitudialWind).toDouble()) : "__");

            return QString("M%1-%2G%3/%4/%5").arg(magnitudeWind).arg(averageSpeed)
                    .arg(maxSpeed).arg(sideWind).arg(longWind);
        }
        // 0600
        // Метеорологическая дальность видимости.
        else if ( column == 2 )
        {
            QString distStr;
            if ( !meteo.contains(MeteoRunway::SightDistance) )
                distStr = "____";
            else
            {
                quint32 dist = meteo.value(MeteoRunway::SightDistance).toUInt();
                if ( dist > 9999 )
                    dist = 9999;
                distStr = QString("%1").arg(dist, 4, 10, QChar('0'));
            }

            return distStr;
        }
        // *7500/7600/7650
        // Метеорологическая дальность видимости ОВИ в начале, середине и конце ВПП
        else if ( column == 3 )
        {
            QString start = meteo.contains(MeteoRunway::SDistanceStart) ?
                        QString::number(meteo.value(MeteoRunway::SDistanceStart).toUInt()) : "____";
            QString middle = meteo.contains(MeteoRunway::SDistanceMiddle) ?
                        QString::number(meteo.value(MeteoRunway::SDistanceMiddle).toUInt()) : "____";
            QString end = meteo.contains(MeteoRunway::SDistanceEnd) ?
                        QString::number(meteo.value(MeteoRunway::SDistanceEnd).toUInt()) : "____";

            return QString("*%1/%2/%3").arg(start).arg(middle).arg(end);
        }
        // 1000 - Высота нижней границы облачности
        else if ( column == 4 )
        {
            return ( meteo.contains(MeteoRunway::LowerCloudiness) ?
                        QString::number(meteo.value(MeteoRunway::LowerCloudiness).toUInt()) : "____" );
        }
        // 740/0990(1019)
        // Давление на рабочем пороге и приведенное к уровню моря
        else if ( column == 5 )
        {
            // 1013,25 гПа = 760 мм.рт.ст.
            const qreal mmToGPaCoef = 1.33322368421;

            QString pressureMM = meteo.contains(MeteoRunway::Pressure) ?
                        QString::number(meteo.value(MeteoRunway::Pressure).toUInt()) : "___";
            QString pressureSea = meteo.contains(MeteoRunway::PressureSea) ?
                        QString::number(meteo.value(MeteoRunway::PressureSea).toUInt()) : "___";
            QString pressGPa = meteo.contains(MeteoRunway::Pressure) ?
                        QString::number(quint32(meteo.value(MeteoRunway::Pressure).toDouble()*mmToGPaCoef)) : "___";

            return QString("%1/%2(%3)").arg(pressureMM).arg(pressGPa).arg(pressureSea);
        }
        // +02.0/+00.0
        // Температура воздуха и точка росы
        else if ( column == 6 )
        {
            QString temperatureSign = meteo.contains(MeteoRunway::Temperature) ?
                        (meteo.value(MeteoRunway::Temperature).toDouble() >= 0. ? "+" : "") : "_";
            QString condPointSign = meteo.contains(MeteoRunway::CondensationPoint) ?
                        (meteo.value(MeteoRunway::CondensationPoint).toDouble() >= 0. ? "+" : "") : "_";

            QString temperature = temperatureSign + (meteo.contains(MeteoRunway::Temperature) ?
                        QString::number(meteo.value(MeteoRunway::Temperature).toDouble(), 'f', 1) : "__");
            QString condPoint = condPointSign + (meteo.contains(MeteoRunway::CondensationPoint) ?
                        QString::number(meteo.value(MeteoRunway::CondensationPoint).toDouble(), 'f', 1) : "__");

            return QString("%1/%2").arg(temperature).arg(condPoint);
        }
    }
    else if ( role == Qt::BackgroundRole )
    {
        if ( !mCurrentRunway.isEmpty() && meteo.GetRunwayName() == mCurrentRunway )
            return QColor(Qt::yellow);
    }
    else if ( role == Qt::TextAlignmentRole )
    {
        return Qt::AlignCenter;
    }

    return QVariant();
}

bool MeteoRunwayModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( role != Qt::EditRole )
        return false;

    int row = index.row(),
        column = index.column();

    if ( row >= mMeteo.size() )
        return false;

    MeteoRunway &meteo = mMeteo[row];

    // 0900 - время наблюдения (час мин)
    if ( column == 0 )
    {
        QTime controlTime = value.toTime();
        if ( controlTime.hour() == 0 && controlTime.minute() == 0 )
            meteo.remove(MeteoRunway::ControlTime);
        else
            meteo.insert(MeteoRunway::ControlTime, QDateTime(QDate::currentDate(), controlTime));
    }
    // M130-13G15/20.8/+15
    // Магнитное направление ветра, средняя и максимальная скорость,
    // боковая и продольная составляющая ветра
    else if ( column == 1 )
    {
        QStringList listSlash = value.toString().split("/");
        // Магнитное направление ветра, средняя и максимальная скорость
        if ( listSlash.size() > 0 )
        {
            QStringList listDash = listSlash[0].split("-");
            // Магнитное направление ветра
            if ( listDash.size() > 0 )
            {
                QString magneticWind = listDash[0];
                magneticWind.remove("M");

                if ( magneticWind.isEmpty() )
                    meteo.remove(MeteoRunway::MagneticWind);
                else
                {
                    quint32 direction = magneticWind.toUInt();
                    direction = direction > 360 ? 0 : direction;
                    meteo.insert(MeteoRunway::MagneticWind, direction);
                }
            }
            // средняя и максимальная скорость
            if ( listDash.size() > 1 )
            {
                QStringList listG = listDash[1].split("G");
                // средняя скорость
                if ( listG.size() > 0 )
                {
                    if ( listG[0].isEmpty() )
                        meteo.remove(MeteoRunway::AverageSpeed);
                    else
                        meteo.insert(MeteoRunway::AverageSpeed, listG[0].toUInt());
                }
                // максимальная скорость
                if ( listG.size() > 1 )
                {
                    if ( listG[1].isEmpty() )
                        meteo.remove(MeteoRunway::MaxSpeed);
                    else
                        meteo.insert(MeteoRunway::MaxSpeed, listG[1].toUInt());
                }
            }
        }
        // боковая составляющая ветра
        if ( listSlash.size() > 1 )
        {
            if ( listSlash[1].isEmpty() )
                meteo.remove(MeteoRunway::SideWind);
            else
                meteo.insert(MeteoRunway::SideWind, listSlash[1].toUInt());
        }
        // продольная составляющая ветра
        if ( listSlash.size() > 2 )
        {
            if ( listSlash[2].isEmpty() )
                meteo.remove(MeteoRunway::LongitudialWind);
            else
                meteo.insert(MeteoRunway::LongitudialWind, listSlash[2].toInt());
        }
    }
    // 0600
    // Метеорологическая дальность видимости.
    else if ( column == 2 )
    {
        quint32 dist = value.toUInt();
        if ( dist > 0 )
            meteo.insert(MeteoRunway::SightDistance, dist);
        else
            meteo.remove(MeteoRunway::SightDistance);
    }
    // *7500/7600/7650
    // Метеорологическая дальность видимости ОВИ в начале, середине и конце ВПП
    else if ( column == 3 )
    {
        QStringList listSlash = value.toString().remove("*").split("/");
        // Метеорологическая дальность видимости ОВИ в начале ВПП
        if ( listSlash.size() > 0 )
        {
            if ( listSlash[0].isEmpty() )
                meteo.remove(MeteoRunway::SDistanceStart);
            else
                meteo.insert(MeteoRunway::SDistanceStart, listSlash[0].toUInt());
        }
        // Метеорологическая дальность видимости ОВИ в середине ВПП
        if ( listSlash.size() > 1 )
        {
            if ( listSlash[1].isEmpty() )
                meteo.remove(MeteoRunway::SDistanceMiddle);
            else
                meteo.insert(MeteoRunway::SDistanceMiddle, listSlash[1].toUInt());
        }
        // Метеорологическая дальность видимости ОВИ в конце ВПП
        if ( listSlash.size() > 2 )
        {
            if ( listSlash[2].isEmpty() )
                meteo.remove(MeteoRunway::SDistanceEnd);
            else
                meteo.insert(MeteoRunway::SDistanceEnd, listSlash[2].toUInt());
        }
    }
    // 1000 - Высота нижней границы облачности
    else if ( column == 4 )
    {
        quint32 cloud = value.toUInt();
        if ( cloud > 0 )
            meteo.insert(MeteoRunway::LowerCloudiness, cloud);
        else
            meteo.remove(MeteoRunway::LowerCloudiness);
    }
    // 740/0990(1019)
    // Давление на рабочем пороге и приведенное к уровню моря
    else if ( column == 5 )
    {
        QStringList listSlash = value.toString().remove(")").split("(");
        // Давление на рабочем пороге
        if ( listSlash.size() > 0 )
        {
            if ( listSlash[0].isEmpty() )
                meteo.remove(MeteoRunway::Pressure);
            else
                meteo.insert(MeteoRunway::Pressure, listSlash[0].toUInt());
        }
        // Давление, приведенное к уровню моря
        if ( listSlash.size() > 1 )
        {
            if ( listSlash[1].isEmpty() )
                meteo.remove(MeteoRunway::PressureSea);
            else
                meteo.insert(MeteoRunway::PressureSea, listSlash[1].toUInt());
        }
    }
    // +02.0/+00.0
    // Температура воздуха и точка росы
    else if ( column == 6 )
    {
        QStringList listSlash = value.toString().split("/");
        // Температура воздуха
        if ( listSlash.size() > 0 )
        {
            if ( listSlash[0].isEmpty() )
                meteo.remove(MeteoRunway::Temperature);
            else
                meteo.insert(MeteoRunway::Temperature, listSlash[0].toDouble());
        }
        // точка росы
        if ( listSlash.size() > 1 )
        {
            if ( listSlash[1].isEmpty() )
                meteo.remove(MeteoRunway::CondensationPoint);
            else
                meteo.insert(MeteoRunway::CondensationPoint, listSlash[1].toDouble());
        }
    }
    else
        return false;

    emit dataChanged(index, index);
    return true;
}

void MeteoRunwayModel::setCurrentRunway(const QString &name)
{
    if ( mCurrentRunway == name )
        return;

    mCurrentRunway = name;
    emit layoutChanged();
}

void MeteoRunwayModel::setMeteo(const TMeteoList &meteo)
{
    mMeteo = meteo;
    emit layoutChanged();
}
