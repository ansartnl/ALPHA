#include "XGridLabel.h"
#include "main.h"

#include <QSettings>

XGridItem::XGridItem()
	: m_Type(None), m_Row(0), m_Column(0), m_Alignment(Qt::AlignLeft)
	, m_RowSpan(1), m_ColumnSpan(1)
{
}

XGridLabel::XGridLabel()
	: m_Type(None), m_Foreground(Qt::black), m_Background(Qt::lightGray)
	, m_Transparent(25)
{

}

void XGridLabel::Load(TMapGrid &map)
{
        QSettings &settings = *globalSettings();
	QVariant var = settings.value("XMasterWindow/Settings/GridLabel");
	if(!var.isNull() && var.isValid())
	{
		QDataStream ds(var.toByteArray());
		ds >> map;
	}
}

void XGridLabel::Save(const TMapGrid &map)
{
	QByteArray arr;
	QDataStream ds(&arr, QIODevice::WriteOnly);
	ds << map;
        QSettings &settings = *globalSettings();
	settings.setValue("XMasterWindow/Settings/GridLabel", arr);
}

QDataStream& operator<< (QDataStream& out, const XGridItem& value)
{
	out << quint32(value.GetType());
	out << value.GetRow();
	out << value.GetColumn();
	out << quint32(value.GetAlignment());
	out << value.GetRowSpan();
	out << value.GetColumnSpan();
	return out;
}

QDataStream& operator>> (QDataStream& in, XGridItem& value)
{
	quint32 iVal;
	in >> iVal;
	value.SetType(XGridItem::LabelItemType(iVal));
	in >> iVal;
	value.SetRow(iVal);
	in >> iVal;
	value.SetColumn(iVal);
	in >> iVal;
	value.SetAlignment(Qt::Alignment(iVal));
	in >> iVal;
	value.SetRowSpan(iVal);
	in >> iVal;
	value.SetColumnSpan(iVal);
	return in;
}

QDataStream& operator<< (QDataStream& out, const XGridLabel& value)
{
	out << quint32(value.GetType());
	out << value.GetForeground();
	out << value.GetBackground();
	out << value.GetTransparent();
	out << value.GetMapItem();
	return out;
}

QDataStream& operator>> (QDataStream& in, XGridLabel& value)
{
	quint32 iVal;
	in >> iVal;
	value.SetType(XGridLabel::LabelType(iVal));
	QColor color;
	in >> color;
	value.SetForeground(color);
	in >> color;
	value.SetBackground(color);
	quint8 iShVal;
	in >> iShVal;
	value.SetTransparent(iShVal);
	XGridLabel::TMapItem map;
	in >> map;
	value.SetMapItem(map);
	return in;
}

QDataStream& operator<< (QDataStream& out, const XGridLabel::TMapItem& map)
{
	out << quint32(map.size());
	XGridLabel::TMapItem::ConstIterator it = map.end();
	XGridLabel::TMapItem::ConstIterator begin = map.begin();
	while (it != begin) {
		--it;
		out << quint32(it.key()) << it.value();
	}
	return out;
}

QDataStream& operator>> (QDataStream& in, XGridLabel::TMapItem& map)
{
	QDataStream::Status oldStatus = in.status();
	in.resetStatus();
	map.clear();

	quint32 n;
	in >> n;

	map.detach();
//	map.setInsertInOrder(true);
	for (quint32 i = 0; i < n; ++i) {
		if (in.status() != QDataStream::Ok)
			break;

		quint32 intKey(0);
		XGridLabel::TMapItem::mapped_type value;
		in >> intKey >> value;
		map.insert(static_cast<XGridLabel::TMapItem::key_type>(intKey), value);
	}
//	map.setInsertInOrder(false);
	if (in.status() != QDataStream::Ok)
		map.clear();
	if (oldStatus != QDataStream::Ok)
		in.setStatus(oldStatus);
	return in;
}

QDataStream& operator<< (QDataStream& out, const XGridLabel::TMapGrid& map)
{
	out << quint32(map.size());
	XGridLabel::TMapGrid::ConstIterator it = map.end();
	XGridLabel::TMapGrid::ConstIterator begin = map.begin();
	while (it != begin) {
		--it;
		out << quint32(it.key()) << it.value();
	}
	return out;
}

QDataStream& operator>> (QDataStream& in, XGridLabel::TMapGrid& map)
{
	QDataStream::Status oldStatus = in.status();
	in.resetStatus();
	map.clear();

	quint32 n;
	in >> n;

	map.detach();
//	map.setInsertInOrder(true);
	for (quint32 i = 0; i < n; ++i) {
		if (in.status() != QDataStream::Ok)
			break;

		quint32 intKey(0);
		XGridLabel::TMapGrid::mapped_type value;
		in >> intKey >> value;
		map.insert(static_cast<XGridLabel::TMapGrid::key_type>(intKey), value);
	}
//	map.setInsertInOrder(false);
	if (in.status() != QDataStream::Ok)
		map.clear();
	if (oldStatus != QDataStream::Ok)
		in.setStatus(oldStatus);
	return in;
}
