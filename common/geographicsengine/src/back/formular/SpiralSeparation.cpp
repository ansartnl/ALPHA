#include "SpiralSeparation.h"

#include <QtCore/QLineF>
#include <QtCore/QMutexLocker>


namespace FormularSeparation
{

bool SpiralSeparation::separate(quint16 id, QPointF &pos)
{
    Q_ASSERT(mFormulars && mLocker && mRepositionRect);

    FormularInfo fiCurrent;
    // Find current formular paramters.
    {
        QMutexLocker lock(mLocker);

        FormularMap::const_iterator itf = mFormulars->find(id);
        if ( itf != mFormulars->end() )
            fiCurrent = *itf;
    }

    if ( fiCurrent.isNull() )
        return false;

    // Check that current formular rectange is inside reposition rectangle of window ...

    if ( !(mRepositionRect->contains(fiCurrent.mPos) || mRepositionRect->contains(fiCurrent.mRect)) )
        return false;

    bool isIntersected = false;

    // Check current rect position. If it does not intersect any rect from the
    // separated list - quit.

    for ( SeparatedCont::const_iterator itc = mSeparated.constBegin();
        itc != mSeparated.constEnd(); ++itc )
    {
        if ( *itc == id )
            continue;

        FormularInfo fi;
        {
            QMutexLocker lock(mLocker);

            FormularMap::const_iterator itf = mFormulars->find(*itc);
            if ( itf != mFormulars->end() )
                fi = *itf;
        }

        if ( !fi.isNull() && fi.mRect.intersects(fiCurrent.mRect) )
        {
            isIntersected = true;
            break;
        }
    }

    if ( !isIntersected )
    {
        mSeparated.insert(id);
        return isIntersected;
    }

    // Spiral search of free space to insert the formular rectangle ...

    const int countAngles = 17;
    qreal angles[countAngles] = { 67., 44., 22., -22., -45., -67., -90., -114., -137., -160.,
                                  -182., -205., -226., -248., -292., -314., -338. };

    QRectF rectForm;

    qreal length = 60.;  // pixels
    // il - index for increasing length
    for ( int il = 0; il < 4; il++ )
    {
        length += il * 60.;

        qreal angle = 0.;

        // ia - index for changing angle
        for ( int ia = 0; ia < countAngles; ia++ )
        {
            isIntersected = false;

            angle = angles[ia];

            QLineF line(fiCurrent.mPos, fiCurrent.mPos+QPointF(1., 0.));
            line.setAngle(90-angle);
            line.setLength(length);

            pos = line.p2();

            rectForm = QRectF(pos.x(), pos.y(),
                              fiCurrent.mRect.width(), fiCurrent.mRect.height());

            for ( SeparatedCont::const_iterator itc = mSeparated.constBegin();
                itc != mSeparated.constEnd(); ++itc )
            {
                if ( *itc == id )
                    continue;

                FormularInfo fi;
                {
                    QMutexLocker lock(mLocker);

                    FormularMap::const_iterator itf = mFormulars->find(*itc);
                    if ( itf != mFormulars->end() )
                        fi = *itf;
                }

                if ( !fi.isNull() && fi.mRect.intersects(rectForm) )
                {
                    isIntersected = true;
                    break;
                }
            }

            if ( !isIntersected )
                break;
        }

        if ( !isIntersected )
            break;
    }

    if ( !isIntersected )
    {
        mSeparated.insert(id);

        // Update rect sizes in the formular map.
        if ( !rectForm.isNull() )
        {
            QMutexLocker lock(mLocker);

            FormularMap::iterator itf = mFormulars->find(id);
            if ( itf != mFormulars->end() )
                (*itf).mRect = rectForm;
        }
    }

    return !isIntersected;
}

}   // namespace FormularSeparation
