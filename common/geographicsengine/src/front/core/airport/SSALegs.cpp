#include "SSALegs.h"
#include "common/Algorithms.h"
#include "math.h"

#include <QtGui/QPainterPath>
#include <QtCore/QLineF>

#define RAD_TO_DEG	57.29577951308232
#define DEG_TO_RAD	.0174532925199432958


//! Base painter class.
class LegBasePainter
{
protected:
    LegBasePainter(QPainterPath &path)
        : mPath(path)
    {}

    QPainterPath    &mPath;
};


//! Painter for CF leg.
class LegCFPainter : public LegBasePainter
{
public:
    enum TurnDirection
    {
        TdUnknown,
        TdRight,
        TdLeft
    };

    LegCFPainter(QPainterPath &path, const QLineF &linePrev, const QLineF &lineCF,
                 TurnDirection turnDirection, qreal minTurnRadius)
        : LegBasePainter(path), gMinTurnRadius(minTurnRadius),
        mLinePrev(linePrev), mLineCF(lineCF), mTurn(turnDirection)
    {}

    void paint();

private:
    /*! Minimum turn radius*/
    qreal gMinTurnRadius;
    /*! Angle constraint - minumum angle that is used in calcualtion
        otherwise TF leg is used.
    */
    static const qreal gAngleConstraint;

    //! Previous course points to the intersection point and
    //! CF course points to the intersection point.
    void paint1case(const QLineF &linePrev, const QLineF &lineCF, const QPointF &pointInters);
    //! Previous course points to the intersection point and
    //! CF course points to the opposite direction from the intersection point.
    void paint2case(const QLineF &linePrev, const QLineF &lineCF, const QPointF &pointInters);
    //! Previous course points to the opposite direction from intersection point and
    //! CF course points to the intersection point.
    void paint3case(const QLineF &linePrev, const QLineF &lineCF, const QPointF &pointInters);
    //! Previous course points to the opposite direction from intersection point and
    //! CF course points to the opposite direction from the intersection point.
    void paint4case(const QLineF &linePrev, const QLineF &lineCF, const QPointF &pointInters);

    //! Direction of courses are parallel.
    void paintParallel(const QLineF &linePrev, const QLineF &lineCF);

    //! Circle turn on 90 degree.
    /*!
        \return True if turn was successfull
    */
    bool turn90(const QLineF &linePrev, const QLineF &lineCF, const QPointF &pointInters,
                bool isRight);
    //! Circle turn on 180 degree.
    /*!
        \return True if turn was successfull
    */
    bool turn180(const QLineF &linePrev, const QLineF &lineCF, bool isRight);

    //! Tangent turn via 2 circles adjacent to reference points
    void turnTangent2Circle(const QLineF &linePrev, const QLineF &lineCF, bool isRight);
    //! Tangent turn via 2 circles adjacent to reference points
    void turnTangentCassete(const QLineF &linePrev, const QLineF &lineCF, bool isRight);

    //! Tangent turn via 2 circles adjacent to reference points
    void turnTangentClosest(const QLineF &linePrev, const QLineF &lineCF, bool isRight);

    //! Find intersection point of 2 circles.
    /*! Make sure that circles intersect before using this function.

        \param P0, P1 Center of 2 circles
        \param r0, r1 Radius of each circle
        \param isRight What point from 2 to return
        \return Intersection point of 2 circles (see isRight input parameter)
    */
    QPointF findCircleIntersection(const QPointF &P0, const QPointF &P1,
                                   qreal r0, qreal r1, bool isRight);

    //! Find inner tangent line to 2 circles (tangent line through the center point
    //! of the line that joins P0 and P1 points).
    /*! Make sure that circles intersect before using this function.
        \param P0, P1 Center of 2 circles
        \param r0, r1 Radius of each circle
        \param isRight What point from 2 to return
        \return Intersection points between 2 circles and tangent line
    */
    QLineF findInnerCircleTangent(const QPointF &P0, const QPointF &P1,
                                  qreal r0, qreal r1, bool isRight);

    //! Find outer tangent line to 2 circles.
    /*! Make sure that circles intersect before using this function.
        \param P0, P1 Center of 2 circles
        \param r0, r1 Radius of each circle
        \param isRight What point from 2 to return
        \return Intersection points between 2 circles and tangent line
    */
    QLineF findOuterCircleTangent(const QPointF &P0, const QPointF &P1,
                                  qreal r0, qreal r1, bool isRight);

private:
    const QLineF&   mLinePrev;
    const QLineF&   mLineCF;
    TurnDirection   mTurn;
};
const qreal LegCFPainter::gAngleConstraint = 3.;


