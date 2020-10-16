#ifndef BASE_EDITOR__H
#define BASE_EDITOR__H

#include "RefreshInterface.h"
#include <QWidget>

class QSqlTableModel;

//! Base class for editors
class BaseEditor : public QWidget, public RefreshInterface
{
    Q_OBJECT
public:
    explicit BaseEditor(QWidget* parent = 0, Qt::WindowFlags f = 0);

public slots:
    void onFirChanged(int id, const QString &name);

protected:
    const QString firName() const;
    int firId() const;
    bool isFirValid() const;
    virtual void updateFirName();
    virtual void setFirId();
    bool selectAll(QSqlTableModel* model);

    //! Inherited from QWidget
    virtual bool eventFilter(QObject *obj, QEvent *event);

protected slots:
    virtual void onFind(QObject *obj);
    virtual void onFindNext(QObject *obj);

private:
    QString mCurrentFirName;
    int mCurrentFirId;
};

#endif // BASE_EDITOR__H
