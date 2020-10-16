#ifndef BEARING_EDITOR_H
#define BEARING_EDITOR_H

#include <QDialog>
#include <QMap>
#include "front/common/XAppSettings.h"

class QAbstractButton;
namespace Ui
{
    class BearingEditor;
}

//! Class for editing bearing point
class CBearingEditor
    : public QDialog,
    public AppSettings
{
    Q_OBJECT

    static const char DB_BEARING_TABLE[];
    static const char DB_BEARING_ID[];
    static const char DB_BEARING_NAME[];
    static const char DB_BEARING_FIR_NAME[];
    static const char DB_BEARING_RDF_LATITUDE[];
    static const char DB_BEARING_RDF_LONGITUDE[];
    static const char DB_BEARING_SAC[];
    static const char DB_BEARING_SIC[];
    static const char DB_BEARING_CHANNEL[];
    static const char DB_BEARING_DECLINATION[];

public:
    CBearingEditor(QWidget *pParent = NULL);
    virtual ~CBearingEditor();

protected:
    //! Inherited from QWidget
    virtual void closeEvent(QCloseEvent* pEvent);
    //@{
    //! Interface CXAppSettings
    virtual void Translate();
    virtual void UpdateXFont() {}
    virtual void SI() {}
    //@}

private slots:
    void on_firCombo_currentIndexChanged(int);
    void on_operButtons_clicked(QAbstractButton*);
    void on_addBearingButton_clicked();
    void on_deleteBearingButton_clicked();
    void activateOperButtons();

private:
    Ui::BearingEditor *ui;
    QMap<QString, int> bearingModelIndex;
    QString m_sFirID;
    QString firName;
    QStringList firList;
    QString queryStr;
};

#endif // BEARING_EDITOR_H
