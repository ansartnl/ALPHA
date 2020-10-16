#ifndef _GUI_POINT_H__
#define _GUI_POINT_H__

#include "GUIObject.h"
#include "orm/Point.h"
#include "GUIInterface.h"
#include "cs/CoordSystem.h"


//! Geodesy graphics engine
namespace geoGE {

/*!
  // There is an example of how GUI* object(s) is(are) organized in the GeoGE:
  \code
    typedef QSharedPointer<Scene>   TScenePointer;

    TScenePointer scene = TScenePointer(new GUISceneEx);

    ....

    // Load Arinc data from database.
    QSharedPointer<db::CDBQuery> dbQuery(m_DB->CreateQuery());
    dbQuery->Prepare(
        "SELECT point.id, name, longitude, latitude, icao, type, airport "
        "FROM point, zone_point "
        "WHERE zone_point.point_id = point.id AND zone_id = :1 AND type != 4");
    // Binding
    dbQuery->BindValue(":1", scene.GetID());

    if ( !dbQuery->exec() )
        throw tr("Execution error. %1").arg(dbQuery->lastError().text());

    // Reading point by point from database
    while(dbQuery->next())
    {
        QSharedPointer<Point> point(scene.CreatePoint((Point::Type)dbQuery->value(5).toInt()));
        point->SetID(dbQuery->value(0).toUInt());
        point->SetName(dbQuery->value(1).toString());
        point->SetCoord(CCoord(
                  dbQuery->value(2).toDouble(),
                  dbQuery->value(3).toDouble()));
        point->SetICAO(dbQuery->value(4).toString());
        point->SetType((Point::Type)dbQuery->value(5).toInt());
        point->SetAirportName(dbQuery->value(6).toString());

        scene.AddPoint(point);
  }
  \endcode

  Same philosophy is applied for other GUI representation.

  In order to add additional functionality for the object or its graphical
  representation it is only needed to make an inheritance and then allocate
  new object in Create* function of application level GUISceneEx class:
  \code
    class GUIPointMy : public geoGE::GUIPoint
    {
        ....
    };

    class GUISceneEx : public geoGE::GUIScene, public SceneEx
    {
        public:

        .....

        virtual QSharedPointer<arinc::Point> CreatePoint(arinc::Point::Type type) const
        {
            // Thus it will be possible to add (or even replace) additional
            // functionality for base GUIPoint behavior.

            // No more code is required to add - current Arinc loader leaves
            // unchanged but still does what we want (loads Arinc info from DB).
            return QSharedPointer<arinc::Point>(new GUIPointMy);
        }
    };
  \endcode

*/

//! Class for the graphical representation of FIR's point
class GUIPoint
    : public arinc::Point,
    public GUIUntransformableObject,
	public IGUIItemHandler
{
public:
	//! Constructor
    GUIPoint();

	//@{
    //! This function defines the outer bounds of the item as a rectangle
	virtual QRectF boundingRect() const;
    //! Returns the shape of this item as a QPainterPath in local coordinates
	QPainterPath shape() const;
    //! This function paints the contents of an item in local coordinates
	virtual void paint(
		QPainter *painter,
		const QStyleOptionGraphicsItem *option,
		QWidget *widget);
	//@}

	//@{
	//! Interface IGUIItemHandler
    virtual void SetPosition(const TCoordSystem &cs);
    virtual void SetSI(qint16 SI) {}
	//@}

	//! Inherited from ItemHandler
	virtual void MarkItemActive(bool bActive);

private:
	QPointF m_CenterPoint;
	QRectF m_Zone;
	int m_iPenWidth;
    QPainterPath    m_path;
};

}   // namespace geoGE

#endif // _GUI_POINT_H__
