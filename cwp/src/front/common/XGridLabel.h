#ifndef XGRIDLABEL_H
#define XGRIDLABEL_H

#include <QtGui/QColor>
#include <QtCore/QMap>

//! Class stores position of label item
/*!
    In order to add additional item type it is required to modify
    \b LabelItemType enumeration (add type in the end of enum).
    Then new item should be associated with the label. It is implemented
    in the XLabelConstructor.

    Lets look at the example what needs to be done to insert new item
    into formular:
    \code
        CXLabelConstructor::CXLabelConstructor(..)
        {
            ...

            createLabelItem(CXGridItem::MyItemType, defaultPoint, maxColumnWidth,
                            tr("This is a tooltip in the editor for my label item"));

            ...
        }

        // And that is it - nothing more need to be added.
    \endcode

    Of cause we need to add it to the AirplaneLabel (GUI representation of new
    item in the drawable formular).

    So need to be done 2 things (lets look at the code):
    \code
        // 1. Add label into GUIAirplaneLabel

            // a. - Declare variable and initialize new item.
            void CGUIAirplaneLabel::initGrid()
            {
                ...

                m_itemMyItem = new CGUILabelText(this);
                m_itemMyItem->SetReadOnly(false);
                m_itemMyItem->setText(tr("MyItemType"));

                ...
            }

            // b. - Assign data with the CGUILabelText (add text to GUI item)
            void CGUIAirplaneLabel::loadTypeGrid(bool bFullShowVal)
            {
                ...

                if(labelMap.contains(XGridItem::MyItemType))
                {
                    m_itemMyItem->SetVisible(true);
                    m_itemMyItem->setVisible(true);
                    const XGridItem& itemPos = labelMap[XGridItem::MyItemType];
                    pGrid->addItem(m_itemMyItem, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                    m_itemMyItem->SetTextColor(m_colorForeground);
                }
                else
                {
                    m_itemMyItem->SetVisible(false);
                    m_itemMyItem->setVisible(false);
                }

                ...
            }

            // c. - Update information in the formular.
            // updateGrid() function calls one time per airplane new position update.
            void CGUIAirplaneLabel::updateGrid()
            {
                m_itemMyItem->setText(someText);
            }

            // d. - Use mousePressEvent, mouseReleaseEvent or mouseDoubleClickEvent for
            // handling click on the item

        // 2. Add data information that will be associated with new item (if necessary).
        // This can be done in the newly added functionality for airplane object or
        // accessed via airplane object explicitly.

    \endcode
*/
class XGridItem
{
  public:
    //! Supported types of items.
    enum LabelItemType
    {
        None = 0,
        ACID,
        CODE,
        LX,
        NS,
        AFL,
        CFL,
        TREND,
        VRC,
        HDG,
        BANK,
        NextPoint,
        SPD1,
        SPD2,
        V,
        GOV,
        FUEL,
        TFL,
        HDG2,
        UVD,
        PEL,
        RFL,
        AZIMUTH,
        DISTANCE,
        QNH,

		SPD,
		GSP,
		W
    };

    //! Constructor
    XGridItem();
  public:
    const LabelItemType GetType() const
    {
      return m_Type;
    }
    void SetType(const LabelItemType type)
    {
      m_Type = type;
    }
    const qint32 GetRow() const
    {
      return m_Row;
    }
    void SetRow(const qint32 row)
    {
      m_Row = row;
    }
    const qint32 GetColumn() const
    {
      return m_Column;
    }
    void SetColumn(const qint32 column)
    {
      m_Column = column;
    }
    const Qt::Alignment GetAlignment() const
    {
      return m_Alignment;
    }
    void SetAlignment(Qt::Alignment alignment)
    {
      m_Alignment = alignment;
    }
    const qint32 GetRowSpan() const
    {
      return m_RowSpan;
    }
    void SetRowSpan(const qint32 row_span)
    {
      m_RowSpan = row_span;
    }
    const qint32 GetColumnSpan() const
    {
      return m_ColumnSpan;
    }
    void SetColumnSpan(const qint32 column_span)
    {
      m_ColumnSpan = column_span;
    }
  private:
    /** Label type */
    LabelItemType m_Type;
    /** Row index */
    qint32 m_Row;
    /** Column index */
    qint32 m_Column;
    /** Alignment relative to the borders */
    Qt::Alignment m_Alignment;
    /** Span number of rows */
    qint32 m_RowSpan;
    /** Span number of columns */
    qint32 m_ColumnSpan;
};


//! Container for one formular label items.
class XGridLabel
{
  public:
    //! Supported formular types.
    enum LabelType
    {
        None = 0,
        Controlled,
        ControlledShort,
        ControlledSelected,
        In,
        InShort,
        InSelected,
        Related,
        RelatedShort,
        RelatedSelected,
        Unrelated,
        UnrelatedShort,
		UnrelatedSelected
    };

    //! Definition of container for formular items
    typedef QMap<XGridItem::LabelItemType, XGridItem> TMapItem;
    //! Definition of container for all formular labels.
    typedef QMap<XGridLabel::LabelType, XGridLabel> TMapGrid;

    //! Constructor
    XGridLabel();

  public:
    //! Type of a formular.
    const LabelType GetType() const
    {
      return m_Type;
    }
    //! Define type for the formular.
    void SetType(const LabelType V)
    {
      m_Type = V;
    }

    //! Retrieve text color.
    const QColor GetForeground() const
    {
      return m_Foreground;
    }
    //! Set color of text.
    void SetForeground(const QColor &V)
    {
      m_Foreground = V;
    }
    //! Get current background color.
    const QColor GetBackground() const
    {
      return m_Background;
    }
    //! Set background color.
    void SetBackground(const QColor &V)
    {
      m_Background = V;
    }
    //! Retrieve formular transparency level.
    const quint8 GetTransparent() const
    {
      return m_Transparent;
    }
    //! Set transparency level.
    /*!
        \param V Transparency level [0 .. 100]. 0 - opaque.
    */
    void SetTransparent(const quint8 V)
    {
      m_Transparent = V;
    }
    //! Get access to formular items.
    const TMapItem &GetMapItem() const
    {
      return m_MapItem;
    }
    //! Get access to formular items.
    TMapItem &GetMapItem()
    {
      return m_MapItem;
    }
    //! Change formular items.
    void SetMapItem(const TMapItem &V)
    {
      m_MapItem = V;
    }

    //! Load settings for all formulars.
    static void Load(TMapGrid& map);
    //! Save settings of all formulars.
    static void Save(const TMapGrid& map);

  private:
    /** Type of a label (formular) */
    LabelType m_Type;
    /** Color of text */
    QColor m_Foreground;
    /** Color of background */
    QColor m_Background;
    /** Transparency value [0 .. 100]. 0 - opaque */
    quint8 m_Transparent;
    /** Container of items */
    TMapItem m_MapItem;
};

QDataStream& operator<<(QDataStream& out, const XGridItem& value);
QDataStream& operator>>(QDataStream& in, XGridItem& value);

QDataStream& operator<<(QDataStream& out, const XGridLabel& value);
QDataStream& operator>>(QDataStream& in, XGridLabel& value);

QDataStream& operator<<(QDataStream& out, const XGridLabel::TMapItem& map);
QDataStream& operator>>(QDataStream& in, XGridLabel::TMapItem& map);

QDataStream& operator<<(QDataStream& out, const XGridLabel::TMapGrid& map);
QDataStream& operator>>(QDataStream& in, XGridLabel::TMapGrid& map);

#endif // XGRIDLABEL_H
