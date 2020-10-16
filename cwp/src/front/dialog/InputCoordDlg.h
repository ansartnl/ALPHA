#ifndef INPUT_COORD_DLG_H
#define INPUT_COORD_DLG_H

#include <QDialog>
#include "Coordinate.h"

class CoordEdit;

//! Dialog for input coordinate
class InputCoordDlg: public QDialog
{
    Q_OBJECT
public:
    InputCoordDlg(QWidget* parent = 0, QString sTitle = tr("Input coordinate"), QString sLabel = tr("Coordinate"));

    CCoord GetCoord() const;
    void SetCoord(const CCoord& coord);

protected slots:
    virtual void accept();

private slots:
    void OnButCopy();
    void OnButPaste();

private:
    CCoord      m_coord;
    CoordEdit*  m_pLatitude;
    CoordEdit*  m_pLongitude;
};

#endif // INPUT_COORD_DLG_H