void LegCFPainter::paint()
{
    // If CF leg is a first leg in the path -> should not happen but still:
    // do TF leg instead.
    if (mLinePrev.isNull())
    {
        SsaPainter::drawTF(mPath, mLineCF.p1());
        return;
    }

    QLineF linePrev = mLinePrev;
    QLineF lineCF = mLineCF;

    // Distance between points should be bigger than minimum radius.
    if ( QLineF(linePrev.p1(), lineCF.p1()).length() <= 2.*gMinTurnRadius )
    {
        SsaPainter::drawTF(mPath, mLineCF.p1());
        return;
    }

//    qreal anglePrev = linePrev.angle();
//    qreal angleCF = lineCF.angle();
    qreal anglePrevToCF = linePrev.angle(lineCF);
//    qDebug("AnglePrev = %.3f  AngleCF = %.3f  Angle between = %.3f", anglePrev, angleCF, anglePrevToCF);

    QPointF pointInters;
    // Lines are parallel - do not do anything - draw TF leg instead.
    if ( anglePrevToCF == 0. || anglePrevToCF == 180. ||
         lineCF.intersect(linePrev, &pointInters) == QLineF::NoIntersection )
    {
        paintParallel(linePrev, lineCF);
        return;
    }

    QLineF linePrevInters(linePrev.p1(), pointInters);
    QLineF lineCFInters(lineCF.p1(), pointInters);

    // If intersection point lays on one of cource lines - do TF leg instead.
    if ( qFuzzyIsNull(linePrevInters.length()) || qFuzzyIsNull(lineCFInters.length()) )
    {
        turnTangentClosest(linePrev, lineCF, algo::isRightSide(linePrev, lineCF.p1()));
        return;
    }


    // 1. Previous course points to the intersection point and
    //    CF course points to the intersection point.

    if ( algo::classify(linePrev, linePrevInters) == algo::Direct &&
         algo::classify(lineCF, lineCFInters) == algo::Direct )
        paint1case(linePrev, lineCF, pointInters);

    // 2. Previous course points to the intersection point and
    //    CF course points to the opposite direction from the intersection point.

    else if ( algo::classify(linePrev, linePrevInters) == algo::Direct &&
              algo::classify(lineCF, lineCFInters) == algo::Opposite )
        paint2case(linePrev, lineCF, pointInters);

    // 3. Previous course points to the opposite direction from intersection point and
    //    CF course points to the intersection point.

    else if ( algo::classify(linePrev, linePrevInters) == algo::Opposite &&
              algo::classify(lineCF, lineCFInters) == algo::Direct )
        paint3case(linePrev, lineCF, pointInters);

    // 4. Previous course points to the opposite direction from intersection point and
    //    CF course points to the opposite direction from the intersection point.

    else if ( algo::classify(linePrev, linePrevInters) == algo::Opposite &&
              algo::classify(lineCF, lineCFInters) == algo::Opposite )
        paint4case(linePrev, lineCF, pointInters);
}

void LegCFPainter::paintParallel(const QLineF &linePrev, const QLineF &lineCF)
{
    bool isPointOnRight = algo::isRightSide(linePrev, lineCF.p1());

    if ( algo::classify(linePrev, lineCF) == algo::Opposite )
    {
        if ( turn180(linePrev, lineCF, isPointOnRight) )
            return;
    }
    else
    {
        turnTangentClosest(linePrev, lineCF, isPointOnRight);
    }
}

void LegCFPainter::paint1case(const QLineF &linePrev, const QLineF &lineCF, const QPointF &pointInters)
{
    bool isPointOnRight = algo::isRightSide(linePrev, lineCF.p1());

    if ( (mTurn == LegCFPainter::TdRight && !isPointOnRight) ||
         (mTurn == LegCFPainter::TdLeft && isPointOnRight) )
    {
        turnTangentCassete(linePrev, lineCF, isPointOnRight);
    }
    else
    {
        turnTangent2Circle(linePrev, lineCF, isPointOnRight);
    }
}

