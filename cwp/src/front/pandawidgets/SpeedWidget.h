#ifndef SPEEDWIDGET_H
#define SPEEDWIDGET_H

#include <set>

#include <QFrame>
#include <QSet>
#include <QHash>
#include <QVector>

#include "ui_SpeedWidget.h"

class QToolButton;
class QGridLayout;


/**
    @class SpeedWidget
    @brief Виджет - рекомендуемая приборная скорость.
*/
class SpeedWidget: public QFrame
{
    Q_OBJECT

public:

    enum Measure {
        KMPH = 0,
        MACH,
        KNOT
    };

    SpeedWidget(const QString &acid, const QString &init_value, QWidget *parent = 0, Qt::WindowFlags flags = Qt::Popup);

    void setGrid(bool gridVisible = true);

    bool setCountVisibleButtons(int count);

    void setACID(const QString &acid);

    bool setVisibleValue(const QString &value);

    bool setValueColor(const QString &value, const QColor &foreground_color, const QColor &background_color);

    void setRangeColor(float valueMin, float valueMax, Measure measure, const QColor &foreground_color, const QColor &background_color);

Q_SIGNALS:

    void accepted(const QString &value);

    void rejected();

    void valueEnter(const QString &value);

protected:

    virtual void closeEvent(QCloseEvent *event);

    virtual bool eventFilter(QObject *obj, QEvent *event);

    virtual void showEvent(QShowEvent *event);

private Q_SLOTS:

    void toolClicked();

    void on_kmButton_clicked();

    void on_ktButton_clicked();

    void on_mButton_clicked();

    void on_tbRemove_clicked();

    void on_customValue_editingFinished();

private:
    Q_DISABLE_COPY(SpeedWidget)

    void init();

    void updateButtons(Measure);

    void initValues();
    void createButtons(Measure measure);

    bool valueFromString(const QString &input, Measure &measure, float &value);
    QString knotToString(const QString &value);
    QString machToString(const QString &value);

private:

    QSharedPointer<Ui::SpeedWidget> ui;
    QValidator *knot_validator_;
    QValidator *mach_validator_;

    Measure measure_;

    bool first_show_;

    QMultiHash<int, QToolButton *> buttons_;

    QGridLayout *grid_layout_;

    static QHash<int, std::set<float> > values_by_measure_;

    int visible_buttons_count_;

    bool grid_visible_;
};

#endif // SPEEDWIDGET_H
