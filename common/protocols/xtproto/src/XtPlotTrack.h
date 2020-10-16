#ifndef XT_PLOT_TRACK_DATA_H
#define XT_PLOT_TRACK_DATA_H

#include "xtproto_global.h"
#include "XtDefines.h"
#include "XtObject.h"
#include "XtCommon.h"
#include <QtGlobal>
#include <QVector>
#include <QtCore/QDataStream>

XT_BEGIN_NAMESPACE

//! Plot data storage
struct XTPROTO_EXPORT Plot : public CObject
{
    enum { InvalidBoardNumber = 0xFFFF };
    enum { InvalidNumber = 0xFFFF };
    enum { InvalidRadar = 0xFFFF };
    typedef int     TPlotMarkTypes;

    Plot() : mBoardNumber(InvalidBoardNumber), mNumber(InvalidNumber), mRlsId(InvalidRadar)
    {}

    QByteArray toArray() const;
    virtual void Serialize(QDataStream& out) const;
    virtual void Deserialize(QDataStream& in)
    {
        CObject::Deserialize(in);
        in >> mNumber >> mBoardNumber >> mRlsId >> mType >> mLocation;
    }

    bool operator== (const Plot &obj) const
    {
        return ( mNumber == obj.mNumber && mRlsId == obj.mRlsId
                 && mBoardNumber == obj.mBoardNumber
                 && mType == obj.mType && mLocation == obj.mLocation );
    }

    bool operator!= (const Plot &obj) const
    { return !operator ==(obj); }

    XT_OBJECT_TYPE_DECL

public:
    /** Идентификатор (номер борта или код ответчика) */
    quint16         mBoardNumber;
    /** Идентификатор плота */
    quint16         mNumber;
    /** Идентификатор локатора */
    quint16         mRlsId;
    /** Тип радиолокационной отметки (PlotMarkType и комбинация для PRL-VRL отметок) */
    TPlotMarkTypes  mType;
    /** Позиция ВС */
    Coord           mLocation;
};


//! Track data storage
struct XTPROTO_EXPORT Track : public CObject
{
    enum { InvalidBoardNumber = 0xFFFF };
    enum { InvalidNumber = 0xFFFF };
    enum { InvalidRadar = 0xFFFF };
    typedef int                 TTrackMarkTypes;

    Track() : mBoardNumber(InvalidBoardNumber), mNumber(InvalidNumber), mRlsId(InvalidRadar)
    {}

    QByteArray toArray() const;
    virtual void Serialize(QDataStream& out) const;
    virtual void Deserialize(QDataStream& in)
    {
        CObject::Deserialize(in);
        in >> mNumber >> mBoardNumber >> mRlsId >> mType >> mLocation >> mVelocity;
    }

    bool operator== (const Track &obj) const
    {
        return ( mNumber == obj.mNumber && mRlsId == obj.mRlsId
                 && mBoardNumber == obj.mBoardNumber
                 && mType == obj.mType && mLocation == obj.mLocation
                 && mVelocity == obj.mVelocity );
    }

    bool operator!= (const Track &obj) const
    { return !operator ==(obj); }

    XT_OBJECT_TYPE_DECL

public:
    /** Идентификатор (номер борта или код ответчика) */
    quint16         mBoardNumber;
    /** Идентификатор трека */
    quint16         mNumber;
    /** Идентификатор локатора */
    quint16         mRlsId;
    /** Тип радиолокационной отметки (PlotMarkType и комбинация для PRL-VRL отметок) */
    TTrackMarkTypes mType;
    /** Позиция ВС */
    Coord           mLocation;
    /** Вектор скорости (метр/секунда) */
    Velocity        mVelocity;
};

XT_END_NAMESPACE

QDataStream &operator<<(QDataStream &out, const Xt::Plot &plot);
QDataStream &operator<<(QDataStream &out, const Xt::Track &track);

#endif // XT_PLOT_TRACK_DATA_H
