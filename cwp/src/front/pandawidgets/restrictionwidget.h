#ifndef RESTRICTIONWIDGET_H
#define RESTRICTIONWIDGET_H

#include <vector>

#include <QtGui>

#include "Restriction.h"
#include "restrictionlineedit.h"
#include "BaseToolDialog.h"
#include <QtSql/QSqlRelationalTableModel>
#include <QLineEdit>

class PandaPoint;
class PandaItemBase;
class RestrListWidgetItem;
class QListWidgetItem;
class QLineEdit;
class QComboBox;
class QCompleter;

namespace Ui
{
class RestrictionWidget;
}

/**
    @class RestrictionWidget
    @brief Виджет для работы с ограничениями.
*/
class RestrictionWidget : public BaseToolDialog
{
    Q_OBJECT

    enum PointType {
        NAMETYPE = 0,
        GEOTYPE
    };

    enum RestrictionPages {
        POLYGON_PAGE = 0,
        POLYLINE_PAGE,
        CIRCLE_PAGE
    };

    enum RestrictionParametersPages
    {
        EMPTY_PAGE = 0,
        FILLED_PAGE
    };

public:

    enum RestrictionType {
        POLYGON = 0,
        CIRCLE
    };

    explicit RestrictionWidget(QWidget *parent = 0);

    ~RestrictionWidget();


public slots:

    void setCompleter(const QStringList &completions);

    //void itemRouteListReset(const Routes &routes);
    //void itemOrpListReset(const Orps &orps);
    void itemListReset(const QList<QSharedPointer<CRestrictionUser> > &restrictions);
    void itemAdded(const QSharedPointer<CRestrictionUser> &restriction);
    void itemModified(const QSharedPointer<CRestrictionUser> &restriction);
    void itemDeleted(const QUuid &uid);

    void internalPikerModeOff();

    void onMouseMove(const Qt::MouseButton &button, const PandaPoint &point, const QSet<PandaItemBase *> &itemsList);
    void onMousePress(const Qt::MouseButton &button, const PandaPoint &point, const QSet<PandaItemBase *> &itemsList);
    void onMouseRelease(const Qt::MouseButton &button, const PandaPoint &point, const QSet<PandaItemBase *> &items_list);

    void onDbConnectionChanged(bool connected);

protected slots:
    void closeEvent(QCloseEvent *event);

private slots:

    void enableUpdate();

    void currentRouteItemChanged(QListWidgetItem *current, QListWidgetItem *prev);
    void routeFilterChanged(const QString &str);

    void toUpper(const QString &text);

    void createRestriction();
    void updateRestriction();
    void deleteRestriction();

    void nextLinePageClicked();
    void prevLinePageClicked();

    void firstLinePointClicked(int checked);
    void secondLinePointClicked(int checked);

    void firstLineStartTypeChanged(int currentIndex);
    void firstLineEndTypeChanged(int currentIndex);
    void secondLineStartTypeChanged(int currentIndex);
    void secondLineEndTypeChanged(int currentIndex);

    void setTextForLine(int type, int index, QLineEdit *line);

    void addClicked();
    void deleteClicked();
    void upButtonClicked();
    void downButtonClicked();
    void showButtonClicked();
    void hideButtonClicked();
    void OnChangeSelect(const QItemSelection &itsnew, const QItemSelection & its);

    void pointTypeChanged(int typeIndex, const CCoord &coord, QLineEdit *lineEdit);
    void changed(int);

    void lineStartFirstPointTypeChanged(int currentIndex);
    void lineEndFirstPointTypeChanged(int currentIndex);
    void lineStartSecondPointTypeChanged(int currentIndex);
    void lineEndSecondPointTypeChanged(int currentIndex);

    void circleCenterPointTypeChanged(int currentIndex);
    void ellipseCenterPointTypeChanged(int currentIndex);



    void pointValueEditingFinished(int currentIndex, CCoord &coord, QLineEdit *lineEdit);

    void on_leFirstPolyEditEditingFinished();

