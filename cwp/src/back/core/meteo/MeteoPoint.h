#ifndef METEOPOINT_H
#define METEOPOINT_H

#include "Coordinate.h"

namespace core {

	//! Class describes the points in meteo zone
	class CMeteoPoint
	{
	public:

		CMeteoPoint()
			: m_uiID(0), m_uiMeteoID(0), m_iSN(0)
		{}

		//@{
		//! Meteo point primary key in DB
		void SetID(unsigned int uiID) { m_uiID = uiID; }
		unsigned int GetID() const { return m_uiID; }
		//@}

		//@{
		//! Meteo zone primary key in DB
		void SetMeteoID(unsigned int uiMeteoID) { m_uiMeteoID = uiMeteoID; }
		unsigned int GetMeteoID() const { return m_uiMeteoID; }
		//@}

		//@{
		//! Meteo point coordinate
		void SetPosition(const CCoord &Coord) { m_Coord = Coord; }
		CCoord &GetPosition() { return m_Coord; }
		const CCoord &GetPosition() const { return m_Coord; }

		CCoord &GetCoord() { return m_Coord; }
		const CCoord &GetCoord() const { return m_Coord; }
		void SetCoord(const CCoord &Coord) { m_Coord = Coord; }
		//@}

		//@{
		//! Serial number of meteo point
		void SetSN(int iSN) { m_iSN = iSN; }
		int GetSN() const { return m_iSN; }
		//@}

	private:

		unsigned int m_uiID;
		unsigned int m_uiMeteoID;
		CCoord m_Coord;
		int m_iSN;

	};
}

#endif // METEOPOINT_H
