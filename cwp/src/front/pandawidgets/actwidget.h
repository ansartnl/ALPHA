#ifndef ACTWIDGET_H
#define ACTWIDGET_H

#include "BaseToolDialog.h"
#include "XAppSettings.h"

class ActTableModel;
class QAbstractItemDelegate;

namespace Ui
{
class ActWidget;
}

class ActWidget : public BaseToolDialog, public AppSettings
{
    Q_OBJECT

public:
    ActWidget(const QString &title, ActTableModel *model, QWidget *parent = 0);
    ~ActWidget();

public slots:
    void setItemDelegateForColumn(int column, QAbstractItemDelegate *delegate);

signals:
    void act();

protected:
    virtual void showEvent(QShowEvent *event);

    virtual void keyPressEvent(QKeyEvent *event);

    void UpdateXFont();
    void Translate();
    void SI();

private slots:
    void onActClicked();

    void updateSize();

private:
    void updateLanguage();

private:
    Q_DISABLE_COPY(ActWidget)

    Ui::ActWidget *ui;
    ActTableModel *model_;

};

#endif // ACTWIDGET_H
