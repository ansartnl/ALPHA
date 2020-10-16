#ifndef FLIGHTLEVELWIDGET_H
#define FLIGHTLEVELWIDGET_H

#include <set>
#include <memory>
#include <functional>

#include <QFrame>
#include <QHash>
#include <QSet>
#include <QSharedPointer>

#include "ui_FlightLevelWidget.h"

class QToolButton;
class QGridLayout;
class QSpacerItem;


/**
    @class FlightLevelWidget
    @brief Виджет - эшелонатор.
*/
class FlightLevelWidget: public QFrame
{
	Q_OBJECT

	static const QString kWhiteBackgroundStyle;
	static const QString kOrangeBackgroundStyle;
	static const QString kGrayBackgroundStyle;
	static const QString kBorderStyle;

public:
    /**
        Перечисление единиц измерений эшелонов.
    */
    enum Measure {
        METERS = 0,
        FEET,

        MEASURE_COUNT
    };

    struct FlightLevel
    {
        QString name;
        int echelon;
        int meters;
        int feet;
    };

    static const int kInvalidValue = 0;

    /**
        Конструктор.
        @param acid идентификатор воздушного судна.
        @param type тип эшелонатора.
        @param values список значений эшелонов.
        @param measure единица измерений эшелонов.
        @param parent указатель на родительский объект.
        @param flags оконные свойства виджета.
    */
    FlightLevelWidget(const QString &acid, const QString &type,
                      QWidget *parent = 0, Qt::WindowFlags flags = Qt::Popup);

    /**
        Метод устанавливает/сбрасывает отображение границ кнопок эшелонов.
        @param grid признак отображения границ кнопок.
    */
    void setGrid(bool grid_visible = true);

    void setTflMarkerVisible(bool visible);
    void setTflMarkerChecked(bool checked);
    bool isTflMarkerChecked() const;

    /**
        Метод задает количество видимых эшелонов в первой колонке на экране.
        @param count количество эшелонов.
    */
    bool setCountVisibleButtons(int count);
    /**
        Метод устанавливает идентификатор воздушного судна.
        @param acid идентификатор воздушного судна.
    */
    void setACID(const QString &acid);
    /**
        Метод устанавливает тип эшелонатора.
        @param type тип эшелонатора.
    */
    void setType(const QString &type);
    /**
        Метод задает эшелон отображаемый на экране при вызове эшелонатора или
        заполняет текстовое поле заданным значением эшелона, если такой эшелон не найден.
        @param sValue значение эшелона.
    */
    bool setVisibleValue(const QString &visible_value);
    QPoint buttonPosition(const QString &button_value);
    /**
        Метод задает цвет для заданного значения эшелона.
        @param sValue значение эшелона.
        @param foregroundColor цвет надписи значения эшелона на кнопке.
        @param backgroundColor цвет кнопки с эшелоном.
    */
    bool setValueColor(const QString &value, const QColor &foreground_color, const QColor &background_color);
    /**
        Метод задает цвет в указанных границах значений эшелонов.
        @param valueMin нижняя граница значения эшелона.
        @param valueMax верхняя граница значения эшелона.
        @param measure единицы измерения.
        @param foregroundColor цвет надписи значения эшелона на кнопке.
        @param backgroundColor цвет кнопки с эшелоном.
    */
    void setRangeColor(int valueMin, int valueMax, Measure measure, const QColor &foregroundColor,
                       const QColor &backgroundColor);
    /**
        Метод задает список значений для эшелонатора и единицы измерения этих значений.
        @param values список значений эшелонов.
        @param measure единицы измерения.
    */
    void setValues(const QList<FlightLevel> &values, Measure measure = METERS);

    int value(Measure measure) const;

    void setFocusOnCustomEdit() const;

Q_SIGNALS:
    /**
        Сигнал генерируется при скрытии виджета по причине выбора эшелона.
    */
	void accepted(double value_feet, double value_meter, int measure);
    /**
        Сигнал генерируется при скрытии виджета по причине невыбора эшелона.
    */
    void rejected();
    /**
        Сигнал генерируется при наведении на эшелон.
    */
    void valueEnter(const QString &flight_level);

    void tflMarkerChecked();

protected:

    virtual bool eventFilter(QObject *obj, QEvent *event);

    virtual void showEvent(QShowEvent *event);

    virtual void closeEvent(QCloseEvent *event);

private Q_SLOTS:
    /**
        Метод вызывается при нажатии на кнопку эшелона.
    */
    void toolClicked();

    void on_tbRemove_clicked();
    /**
        Метод вызывается при завершении ввода эшелона.
    */
    void on_customValue_editingFinished();

    void onMarkerStateChanged(int state);

private:
    QList<FlightLevel> loadFlightLevels();

private:
    Q_DISABLE_COPY(FlightLevelWidget)

    /**
        Инициализация эшелонатора.
    */
    void init();
    /**
        Указатель на окно.
    */
    QSharedPointer<Ui::FlightLevelWidget> ui;
    /**
        Признак первого отображения виджета на экране.
    */
    bool first_show_;
    /**
        Список кнопок эшелонов.
    */
    QSet<QToolButton *> buttons_;
    /**
        Указатель на класс для выравнивания эшелонов.
    */
    QSpacerItem *verticalSpacer1;
    QSpacerItem *verticalSpacer2;
    /**
        Словарь значений эшелонов для разных единиц измерений.
    */
    QList<FlightLevel> flight_levels_;
    /**
        Количество видимых эшелонов в первой колонке эшелонатора на экране.
    */
    int visible_buttons_count_;
    /**
        Признак отображения границ кнопок эшелонов.
    */
    bool grid_visible_;

    int value_in_meters_;
    int value_in_feets_;
};

#endif // FLIGHTLEVELWIDGET_H