void LegCFPainter::paint2case(const QLineF &linePrev, const QLineF &lineCF, const QPointF &pointInters)
{
    qreal angle = algo::oppositeVector(linePrev).angle(lineCF);

    bool isPointOnRight = algo::isRightSide(linePrev, lineCF.p1());
    bool isRightTurn = isPointOnRight;

    QLineF linePrevOpposite = algo::oppositeVector(linePrev);

    angle = linePrevOpposite.angle(lineCF);
    qreal angleHalf = angle / 2.;

    if ( angleHalf < 0.001 )
    {
        paintParallel(linePrev, lineCF);
        return;
    }

    if ( turn90(linePrev, lineCF, pointInters, isRightTurn) )
        return;

    turnTangent2Circle(linePrev, lineCF, isPointOnRight);
}

void LegCFPainter::paint3case(const QLineF &linePrev, const QLineF &lineCF, const QPointF &pointInters)
{
    bool isPointOnRight = algo::isRightSide(linePrev, lineCF.p1());

    if ( (mTurn == LegCFPainter::TdRight && isPointOnRight) ||
         (mTurn == LegCFPainter::TdLeft && !isPointOnRight) ||
         mTurn == LegCFPainter::TdUnknown )
    {
        turnTangentCassete(linePrev, lineCF, !isPointOnRight);
    }
    else
    {
        turnTangentCassete(linePrev, lineCF, isPointOnRight);
    }
}

void LegCFPainter::paint4case(const QLineF &linePrev, const QLineF &lineCF, const QPointF &pointInters)
{
    bool isPointOnRight = algo::isRightSide(linePrev, lineCF.p1());

    if ( (mTurn == LegCFPainter::TdRight && !isPointOnRight) ||
         (mTurn == LegCFPainter::TdLeft && isPointOnRight) )
    {
        turnTangentCassete(linePrev, lineCF, isPointOnRight);
    }
    else
    {
        turnTangent2Circle(linePrev, lineCF, isPointOnRight);
    }
}

bool LegCFPainter::turn90(const QLineF &linePrev, const QLineF &lineCF,
                          const QPointF &pointInters, bool isRightTurn)
{
    QLineF linePrevOpposite = algo::oppositeVector(linePrev);

    qreal angle = linePrevOpposite.angle(lineCF);
    qreal angleHalf = angle / 2.;

    qreal lengthMinTurn = gMinTurnRadius / tan(DEG_TO_RAD * angleHalf);

    qreal lengthToPrev = QLineF(pointInters, linePrev.p1()).length(),
        lengthToCF = QLineF(pointInters, lineCF.p1()).length();

    if ( lengthToPrev < lengthMinTurn || lengthToCF < lengthMinTurn )
        return false;

    // Find minimum distance to any of points (prev or CF).
    qreal lengthMin = qMin(lengthToPrev, lengthToCF);
    qreal radiusTurn = tan(DEG_TO_RAD * angleHalf) * lengthMin;

    QLineF linePrevTranslate = linePrev;
    linePrevTranslate.setLength(lengthToPrev-lengthMin);

    QLineF linePrevTranslated = linePrev.translated(linePrevTranslate.dx(), linePrevTranslate.dy());
    QLineF linePrevNormal = linePrev.translated(linePrevTranslate.dx(), linePrevTranslate.dy())
                            .normalVector();
    linePrevNormal.setLength(radiusTurn);

    qreal coeffTurn = isRightTurn ? -1. : 1.;
    if ( isRightTurn )
        linePrevNormal = algo::oppositeVector(linePrevNormal);

    mPath.lineTo(linePrevNormal.p1());

    QPointF centerCircle = linePrevNormal.p2();
    mPath.arcTo(QRectF(centerCircle-QPointF(radiusTurn, radiusTurn), QSizeF(radiusTurn*2., radiusTurn*2.)),
                linePrevNormal.angle()+180., coeffTurn*(180.-angle));

    if ( !qFuzzyCompare(lengthToCF, lengthMin) )
        mPath.lineTo(lineCF.p1());

    return true;
}

