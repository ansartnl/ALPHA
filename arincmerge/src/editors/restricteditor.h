#ifndef RESTRICTEEDITOR_H
#define RESTRICTEEDITOR_H

#include "ui_restricteditor.h"
#include "viewdelegate.h"
#include "baseeditor.h"
#include <QWidget>
#include <QtSql/QSqlDatabase>

class QSqlRelationalTableModel;
class QSqlQueryModel;

//! Class for editing restriction zones
class RestrictEditor : public BaseEditor, public Ui::RestrictEditor
{
Q_OBJECT

public:
    //! Constructor
    /*!
     \param pDB a pointer to CDB
     \param parent a parent widget
     */
    explicit RestrictEditor(QWidget* parent = 0, const QSqlDatabase &db = QSqlDatabase::database());
    //! Destructor
    virtual ~RestrictEditor();

    //! Interface ILanguage
    virtual void translate();

    virtual void refresh();

protected:
    void updateFirName();
    void setFirId();

    //! Inherited from QWidget
    void closeEvent(QCloseEvent *pEvent);
    void changeEvent(QEvent *event);

private slots:
    virtual void addRestriction();
    virtual void deleteRestriction();
    void restrictionChanged(const QModelIndex& ModInd);
    virtual void addPoint();
    virtual void deletePoint();
    virtual void onOperButton(QAbstractButton*);
    void activateOperButtons();

private:
    QSqlDatabase mDatabase;
    QSqlRelationalTableModel *restrictionsModel;
    QSqlRelationalTableModel *pointsModel;
    RelModifyDelegate *m_pModDelegateType;
    RelModifyDelegate *restrictionPointDelegate;
    quint32 m_qCurRestrictID;
    quint32 m_qZoneID;
    QMap<QString, int> restrictionsModelIndex;
    QMap<QString, int> restrictionPointIndex;
};

#endif // RESTRICTEEDITOR_H
