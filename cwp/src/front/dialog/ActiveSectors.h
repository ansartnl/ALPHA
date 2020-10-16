#ifndef ACTIVESECTORS_H
#define ACTIVESECTORS_H

#include "ui_ActiveSectors.h"



#include "Dao.h"
#include "import/Scene.h"


//! Class for sector activation
class ActiveSectors
	: public QDialog,
	public Ui::ActiveSectors
{
	Q_OBJECT
public:
	//! Constructor
	/*!
	  \param SectorList sector list
	  \param pParent a parent widget
	*/
	ActiveSectors(const Scene::TSceneList& SectorList, QWidget* pParent = 0);

protected:
	void changeEvent(QEvent* event);
private:
	Scene::TSceneList sectorList;

private slots:
	void accept();
};
#endif // ACTIVESECTORS_H