bool LegCFPainter::turn180(const QLineF &linePrev, const QLineF &lineCF, bool isRightTurn)
{
    QLineF linePrevNormal = linePrev.normalVector();

    QPointF pointInters;
    if ( linePrevNormal.intersect(lineCF, &pointInters) == QLineF::NoIntersection )
        return false;

    qreal lengthToCF = QLineF(pointInters, linePrevNormal.p1()).length();

    if ( gMinTurnRadius > lengthToCF )
        return false;

    linePrevNormal.setP2(pointInters);

    bool isPrevPosBeyond = true;

    QLineF lineCFDirect(linePrev.p1(), lineCF.p1());
    if ( algo::classify(lineCFDirect, lineCF) == algo::Opposite )
    {
        QLineF lineCFNormal = lineCF.normalVector();
        if ( isRightTurn )
            lineCFNormal = algo::oppositeVector(lineCFNormal);
        lineCFNormal.setLength(lengthToCF);

        linePrevNormal = QLineF(lineCFNormal.p2(), lineCFNormal.p1());

        isPrevPosBeyond = false;

        mPath.lineTo(lineCFNormal.p2());
    }

    qreal radiusTurn = lengthToCF / 2.;

    linePrevNormal.setLength(radiusTurn);

    qreal coeffTurn = isRightTurn ? -1. : 1.;

    QPointF centerCircle = linePrevNormal.p2();
    mPath.arcTo(QRectF(centerCircle-QPointF(radiusTurn, radiusTurn), QSizeF(radiusTurn*2., radiusTurn*2.)),
                linePrevNormal.angle()+180., coeffTurn*180.);

    if ( isPrevPosBeyond )
        mPath.lineTo(lineCF.p1());

    return true;
}

void LegCFPainter::turnTangent2Circle(const QLineF &linePrev, const QLineF &lineCF, bool isRight)
{
    QLineF linePrevNormal = linePrev.normalVector();
    linePrevNormal.setLength(gMinTurnRadius);

    QLineF lineCFNormal = lineCF.normalVector();
    lineCFNormal.setLength(gMinTurnRadius);

    if ( !isRight )
        lineCFNormal = algo::oppositeVector(lineCFNormal);
    else
        linePrevNormal = algo::oppositeVector(linePrevNormal);

    QPointF pntCenter = (linePrevNormal.p2() + lineCFNormal.p2()) / 2.;
    qreal r1 = QLineF(pntCenter, linePrevNormal.p2()).length();

    QPointF pntPrev = findCircleIntersection(linePrevNormal.p2(), pntCenter,
                                             gMinTurnRadius, r1, !isRight);
    QPointF pntCF = findCircleIntersection(lineCFNormal.p2(), pntCenter,
                                           gMinTurnRadius, r1, !isRight);

    // First arc calculation ...
    qreal coeffTurn1 = isRight ? -1. : 1.;
    qreal angle1 = QLineF(linePrevNormal.p2(),linePrevNormal.p1()).angleTo(
            QLineF(linePrevNormal.p2(), pntPrev));

    if ( isRight )
        angle1 = 360. - angle1;

    QLineF lineTangent(lineCFNormal.p2(), pntCF);

    // Second arc calculation ...
    qreal coeffTurn2 = isRight ? 1. : -1.;
    qreal angle2 = lineTangent.angleTo( algo::oppositeVector(lineCFNormal) );

    if ( !isRight )
        angle2 = 360. - angle2;

    // Angle should not be too big. If it is then calculation were incorrect.
    if ( angle1 > 270. || angle2 > 270. )
    {
        turnTangentClosest(linePrev, lineCF, algo::isRightSide(linePrev, lineCF.p1()));
        return;
    }

    // Draw path ...

    QPointF centerCircle = linePrevNormal.p2();
    mPath.arcTo(QRectF(centerCircle-QPointF(gMinTurnRadius, gMinTurnRadius),
                       QSizeF(gMinTurnRadius*2., gMinTurnRadius*2.)),
                linePrevNormal.angle()+180., coeffTurn1*angle1);

    mPath.lineTo(pntCF);

    centerCircle = lineCFNormal.p2();
    mPath.arcTo(QRectF(centerCircle-QPointF(gMinTurnRadius, gMinTurnRadius),
                       QSizeF(gMinTurnRadius*2., gMinTurnRadius*2.)),
                lineTangent.angle(), coeffTurn2*angle2);
}