    void onleLineStartPointEditingFinished01();
    void onleLineEndPointEditingFinished01();
    void onleLineStartPointEditingFinished02();
    void onleLineEndPointEditingFinished02();

    void onleFirstLineLowerHeightEditingFinished();
    void onleFirstLineUpperHeightEditingFinished();

    void onleSecondLineLowerHeightEditingFinished();
    void onleSecondLineUpperHeightEditingFinished();

    void onGlobalStartDateChange(const QDate &date);
    void onGlobalEndDateChange(const QDate &date);

    void onGlobalStartTimeChange(const QTime &time);
    void onGlobalEndTimeChange(const QTime &time);

    void onFirstLineStartTime(const QTime &time);
    void onFirstLineEndTime(const QTime &time);
    void onSecondLineStartTime(const QTime &time);
    void onSecondLineEndTime(const QTime &time);

    /**
        Метод вызывается при завершении ввода центральной точки окружности.
    */
    void onleCircleCenterEditingFinished();

    /**
        Метод вызывается при завершении ввода радиуса окружности.
    */
    void onleCircleRadiusEditingFinished();

    void onEllipseCenterEditingFinished();
    void onEllipseBigAxisEditingFinished();
    void onEllipseSmallAxisEditingFinished();
    void onEllipseAngleEditingFinished();


    void pbPickToggled(bool checked);


    void on_leRestrictionTitle_editingFinished();
    void on_cbRestrictionActual_stateChanged(int state);
    void on_cbRestrictionFavorites_stateChanged(int state);

    void currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void onRestrictionTypeChanged(int current_index);
    void onRestrictionShapeChanged(int current_index);


    void onAddRouteRestriction();
    void onDeleteRouteRestriction();
    void updateRouteRestrictionList();

signals:
    void createRestriction(const QSharedPointer<CRestrictionUser> &restriction);
    void updateRestriction(const QSharedPointer<CRestrictionUser> &restriction);
    void deleteRestriction(const QSharedPointer<CRestrictionUser> &restriction);

    void pickerMode(bool aciv);

    void updateGraphicsObject(const CRestrictionUser &restriction);
    void hideGraphicsObject(const CRestrictionUser &restriction);

private:

    void updateLanguage();

    void setGeneralParamForRestriction();

    void showControls(bool visible);

    void polylineEditEditingFinished(int index, int polylinePointTypeIndex, QLineEdit *lineEdit);

    void lineSetLowerHeight(int index, QLineEdit *lineEdit);
    void lineSetUpperHeight(int index, QLineEdit *lineEdit);

    void lineSetStartTime(int index, const QTime &timeEdit);
    void lineSetEndTime(int index, const QTime &timeEdit);

    void polyPointClicked(int checked, int index, QComboBox *comboBox, QLineEdit *lineEdit);

    void initRestrictionUserPoint(int i, int type, const CRestrictionUser::TPoint &point);

    void initLinePointControls(int i, CRestrictionUser::TPoint &point, bool isActiv);

    void polyPointTypeChanged(int typeIndex, int pointIndex, QLineEdit *lineEdit);

    void findCoordsByPointName(const QString &pointName, CCoord &coord);

    bool findCoordsByPointName(const QString &pointName, qreal &latitude, qreal &longitude);


    void updateGraphics();
    void hideGraphics();


    void clearAllForm();
    void clearLineRestrForm();
    void clearCircleRestrForm();
    void clearEllipseRestrForm();
    void clearRouteSectionRestrForm();

    static void clearDataItemPair(CRestrictionUser *restriction, QVector<bool> &activ);

    static CCoord stringToCoord(const QString &str);

    bool updateRestrictionInList(const QSharedPointer<CRestrictionUser> &restriction);

    void setConnectionsToEnableUpdate();

private:

    Ui::RestrictionWidget *ui;

    bool new_restriction_;

    //Routes routes_;
    //Orps orps_;

    QSharedPointer<CRestrictionUser> restriction_;
    QVector<bool> active_point_;

    bool pick_mode_;
    bool mouse_press_;

    unsigned current_line_index_;

    QCompleter *completer_;
};

#endif // RESTRICTIONWIDGET_H
