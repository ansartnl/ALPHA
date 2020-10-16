#ifndef DAILYPLANWIDGET_H
#define DAILYPLANWIDGET_H

#include <set>
#include <memory>
#include <functional>

#include <QFrame>
#include <QHash>
#include <QSet>
#include <QSharedPointer>
#include <QtCore/QUuid>

#include "ui_DailyPlanWidget.h"

#include "AirplaneData.h"
#include "../aftn/aftnws/src/core/fpl.h"
#include <QGraphicsSceneMouseEvent>

class QToolButton;
class QGridLayout;
class QSpacerItem;

class FPL;

class DailyPlanWidget: public QFrame
{
	Q_OBJECT

	static const QString kWhiteBackgroundStyle;
	static const QString kOrangeBackgroundStyle;
	static const QString kGrayBackgroundStyle;
	static const QString kBorderStyle;

public:
    struct FlightLevel
    {
        QString name;
        int echelon;
        int meters;
        int feet;
    };

    struct ShortFpl
    {
        int id;
        QString acid;
        QString date;
        QString time;
        QString adep;
        QString ades;
    };

    static const int kInvalidValue = 0;

    DailyPlanWidget(const QString &acid, const QString &code,
                      QWidget *parent = 0, Qt::WindowFlags flags = Qt::Popup);
    virtual ~DailyPlanWidget();
    void setACID(const QString &acid);
    void setValues(const QList<ShortFpl> &values);

Q_SIGNALS:
    void accepted();
    void rejected();

protected:

    virtual bool eventFilter(QObject *obj, QEvent *event);

    virtual void showEvent(QShowEvent *event);

    virtual void closeEvent(QCloseEvent *event);

private Q_SLOTS:
    void toolClicked();

private slots:
    void onButNew();

private:
    QList<ShortFpl> loadShortFpls();

private:
    Q_DISABLE_COPY(DailyPlanWidget)

    void init();

    QSharedPointer<Ui::DailyPlanWidget> ui;
    bool first_show_;
    QSet<QToolButton *> buttons_;
    QList<ShortFpl> short_fpls_;
    int visible_buttons_count_;
    bool grid_visible_;
    FPL mFpl;
    QList<FPL> Fpls;
    QString code_;
    double koef;

};

#endif // DAILYPLANWIDGET_H
