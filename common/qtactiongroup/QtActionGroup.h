#ifndef _QT_ACTION_GROUP_H__
#define _QT_ACTION_GROUP_H__

#include <QtGui/QActionGroup>


//! Qt extention of native action group class.
/*! Class lets to define exclusive mode with the possibility to
    uncheck the last checked action. This mode has name as lite
    exclusive.
*/
class QtActionGroup : public QActionGroup
{
    Q_OBJECT
    Q_PROPERTY(bool liteExclusive READ isLiteExclusive WRITE setLiteExclusive)

public:
    explicit QtActionGroup(QObject* parent);

    bool isLiteExclusive() const;

public Q_SLOTS:
    //! Sets exclusive mode with the possibility to take it off.
    /*! After lite mode is set real exclusive mode becomes off.
        Only one mode is possible to use: exclusive or liteExclusive.
    */
    void setLiteExclusive(bool);

private Q_SLOTS:
    void on_triggered(QAction *);

private:
    Q_DISABLE_COPY(QtActionGroup)
    bool    mIsLiteExclusive;
};

#endif // _QT_ACTION_GROUP_H__
