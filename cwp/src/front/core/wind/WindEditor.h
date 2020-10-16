#ifndef XWIND_EDITOR_H
#define XWIND_EDITOR_H

#include "Dao.h"
#include "Wind.h"
#include "XAppSettings.h"

#include <QtCore/QSharedPointer>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QObject>

#include <QWidget>
#include <QLineEdit>
#include <QTableWidget>


class QComboBox;
class QTableWidget;
class QGridLayout;
class QIntValidator;

namespace gui
{

	//! Class for editing wind's characteristics
	class WindEditor
		: public QTableWidget,
		public AppSettings
	{
		Q_OBJECT
	public:

		//! Constructor and destructor
		WindEditor( QWidget *pParent);
		virtual ~WindEditor();

		//! Attach to specified toolbar controls
		void UpdateActions(QComboBox *pZonesCombo);

	protected:
		//@{
		//! Interface CXAppSettings
		virtual void Translate();
		virtual void UpdateXFont() {}
		virtual void SI() {}
		//@}
	private:

		int MetresToF(double dMetres);

	private slots:

		void OnZoneChanged(int iZoneIndex);
		void OnSpeedChanged(int iRow, unsigned int uiWindID, int iSpeed);
		void OnSpeedActivated(int iRow, unsigned int uiWindID);
		void OnAzimuthChanged(int iRow, unsigned int uiWindID, int iAzimuth);
		void OnAzimuthActivated(int iRow, unsigned int uiWindID);

	private:

		
		QComboBox *m_pZonesCombo;
		QMap<unsigned int, core::CWind> m_Winds;

	};

	//! Class used for editing WindSpeed (0 .. 9999)
	class WindSpeedLineEdit
		: public QLineEdit
	{
		Q_OBJECT
	public:

		WindSpeedLineEdit(int iRow, unsigned int uiWindID, int iSpeed, QWidget *pParent = NULL);

	signals:

		void speedChanged(int iRow, unsigned int uiWindID, int iSpeed);
		void speedActivated(int iRow, unsigned int uiWindID);

	private slots:

		void OnEditFinished();

	protected:

		virtual void focusInEvent(QFocusEvent *pEvent);

	private:

		int m_iRow;
		QIntValidator *m_pValidator;
		unsigned int m_uiWindID;
		int m_iSpeed;

	};

	//! Class used for editing Azimuth (0 .. 359)
	class WindAzimuthLineEdit
		: public QLineEdit
	{
		Q_OBJECT
	public:

		WindAzimuthLineEdit(int iRow, unsigned int uiWindID, int iAzimuth, QWidget *pParent = NULL);

	signals:

		void azimuthChanged(int iRow, unsigned int uiWindID, int iAzimuth);
		void azimuthActivated(int iRow, unsigned int uiWindID);

	private slots:

		void OnEditFinished();

	protected:

		virtual void focusInEvent(QFocusEvent *pEvent);

	private:
		int m_iRow;
		QIntValidator *m_pValidator;
		unsigned int m_uiWindID;
		int m_iAzimuth;
	};
}

#endif // XWIND_EDITOR_H