void LegCFPainter::turnTangentCassete(const QLineF &linePrev, const QLineF &lineCF, bool isRight)
{
    QLineF linePrevNormal = linePrev.normalVector();
    linePrevNormal.setLength(gMinTurnRadius);

    QLineF lineCFNormal = lineCF.normalVector();
    lineCFNormal.setLength(gMinTurnRadius);

    if ( !isRight )
    {
        linePrevNormal = algo::oppositeVector(linePrevNormal);
        lineCFNormal = algo::oppositeVector(lineCFNormal);
    }

    QPointF pntCenter = (linePrevNormal.p2() + lineCFNormal.p2()) / 2.;
    qreal r1 = QLineF(pntCenter, linePrevNormal.p2()).length();

    QPointF pntPrev = findCircleIntersection(linePrevNormal.p2(), pntCenter,
                                             gMinTurnRadius, r1, isRight);
    QPointF pntCF = findCircleIntersection(lineCFNormal.p2(), pntCenter,
                                           gMinTurnRadius, r1, !isRight);

    qreal coeffTurn = isRight ? 1. : -1.;
    qreal angle = QLineF(linePrevNormal.p2(),linePrevNormal.p1()).angleTo(
            QLineF(linePrevNormal.p2(), pntPrev));

    if ( !isRight )
        angle = 360. - angle;

    // Angle should not be too big. If it is then calculation were incorrect.
    if ( angle < 330. )
    {
        QPointF centerCircle = linePrevNormal.p2();
        mPath.arcTo(QRectF(centerCircle-QPointF(gMinTurnRadius, gMinTurnRadius),
                           QSizeF(gMinTurnRadius*2., gMinTurnRadius*2.)),
                    linePrevNormal.angle()+180., coeffTurn*angle);
    }

    QLineF lineTangent(lineCFNormal.p2(), pntCF);

    coeffTurn = isRight ? 1. : -1.;
    angle = lineTangent.angleTo( algo::oppositeVector(lineCFNormal) );

    if ( !isRight )
        angle = 360. - angle;

    mPath.lineTo(pntCF);

    QPointF centerCircle = lineCFNormal.p2();
    mPath.arcTo(QRectF(centerCircle-QPointF(gMinTurnRadius, gMinTurnRadius),
                       QSizeF(gMinTurnRadius*2., gMinTurnRadius*2.)),
                lineTangent.angle(), coeffTurn*angle);
}

void LegCFPainter::turnTangentClosest(const QLineF &linePrev, const QLineF &lineCF, bool isRight)
{
    QLineF linePrevNormal = linePrev.normalVector();
    linePrevNormal.setLength(gMinTurnRadius);

    QLineF lineCFNormal = lineCF.normalVector();
    lineCFNormal.setLength(gMinTurnRadius);
    QLineF lineCFNormalOpposite = lineCFNormal;
    lineCFNormalOpposite.setAngle(lineCFNormalOpposite.angle()+180.);

    if ( isRight )
        linePrevNormal = algo::oppositeVector(linePrevNormal);

    qreal lengthToCFRightCircle = QLineF(linePrevNormal.p2(), lineCFNormalOpposite.p2()).length();
    qreal lengthToCFLeftCircle = QLineF(linePrevNormal.p2(), lineCFNormal.p2()).length();

    QLineF lineTangent;
    QLineF lineCFNormalClosest;
    qreal coeffTangentTurn = 1.;
    if ( isRight )
    {
        if ( lengthToCFRightCircle < lengthToCFLeftCircle)
        {
            coeffTangentTurn = isRight ? -1. : 1.;
            lineCFNormalClosest = lineCFNormalOpposite;
            lineTangent = findOuterCircleTangent(linePrevNormal.p2(), lineCFNormalOpposite.p2(),
                                                 gMinTurnRadius, gMinTurnRadius, isRight);
        }
        else
        {
            coeffTangentTurn = isRight ? 1. : -1.;
            lineCFNormalClosest = lineCFNormal;
            lineTangent = findInnerCircleTangent(linePrevNormal.p2(), lineCFNormal.p2(),
                                                 gMinTurnRadius, gMinTurnRadius, isRight);
        }
    }
    else
    {
        if ( lengthToCFRightCircle < lengthToCFLeftCircle)
        {
            coeffTangentTurn = isRight ? 1. : -1.;
            lineCFNormalClosest = lineCFNormalOpposite;
            lineTangent = findInnerCircleTangent(linePrevNormal.p2(), lineCFNormalOpposite.p2(),
                                                 gMinTurnRadius, gMinTurnRadius, isRight);
        }
        else
        {
            coeffTangentTurn = isRight ? -1. : 1.;
            lineCFNormalClosest = lineCFNormal;
            lineTangent = findOuterCircleTangent(linePrevNormal.p2(), lineCFNormal.p2(),
                                                 gMinTurnRadius, gMinTurnRadius, isRight);
        }
    }

    if ( lineTangent.isNull() )
    {
        // Hope never make a loop here as this is a nice desicion instead of making TF leg.
        turnTangentClosest(linePrev, lineCF, !isRight);
        return;
    }

    qreal coeffTurn = isRight ? -1. : 1.;
    qreal angle = QLineF(linePrevNormal.p2(),linePrevNormal.p1()).angleTo(
            QLineF(linePrevNormal.p2(), lineTangent.p1()));

    if ( isRight )
        angle = 360. - angle;

    if ( angle > 355. )
    {
        mPath.lineTo(lineCF.p1());
        return;
    }
    else if ( angle > 300. )
    {
        // Hope this recursive call is safe.
        // Need to double check but see conditions above (in main function) instead.
        turnTangentClosest(linePrev, lineCF, !isRight);
        return;
    }

    QPointF centerCircle = linePrevNormal.p2();
    mPath.arcTo(QRectF(centerCircle-QPointF(gMinTurnRadius, gMinTurnRadius),
                       QSizeF(gMinTurnRadius*2., gMinTurnRadius*2.)),
                linePrevNormal.angle()+180., coeffTurn*angle);

    centerCircle = lineCFNormalClosest.p2();
    QLineF lineTangentNormal(centerCircle, lineTangent.p2());
    angle = lineTangentNormal.angleTo( algo::oppositeVector(lineCFNormalClosest) );

    if ( coeffTangentTurn < 0 )
        angle = 360. - angle;

    if ( QLineF(lineTangent.p1(),lineCF.p1()).length() < lineTangent.length() &&
         algo::classify(lineTangent, lineCF) == algo::Direct )
    {
        mPath.lineTo(lineCF.p1());
        return;
    }

    mPath.lineTo(lineTangent.p2());

    mPath.arcTo(QRectF(centerCircle-QPointF(gMinTurnRadius, gMinTurnRadius),
                       QSizeF(gMinTurnRadius*2., gMinTurnRadius*2.)),
                lineTangentNormal.angle(), coeffTangentTurn*angle);
}

