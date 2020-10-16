#ifndef WIND_H
#define WIND_H

namespace core {

	//! Class describes the wind on flight level
	class CWind
	{
	public:

		CWind()
			: m_uiID(0), m_uiZoneID(0), m_dFLevelStart(0), m_dFLevelEnd(0),
			m_dSpeed(0), m_dAzimuth(0)
		{}

		//@{
		//! Properties
		void SetID(unsigned int uiID) { m_uiID = uiID; }
		unsigned int GetID() const { return m_uiID; }
		void SetZoneID(unsigned int uiZoneID) { m_uiZoneID = uiZoneID; }
		unsigned int GetZoneID() const { return m_uiZoneID; }
		void SetFLevelStart(double dFLevelStart) { m_dFLevelStart = dFLevelStart; }
		double GetFLevelStart() const { return m_dFLevelStart; }
		void SetFLevelEnd(double dFLevelEnd) { m_dFLevelEnd = dFLevelEnd; }
		double GetFLevelEnd() const { return m_dFLevelEnd; }
		void SetSpeed(double dSpeed) { m_dSpeed = dSpeed; }
		double GetSpeed() const { return m_dSpeed; }
		void SetAzimuth(double dAzimuth) { m_dAzimuth = dAzimuth; }
		double GetAzimuth() const { return m_dAzimuth; }
		//@}

	private:
		unsigned int m_uiID;
		unsigned int m_uiZoneID;
		double m_dFLevelStart;
		double m_dFLevelEnd;
		double m_dSpeed;
		double m_dAzimuth;
	};

}

#endif // WIND_H
