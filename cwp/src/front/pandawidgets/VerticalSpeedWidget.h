#ifndef VERTICALSPEEDWIDGET_H
#define VERTICALSPEEDWIDGET_H

#include <set>

#include <QVector>
#include <QFrame>
#include <QSet>
#include <QMultiHash>
#include <QHash>

#include "ui_VerticalSpeedWidget.h"

class QToolButton;
class QGridLayout;


/**
    @class VerticalSpeedWidget
    @brief Виджет - рекомендуемая вертикальная скорость.
*/
class VerticalSpeedWidget: public QFrame
{
    Q_OBJECT

public:

    enum Measure {
        METERS = 0,
        FEET,

        MEASURE_COUNT
    };

    VerticalSpeedWidget(const QString &acid, const QString &init_value, QWidget *parent = 0, Qt::WindowFlags flags = Qt::Popup);

    void setGrid(bool gridVisible = true);

    bool setCountVisibleButtons(int count);
    /**
        Метод устанавливает идентификатор воздушного судна.
        @param acid идентификатор воздушного судна.
    */
    void setACID(const QString &acid);

    /**
        Метод задает значение скорости отображаемое на экране при вызове виджета или
        заполняет текстовое поле заданным значением скорости, если такое значение не найдено.
        @param sValue значение эшелона.
    */
    bool setVisibleValue(const QString &sValue);
    /**
        Метод задает цвет для заданного значения скорости.
        @param sValue значение скорости.
        @param foregroundColor цвет надписи значения скорости на кнопке.
        @param backgroundColor цвет кнопки со значением скорости.
    */
    bool setValueColor(const QString &sValue, const QColor &foregroundColor, const QColor &backgroundColor);
    /**
        Метод задает цвет в указанных границах значений скоростей.
        @param valueMin нижняя граница значения скорости.
        @param valueMax верхняя граница значения скорости.
        @param measure единицы измерения.
        @param foregroundColor цвет надписи значения скорости на кнопке.
        @param backgroundColor цвет кнопки со значением скорости.
    */
    void setRangeColor(int valueMin, int valueMax, Measure measure, const QColor &foregroundColor, const QColor &backgroundColor);

Q_SIGNALS:
    /**
        Сигнал генерируется при скрытии виджета по причине выбора скорости.
    */
    void accepted(const QString &value);
    /**
        Сигнал генерируется при скрытии виджета по причине невыбора скорости.
    */
    void rejected();
    /**
        Сигнал генерируется при наведении на кнопку со значением скорости.
    */
    void valueEnter(const QString &value);

protected:
    /**
        Обработчик событий для кнопок скоростей.
    */
    virtual bool eventFilter(QObject *obj, QEvent *event);
    /**
        Обработчик отображения окна.
    */
    virtual void showEvent(QShowEvent *event);
    /**
        Обработчик запроса закрытия окна.
    */
    virtual void closeEvent(QCloseEvent *event);

private Q_SLOTS:

    void toolClicked();

    void on_fButton_clicked();

    void on_mButton_clicked();

    void on_tbRemove_clicked();

    void on_customValue_editingFinished();

private:
    Q_DISABLE_COPY(VerticalSpeedWidget)

    void init();

    void updateButtons(Measure);

    void initValues();
    void createButtons(Measure measure);

private:

    QSharedPointer<Ui::VerticalSpeedWidget> ui;

    Measure measure_;

    bool first_show_;

    QMultiHash<int, QToolButton *> buttons_;

    QGridLayout *grid_layout_;

    static QHash<int, std::set<int> > values_by_measure_;

    int visible_buttons_count_;

    bool grid_visible_;
};

#endif // VERTICALSPEEDWIDGET_H
