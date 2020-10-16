#include "actwidget.h"
#include "ui_actwidget.h"

#include <QKeyEvent>
#include "acttablemodel.h"

ActWidget::ActWidget(const QString &title, ActTableModel *model, QWidget *parent) :
    BaseToolDialog(false, true, parent),
    ui(new Ui::ActWidget),
    model_(model)
{
    ui->setupUi(this);
    updateLanguage();

    ui->tvActIn->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tvActIn->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    ui->tvActIn->setModel(model_);

    connect(ui->pbClose, SIGNAL(clicked()), this, SLOT(hide()));
    connect(ui->pbAct, SIGNAL(clicked()), this, SLOT(onActClicked()));

    connect(ui->tvActIn->horizontalHeader(), SIGNAL(geometriesChanged()), SLOT(updateSize()), Qt::QueuedConnection);
    connect(ui->tvActIn->verticalHeader(), SIGNAL(geometriesChanged()), SLOT(updateSize()), Qt::QueuedConnection);

    setWindowTitle(title);
}

ActWidget::~ActWidget()
{
    delete ui;
}

void ActWidget::setItemDelegateForColumn(int column, QAbstractItemDelegate *delegate)
{
    ui->tvActIn->setItemDelegateForColumn(column, delegate);
}

void ActWidget::showEvent(QShowEvent *event)
{
    QModelIndex index = ui->tvActIn->model()->index(0, 0, QModelIndex());
    ui->tvActIn->edit(index);
}

void ActWidget::keyPressEvent(QKeyEvent *event)
{
    if ((Qt::Key_Enter == event->key()) || (Qt::Key_Return == event->key())) {
        onActClicked();
        return;
    }

    QWidget::keyPressEvent(event);
}

void ActWidget::UpdateXFont()
{
    staticMetaObject.invokeMethod(this, "updateSize", Qt::QueuedConnection);
}

void ActWidget::Translate()
{
}

void ActWidget::SI()
{
}

void ActWidget::onActClicked()
{
    if (!model_->checkData(0))
        return;
    emit act();
    hide();
}

void ActWidget::updateSize()
{
    int iHeight(ui->tvActIn->verticalHeader()->length());
    iHeight += ui->tvActIn->horizontalHeader()->height();
    iHeight += ui->pbAct->size().height();

    setMinimumHeight(iHeight);
    setMaximumHeight(iHeight);
}

void ActWidget::updateLanguage()
{
    ui->pbClose->setText(tr("CLOSE"));
    ui->pbAct->setText(tr("ACT"));
}
