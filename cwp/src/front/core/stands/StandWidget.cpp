#include "StandWidget.h"
#include "StandModel.h"

#include <QTableView>
#include <QVBoxLayout>
#include <QHeaderView>

StandWidget::StandWidget(QWidget* pParent)
    : QWidget(pParent), mView(new QTableView(this)), mModel(new StandModel(this))
{
    mSortingEnable = true;

    mView->setAutoFillBackground(false);
    mView->viewport()->setAutoFillBackground(false);

    mView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mView->setAlternatingRowColors(false);
    mView->setSelectionMode(QAbstractItemView::SingleSelection);
    mView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mView->setSortingEnabled(mSortingEnable);

    mView->horizontalHeader()->setMinimumWidth(10);
    mView->horizontalHeader()->setDefaultSectionSize(70);
    mView->horizontalHeader()->setCascadingSectionResizes(false);
    mView->horizontalHeader()->setSortIndicatorShown(mSortingEnable);
    mView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    mView->horizontalHeader()->setMovable(false);
    mView->horizontalHeader()->setClickable(true);
    mView->horizontalHeader()->setHighlightSections(false);

    mView->verticalHeader()->setDefaultSectionSize(20);
    mView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    mView->verticalHeader()->setClickable(false);
    mView->verticalHeader()->hide();

    mView->setModel(mModel);

    if(mSortingEnable)
        mView->sortByColumn(0, Qt::AscendingOrder);

    UpdateXFont();

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(mView);
    setLayout(layout);
    adjustSize();
    SizeToContents();
}

StandWidget::~StandWidget()
{

}

void StandWidget::SetModelData(const QMap<quint32, QSharedPointer<GroundStand> >& pListData)
{
    mModel->SetModelData(pListData.values());
    mView->setSortingEnabled(mSortingEnable);
    SizeToContents();
}

void StandWidget::SetModelData(const QSharedPointer<GroundStand>& pData)
{
    mModel->SetModelData(pData);
    mView->setSortingEnabled(mSortingEnable);
    SizeToContents();
}

void StandWidget::SizeToContents(bool bSend/* = true*/)
{
    mView->resizeColumnsToContents();
    if (bSend)
        emit sizeChanged();
}

const QSize StandWidget::GetVisibleSize() const
{
    int iWidth(mView->horizontalHeader()->length()), iHeight(mView->verticalHeader()->length());
    iWidth += mView->verticalHeader()->isVisible() ? mView->verticalHeader()->width() : 0;
    iHeight += mView->horizontalHeader()->height();
#ifdef Q_OS_LINUX
    iWidth += 8;
    iHeight += 42;
#endif
    return QSize(iWidth + 4, iHeight + 4);
}

void StandWidget::UpdateXFont()
{
    QFont font1 = mView->font();
    font1.setPointSize(AppSettings::GetTextSize().GetCurrentTextSize());
    mView->setFont(font1);
}
