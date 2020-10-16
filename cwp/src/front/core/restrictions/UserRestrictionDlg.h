#ifndef USERRESTRICTIONDLG_H
#define USERRESTRICTIONDLG_H

#include <QDialog>
#include <QtCore/QMap>

namespace Ui {
    class UserRestrictionDlg;
}
class QModelIndex;
class CRestrictionUser;


class UserRestrictionDlg : public QDialog
{
    Q_OBJECT

public:
    // Role for accessing dialog controls.
    enum UserRole
    {
        User,   //!< Lets add only restrictions
        Admin   //!< Adds (restrictions and obstacles). Obstacles do not have end date
    };

    enum Type
    {
        Circle,
        Polygon
    };

    explicit UserRestrictionDlg(UserRole role, QWidget *parent = 0);
    ~UserRestrictionDlg();

    //! Set fir id.
    void setFirId(quint32 id) { mFirId = id; }

    //! Insert points from clipboard into table.
    void insertPointsFromClipboard(Type typePoints);

    //! Update user controls for editing.
    void insertZone(const CRestrictionUser &r);

    //! Return currently added restriction zone id.
    quint32 getId() const { return mRestrictionId; }

    virtual void accept();

    void translate();

protected:
    virtual void changeEvent(QEvent* event);

private slots:
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

    void on_comboBoxType_currentIndexChanged(int index);

    void on_toolButtonAdd_clicked();
    void on_toolButtonDelete_clicked();
    void on_toolButtonPaste_clicked();
    void on_toolButtonClear_clicked();

private:
    void setupUi();

    void setButtonBoxEnabled(bool on);

    //! Read points from clipboard.
    QList<QPointF> getFromClipboard();

    //! Find maximum SN value in the table.
    int findMaxSN() const;

    void pasteAsCircle();
    void pasteAsPolygon();

private:
    Ui::UserRestrictionDlg *ui;
    quint32                 mFirId;
    quint32                 mRestrictionId;
    UserRole                mRole;
    QMap<QString, int>      mPointsModelIndex;
};

#endif // USERRESTRICTIONDLG_H
