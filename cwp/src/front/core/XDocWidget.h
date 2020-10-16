#ifndef XDOCWIDGET_H
#define XDOCWIDGET_H

#include "front/common/XAppSettings.h"

#include <QThread>
#include <QDockWidget>
#include <QShortcut>
#include <QUuid>

class Dao;

class Titlebar : public QWidget
{
    private:
        QWidget *parent;
        QPoint cursor;

    public:
        Titlebar( QWidget *parent ) ;
    protected:
        virtual void mouseDoubleClickEvent(QMouseEvent *event);

};

//! Class containes panels
class XDocWidget: public QDockWidget, public AppSettings
{
    Q_OBJECT
public:
    enum DocType
    {
        ListACC, ListAPP, ListSIL_ACC, ListSIL_APP_ARR, ListSIL_APP_DEP,
        MeteoRunwayInfo, MeteoRunwayInfo2,RestrictionsUserEditor, RestrictionsAdminEditor,
        Runway, FPLsList, StandList, ListActIn, ListActOut, ListActDep,
        CtrlWindow, ListDailyPlan, UserPicsList, ListCost
    };

    //! Interface to CXDocWidget
    class IDocHandler
    {
    public:
        virtual ~IDocHandler()
        {
        }
        virtual void CloseDoc(XDocWidget::DocType type) = 0;
    };

    //! Constructor
    /*!
         \param type a document type
         \param pParent a handler
         \param parent a parent widget
         \param f a widget's flags
         */
    explicit XDocWidget(DocType type, IDocHandler* pParent, QWidget* parent = 0, Qt::WindowFlags f = 0, const QString& customData = QString());
    explicit XDocWidget(DocType type, QString privileges, IDocHandler* pParent, QWidget* parent = 0, Qt::WindowFlags f = 0, const QString& customData = QString());
    explicit XDocWidget(DocType type, QStringList _lst, IDocHandler* pParent, QWidget* parent = 0, Qt::WindowFlags f = 0, const QString& customData = QString());
    virtual ~XDocWidget();

    virtual void Refresh();

signals:
    void startRunwaySync();
    void getPickCoord(bool bMulti, QUuid id);
    void UpdateMeteoToMaster();
    void UpdateMeteo();

public slots:
    void OnPickCoord(QUuid id);

protected:
    //@{
    //! Inherited from QDockWidget
    virtual void resizeEvent(QResizeEvent* event);
    virtual void enterEvent(QEvent* event);
    virtual void leaveEvent(QEvent* event);
    virtual void closeEvent(QCloseEvent* event);
    virtual void showEvent(QShowEvent* event);
    virtual bool event(QEvent* event);
    //@}
    //@{
    //! Interface CXAppSettings
    virtual void Translate();
    virtual void UpdateXFont()
    {
    }
    virtual void SI()
    {
    }
    //@}

public slots:
    void OnRunwayMinimizeToggle();

private slots:
    void OnChildSizeChanged();
    void OnTopLevelChanged(bool topLevel);
    void OnGetPickCoord(bool bMulti);
    void OnUpdateMeteoToMaster();

private:
    void CalcVisibleRect();
    void Init(IDocHandler* pParent);

private:
    IDocHandler* m_pParent;
    DocType m_Type;
    bool m_bHover;
    bool m_bMinimized;
    bool isFirstShow;
    QThread workerThread;
    QSize mRunwaySize;
    QUuid mPickCoordID;
    QString m_privileges;
    QStringList lst;
};

#endif // XDOCWIDGET_H
