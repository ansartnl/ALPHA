#ifndef XMETEO_EDITOR_H
#define XMETEO_EDITOR_H

#include <QtCore/QObject>
#include <QWidget>
#include <QSplitter>
#include <QtCore/QSharedPointer>
#include <QLineEdit>
#include <QComboBox>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QList>
#include <QTableWidget>

#include "Dao.h"
#include "core/meteo/Meteo.h"
#include "core/meteo/MeteoPoint.h"
#include "front/common/XAppSettings.h"

class QAction;
class QGridLayout;
class QTableWidget;
class QTableWidgetItem;
class QIntValidator;
class QDoubleValidator;

namespace gui
{

	//! Class for editing meteo zone
	class CXMeteoEditor
		: public QSplitter,
		public AppSettings
	{
		Q_OBJECT
	public:

		CXMeteoEditor( QWidget *pParent = NULL);
		virtual ~CXMeteoEditor();

		void UpdateActions(QComboBox *pZonesCombo, QAction *pNewMeteoAction, QAction *pDeleteMeteoAction,
						   QAction *pNewMeteoPointAction, QAction *pDeleteMeteoPointAction);

	protected:
		//@{
		//! Interface CXAppSettings
		virtual void Translate();
		virtual void UpdateXFont() {}
		virtual void SI() {}
		//@}

	private slots:

		void OnZoneChanged(int iZoneIndex);

		void OnMeteoNameActivated(unsigned int uiMeteoID);
		void OnMeteoNameChanged(unsigned int uiMeteoID, const QString &sName);
		void OnMeteoTypeActivated(unsigned int uiMeteoID);
		void OnMeteoTypeChanged(unsigned int uiMeteoID, core::CMeteo::Type Type);

		void OnMeteoPointSNActivated(unsigned int uiMeteoPointID);
		void OnMeteoPointSNChanged(unsigned int uiMeteoPointID, int iSN);
		void OnMeteoPointLongitudeActivated(unsigned int uiMeteoPointID);
		void OnMeteoPointLongitudeChanged(unsigned int uiMeteoPointID, double dLongitude);
		void OnMeteoPointLatitudeActivated(unsigned int uiMeteoPointID);
		void OnMeteoPointLatitudeChanged(unsigned int uiMeteoPointID, double dLatitude);

		void OnMeteoSelected();
		void OnMeteoPointSelected();

		void OnNewMeteo();
		void OnDeleteMeteo();
		void OnNewMeteoPoint();
		void OnDeleteMeteoPoint();

	private:

		void ReloadMeteos(unsigned int uiZoneID);
		void ReloadMeteoPoints(unsigned int uiMeteoID);
		void AddMeteo(const core::CMeteo &Meteo);
		void AddMeteoPoint(const core::CMeteoPoint &MeteoPoint);
		void SelectMeteo(unsigned int uiMeteoID);
		void SelectMeteoPoint(unsigned int uiMeteoPointID);
		int MeteoIDToRow(unsigned int uiMeteoID);
		int MeteoPointIDToRow(unsigned int uiMeteoPointID);
		void OnMeteoActivated(unsigned int uiMeteoID);
		void OnMeteoPointActivated(unsigned int uiMeteoPointID);

	private:

		
		QTableWidget *m_pMeteosTable;
		QTableWidget *m_pPointsTable;

		QMap<unsigned int, core::CMeteo> m_MeteoIDs;
		QList<unsigned int> m_MeteoIndexes;

		QMap<unsigned int, core::CMeteoPoint> m_PointIDs;
		QList<unsigned int> m_PointIndexes;

		QComboBox *m_pZonesCombo;
		QAction *m_pNewMeteoAction;
		QAction *m_pDeleteMeteoAction;
		QAction *m_pNewMeteoPointAction;
		QAction *m_pDeleteMeteoPointAction;

	};

	//! Class used for editing Meteo Name.
	class CXMeteoNameLineEdit
		: public QLineEdit
	{
		Q_OBJECT
	public:

		CXMeteoNameLineEdit(unsigned int uiMeteoID, const QString &sName, QWidget *pParent = NULL);

	signals:

		void nameChanged(unsigned int uiMeteoID, const QString &sName);
		void nameActivated(unsigned int uiMeteoID);

	private slots:

		void OnNameChanged();

	protected:

		virtual void focusInEvent(QFocusEvent *pEvent);

	private:

		unsigned int m_uiMeteoID;
		QString m_sName;

	};

	//! Class used for editing Meteo Type
	class CXMeteoTypeComboBox
		: public QComboBox
	{
		Q_OBJECT
	public:

		CXMeteoTypeComboBox(unsigned int uiMeteoID, core::CMeteo::Type Type, QWidget *pParent = NULL);

	signals:

		void typeChanged(unsigned int uiMeteoID, core::CMeteo::Type Type);
		void typeActivated(unsigned int uiMeteoID);

	private slots:

		void OnTypeChanged(int iTypeIndex);

	protected:

		virtual void focusInEvent(QFocusEvent *pEvent);

	private:

		unsigned int m_uiMeteoID;
		core::CMeteo::Type m_Type;

	};

	//! Class used for editing Meteo Point SN
	class CXMeteoPointSNLineEdit
		: public QLineEdit
	{
		Q_OBJECT
	public:

		CXMeteoPointSNLineEdit(unsigned int uiMeteoPointID, int iSN, QWidget *pParent = NULL);

	signals:

		void snChanged(unsigned int uiMeteoPointID, int iSN);
		void snActivated(unsigned int uiMeteoPointID);

	private slots:

		void OnEditFinished();

	protected:

		virtual void focusInEvent(QFocusEvent *pEvent);

	private:

		QIntValidator *m_pValidator;
		unsigned int m_uiMeteoPointID;
		int m_iSN;

	};

	//! Class used for editing Meteo Point Latitude or Longitude
	class CXMeteoPointLatLongLineEdit
		: public QLineEdit
	{
		Q_OBJECT
	public:

		CXMeteoPointLatLongLineEdit(unsigned int uiMeteoPointID, double dValue,
									double dMinValue, double dMaxValue, QWidget *pParent = NULL);

	signals:

		void valueChanged(unsigned int uiMeteoPointID, double dValue);
		void valueActivated(unsigned int uiMeteoPointID);

	private slots:

		void OnEditFinished();

	protected:

		virtual void focusInEvent(QFocusEvent *pEvent);

	private:

		QDoubleValidator *m_pValidator;
		unsigned int m_uiMeteoPointID;
		double m_dValue;

	};

}

#endif // XMETEO_EDITOR_H
