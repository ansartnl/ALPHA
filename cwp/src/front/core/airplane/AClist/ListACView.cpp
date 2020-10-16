#include "ListACView.h"
#include "ListACHeaderView.h"
#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QMouseEvent>
#include "main.h"

CListACView::CListACView(QWidget *parent) :
	QTableView(parent)
{
    bool bSortingEnable = true;

	setAutoFillBackground(false);
	viewport()->setAutoFillBackground(false);
	//setMouseTracking(true);

    /*QPalette p = palette();
    p.setColor(QPalette::Highlight, Qt::transparent);
    p.setColor(QPalette::HighlightedText, Qt::black);
    setPalette(p);*/

	setEditTriggers(QAbstractItemView::SelectedClicked | QAbstractItemView::EditKeyPressed);
	setAlternatingRowColors(false);
    //setSelectionMode(QAbstractItemView::NoSelection);
    setSelectionMode(QAbstractItemView::SingleSelection);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSortingEnabled(bSortingEnable);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    CListACHeaderView* header = new CListACHeaderView(Qt::Horizontal, this);

    int widthColumn = networkSettings()->value("users/SizeListAC/widthColumn").toInt();
    setHorizontalHeader((QHeaderView*)header);
    horizontalHeader()->setMinimumWidth(10);
    horizontalHeader()->setMinimumHeight(20);
    horizontalHeader()->setDefaultSectionSize(widthColumn);
	horizontalHeader()->setCascadingSectionResizes(false);
	horizontalHeader()->setSortIndicatorShown(bSortingEnable);
    horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    horizontalHeader()->setMovable(false);
	horizontalHeader()->setClickable(true);
    horizontalHeader()->setHighlightSections(false);

    int heightRow = networkSettings()->value("users/SizeListAC/heightRow").toInt();
    if (!heightRow)
        heightRow = DEFAULT_MAX_ROW_HEIGHT;
    verticalHeader()->setDefaultSectionSize(heightRow);
	verticalHeader()->setResizeMode(QHeaderView::Fixed);
	verticalHeader()->setClickable(false);
	verticalHeader()->hide();

    UpdateXFont();
}

void CListACView::setPlanType(QString type)
{
    static_cast<CListACHeaderView*>(horizontalHeader())->setType(type);
}

void CListACView::setPlanHeaders(QStringList headers)
{
    static_cast<CListACHeaderView*>(horizontalHeader())->setHeaders(headers);
}

void CListACView::scroll(bool scrolled)
{
    static_cast<CListACHeaderView*>(horizontalHeader())->isScrolled = scrolled;
    emit scrollChanged(scrolled);
}

bool CListACView::isScrolled() const
{
    return static_cast<CListACHeaderView*>(horizontalHeader())->isScrolled;
}

void CListACView::leaveEvent(QEvent *event)
{
    emit enter_leaveEvent(false);
    QTableView::leaveEvent(event);
}

void CListACView::enterEvent(QEvent *event)
{
    emit enter_leaveEvent(true);
    QTableView::enterEvent(event);
}

void CListACView::keyReleaseEvent(QKeyEvent *event)
{
    QTableView::keyReleaseEvent(event);
    emit keyPressedEvent(currentIndex(), event);
}

void CListACView::mousePressEvent(QMouseEvent *event)
{
    QTableView::mousePressEvent(event);
    emit mouseEvent(currentIndex(), event);
}

void CListACView::UpdateXFont()
{
    QFont font1 = font();
    font1.setPointSize(AppSettings::GetTextSize().GetCurrentTextSize());
    setFont(font1);
}
