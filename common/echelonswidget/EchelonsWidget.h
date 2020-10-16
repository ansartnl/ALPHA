#ifndef LEVELSWIDGET_H
#define LEVELSWIDGET_H

#include "ui_EchelonsWidget.h"

#include <QtCore/QMetaType>
#include <QtSql/QSqlDatabase>
#include <QFrame>

class QSqlQueryModel;
class QShowEvent;
class QToolButton;

/**
 * Creating this frame widget default as popup
 */
class EchelonsWidget: public QFrame, protected Ui::EchelonsWidget
{
Q_OBJECT

    Q_PROPERTY(int value READ value WRITE setValue)

    static const int METER_DIVIDER = 10;
    static const int METER_CAPACITY = 4;

public:
    enum Measure
    {
        Meters = 0, Feet = 1
    };

    //
    EchelonsWidget(Measure measure, int levelMetersMax = INT_MAX, const QSqlDatabase& database =
                    QSqlDatabase::database(), QWidget *parent = 0, Qt::WindowFlags flags = Qt::Popup);
    ~EchelonsWidget();

    void exec(const QPoint& position, int value);

    double value() const;
    void setValue(double value);
    void setValue(QString sValue);
    void setLandValue(QString sValue, QStringList sList);

    Measure measure() const;

    bool isRemainder() const;

    void setRemainderShown(bool shown);

    QSize sizeHint() const;

    void setVisibleButtons(bool bVisible);

signals:
    void valueChanged(double valueF, double valueM, int measure);
    void valueLandChanged(QString sValue);

protected:
    void changeEvent(QEvent* event);
    void showEvent(QShowEvent* event);

private slots:
    void toolClicked();

    void on_fButton_clicked(bool checked);
    void on_mButton_clicked(bool checked);

    void on_setCustom_clicked();

    void on_landButton_clicked();

private:
    void init(Measure measure);

    QSqlQueryModel *model;

    double mValue;
    Measure mMeasure;

    bool firstShow;

    QList<QToolButton*> buttons;

    QSqlDatabase mDatabase;
    int mlevelMetersMax;
    QGridLayout *glayout;

    Measure mFirstMeasure;
};

#endif // LEVELSWIDGET_H