QPointF LegCFPainter::findCircleIntersection(const QPointF &P0, const QPointF &P1,
                                             qreal r0, qreal r1, bool isRight)
{
    qreal d = r1;

    qreal a = (r0*r0 - r1*r1 + d*d) / (2.*d);
    QPointF P2 = P0 + a*( P1 - P0 ) / d;

    qreal h = sqrt(r0*r0 - a*a);

    qreal cr = isRight ? 1. : -1.f;

    qreal x3 = P2.x() - cr* h*( P1.y() - P0.y() ) / d;
    qreal y3 = P2.y() + cr* h*( P1.x() - P0.x() ) / d;

    return QPointF(x3, y3);
}

QLineF LegCFPainter::findInnerCircleTangent(const QPointF &P0, const QPointF &P1,
                                            qreal r0, qreal r1, bool isRight)
{
    QLineF linePnts(P0, P1);

    qreal r2 = r0 + r1;

    qreal lengthPnts = linePnts.length();
    if ( lengthPnts < r2 )
        return QLineF();

    qreal angle = asin(r2 / lengthPnts) * RAD_TO_DEG;

    qreal coeffTurn = isRight ? 1. : -1.;

    QLineF linePntsTan = linePnts;
    linePntsTan.setAngle(linePntsTan.angle()-coeffTurn*angle);
    linePntsTan.setLength(r2 / tan(angle*DEG_TO_RAD));

    QLineF linePntsTanNormal = linePntsTan.normalVector();
    linePntsTanNormal.setLength(r0);

    QPointF linePntsTanNormalVector(linePntsTanNormal.dx(), linePntsTanNormal.dy());
    return QLineF(linePntsTan.p1()+coeffTurn*linePntsTanNormalVector,
                  linePntsTan.p2()+coeffTurn*linePntsTanNormalVector);
}

QLineF LegCFPainter::findOuterCircleTangent(const QPointF &P0, const QPointF &P1,
                                            qreal r0, qreal r1, bool isRight)
{
    QLineF linePnts = isRight ? QLineF(P0, P1) : QLineF(P1, P0);

    QLineF linePntsNormal = linePnts.normalVector();
    linePntsNormal.setLength(r0);

    QPointF pntT1 = linePntsNormal.p2();

    linePntsNormal.translate(linePnts.dx(), linePnts.dy());

    linePntsNormal.setLength(r1);

    return ( isRight ? QLineF(pntT1, linePntsNormal.p2()) : QLineF(linePntsNormal.p2(), pntT1));
}



namespace SsaPainter
{
    void drawCF(QPainterPath &path, const QLineF &linePrev, const QLineF &lineCF,
                 int turnDirection, qreal radius)
    {
        LegCFPainter(path, linePrev, lineCF,
                     LegCFPainter::TurnDirection(turnDirection), radius).paint();
    }

    void drawTF(QPainterPath &path, const QPointF &pntNext)
    {
        path.lineTo(pntNext);
    }
}
