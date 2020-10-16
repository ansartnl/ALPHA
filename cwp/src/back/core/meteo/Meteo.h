#ifndef METEO_H
#define METEO_H

#include <QtCore/QString>
#include <QtCore/QMap>

#include "MeteoPoint.h"

namespace core
{

	//! Class describes the meteo zones
	class CMeteo
	{
	public:

		typedef QList<QSharedPointer<CMeteoPoint> > TPointList;

		//! Meteo zone type
		enum Type
		{
			Cloudburst	= 0,		// LVN
			LoStorm		= 1,		// (R)
			MedStorm	= 2,		// R)
			HiStorm		= 3,		// R
			Hail		= 4,		// G
            Squall		= 5, 		// H00
            Snow        = 6,    //S
            Rain        = 7,    //N
            Fog         = 8     //F
		};

        CMeteo()
            : m_uiID(0), m_uiZoneID(0),
              m_Course(0), m_bCourse(false), m_Speed(0), m_bSpeed(false),
              m_HeightMin(0), m_bHeightMin(false), m_HeightMax(0), m_bHeightMax(false)
        {}
		virtual ~CMeteo() {}

		//@{
		//! Meteo zone primary key in DB
		void SetID(unsigned int uiID) { m_uiID = uiID; }
		unsigned int GetID() const { return m_uiID; }
		//@}

		//@{
		//! FIR primary key in DB
		void SetZoneID(unsigned int uiZoneID) { m_uiZoneID = uiZoneID; }
		unsigned int GetZoneID() const { return m_uiZoneID; }
		//@}

		//@{
		//! Meteo zone name
		void SetName(const QString &sName) { m_sName = sName; }
		const QString &GetName() const { return m_sName; }
		//@}

		//@{
		//! Meteo zone type
		void SetType(Type iType) { m_Type = iType; }
		Type GetType() const { return m_Type; }
		//@}

        //@{
        //! Meteo zone course
        void SetCourse(int course) { m_Course = course; m_bCourse = true; }
        int GetCourse() const { return m_Course; }
        bool IsCourse() const { return m_bCourse; }
        //@}

        //@{
        //! Meteo zone speed
        void SetSpeed(int speed) { m_Speed = speed; m_bSpeed = true; }
        int GetSpeed() const { return m_Speed; }
        bool IsSpeed() const { return m_bSpeed; }
        //@}

        //@{
        //! Meteo zone minimum height
        void SetHeightMin(int height) { m_HeightMin = height; m_bHeightMin = true; }
        int GetHeightMin() const { return m_HeightMin; }
        bool IsHeightMin() const { return m_bHeightMin; }
        //@}

        //@{
        //! Meteo zone maximum height
        void SetHeightMax(int height) { m_HeightMax = height; m_bHeightMax = true; }
        int GetHeightMax() const { return m_HeightMax; }
        bool IsHeightMax() const { return m_bHeightMax; }
        //@}

		//@{
		//! Meteo zone points
		void AddPoint(const QSharedPointer<CMeteoPoint>& point) { m_Points.append(point); }
		void SetPoints(const TPointList list) { m_Points = list; }
		TPointList& GetPoints() { return m_Points; }
		//@}

	public:

		static QMap<Type, QString> Types()
		{
			QMap<Type, QString> Map;
			Map[Cloudburst] = "LVN";
			Map[LoStorm] = "(R";
			Map[MedStorm] = "R";
			Map[HiStorm] = "R";
			Map[Hail] = "G";
			Map[Squall] = "H00";
            Map[Snow] = "S";
            Map[Rain] = "N";
            Map[Fog] = "F";
			return Map;
        }

	private:

		unsigned int	m_uiID;
		unsigned int	m_uiZoneID;
		QString			m_sName;
		Type			m_Type;

        int             m_Course;
        bool            m_bCourse;
        int             m_Speed;
        bool            m_bSpeed;
        int             m_HeightMin;
        bool            m_bHeightMin;
        int             m_HeightMax;
        bool            m_bHeightMax;

		TPointList		m_Points;

	};

}

#endif // METEO_H
