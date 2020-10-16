#ifndef RESTRICTUSEREDITOR_H
#define RESTRICTUSEREDITOR_H

#include "baseeditor.h"

#include <QWidget>
#include <QMap>

namespace Ui {
    class RestrictUserEditor;
}
class QAbstractButton;
class QModelIndex;

class RestrictUserEditor : public BaseEditor
{
    Q_OBJECT

public:
    //! Constructor
    /*!
        \param pDB Pointer to database
        \param parent Parent widget
    */
    explicit RestrictUserEditor(QWidget *parent = 0);
    //! Destructor
    ~RestrictUserEditor();

    //! Interface ILanguage
    virtual void translate();

    virtual void refresh();

protected:
    //! Inherited from QWidget
    virtual void closeEvent(QCloseEvent* pEvent);
    virtual void changeEvent(QEvent* event);

private slots:
    void on_firCombo_currentIndexChanged(int);
    void on_operButtons_clicked(QAbstractButton*);
    void on_addRestrictionButton_clicked();
    void on_deleteRestrictionButton_clicked();
    void on_addPointButton_clicked();
    void on_deletePointButtun_clicked();
    void restrictionChanged(const QModelIndex& ModInd);
    void activateOperButtons();

    void on_toolButtonPastePolygon_clicked();
    void on_toolButtonPasteCircle_clicked();

private:
    //! Find maximum SN value in the table.
    int findMaxSN() const;

    Ui::RestrictUserEditor *ui;
    QMap<QString, int> restrictionsModelIndex;
    QMap<QString, int> restrictionPointIndex;
    quint32 m_qZoneID;
    quint32 m_qCurRestrictID;
};

#endif // RESTRICTUSEREDITOR_H
