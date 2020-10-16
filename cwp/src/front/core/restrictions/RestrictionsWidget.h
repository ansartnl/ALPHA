#ifndef RESTRICTIONSWIDGET_H
#define RESTRICTIONSWIDGET_H

#include <QWidget>
#include "front/common/XAppSettings.h"

namespace Ui {
    class RestrictionsWidget;
}
class QModelIndex;
class CRestrictionUser;

class RestrictionsWidget
        : public QWidget, public AppSettings
{
    Q_OBJECT

public:
    // Role for accessing dialog controls.
    enum UserRole
    {
        User,   //!< Lets add only restrictions
        Admin   //!< Adds (restrictions and obstacles). Obstacles do not have end date
    };

    explicit RestrictionsWidget(QWidget *parent = 0);
    ~RestrictionsWidget();

    //! Set current user role (controls behavior).
    void setUserRole(UserRole role);

    //@{
    void addFir(uint id, const QString &idName, const QString &name);
    void removeFir(uint id);
    void removeFirs();
    //@}

    //! Add zone to database and show it on the screen.
    void addZone();

    virtual void translate();

    const QSize GetVisibleSize() const;
    void SizeToContents(bool bSend = true);
    void Refresh();

signals:
    void sizeChanged();

protected:
    virtual void changeEvent(QEvent* event);
    virtual void closeEvent(QCloseEvent* pEvent);
    virtual void showEvent(QShowEvent * event);
    //@{
    //! Interface CXAppSettings
    virtual void Translate() {}
    virtual void UpdateXFont();
    virtual void SI() {}
    //@}

private slots:
    void on_toolButtonAdd_clicked();
    void on_toolButtonDelete_clicked();
    void on_toolButtonEditPoints_clicked();
    void on_toolButtonHighlight_clicked();
    void on_toolButtonRequest_clicked();

    void on_comboBoxFIR_currentIndexChanged(int index);

    void onUserresFinished(int result);

    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void onCurrentRowChanged(const QModelIndex & current, const QModelIndex & previous);

private:
    void setupUi();
    void rejectPickingMode();
    void sendUserZone(const QSharedPointer<CRestrictionUser>& pZone, const QString& sType, const QString& sFirIdName);

private:
    Ui::RestrictionsWidget *ui;
    UserRole                mRole;
};

//! Global reference to restriction widget instance.
extern RestrictionsWidget   *gWidgetRestriction;

#endif // RESTRICTIONSWIDGET_H
