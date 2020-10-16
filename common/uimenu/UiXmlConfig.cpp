#include "UiXmlConfig.h"
#include "UiItems.h"

#include "QtCore/QDebug"
#include <QtCore/QFile>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamWriter>


namespace Uim {

namespace UiXmlConfig {

const QString g_uimName = "uim";
const QString g_menuName = "menu";
const QString g_submenusName = "submenus";
const QString g_submenuName = "submenu";
const QString g_mainmenuName = "mainmenu";
const QString g_toolbarName = "toolbar";
const QString g_tabcontrolName = "tabcontrol";
const QString g_rightbarName = "rightbar";
const QString g_groupName = "group";
const QString g_itemName = "item";
const QString g_buttonName = "button";
const QString g_toolbuttonName = "toolbutton";
const QString g_itemAttributeVersion = "version";
const QString g_itemAttributeText = "text";
const QString g_itemAttributeName = "name";
const QString g_itemAttributeToolTip = "toolTip";
const QString g_itemAttributeSeparator = "separator";
const QString g_itemAttributeStyle = "style";
const QString g_itemAttributeSubmenu = "submenu";
const QString g_itemAttributeRow = "row";
const QString g_itemAttributeColumn = "column";
const QString g_itemAttributeRowSpan = "rowSpan";
const QString g_itemAttributeAlignment = "alignment";
const QString g_itemAttributeColumnSpan = "columnSpan";
const QString g_itemAttributeIconSize = "iconSize";
const QString g_itemAttributeSizePolicy = "sizePolicy";
const QString g_itemValueStyleIcO = "IconOnly";
const QString g_itemValueStyleTeO = "TextOnly";
const QString g_itemValueStyleTeBI = "TextBesideIcon";
const QString g_itemValueStyleTeUI = "TextUnderIcon";
const QString g_itemValueSizePolicyFixed = "fixed";
const QString g_itemValueSizePolicyMinimum = "minimum";
const QString g_itemValueTrue = "true";
const QString g_itemValueFalse = "false";
const QString g_itemValueAlignLeft = "AlignLeft";
const QString g_itemValueAlignRight = "AlignRight";
const QString g_itemValueAlignHCenter = "AlignHCenter";
const QString g_itemValueAlignTop = "AlignTop";
const QString g_itemValueAlignBottom = "AlignBottom";
const QString g_itemValueAlignVCenter = "AlignVCenter";
const QString g_itemValueAlignCenter = "AlignCenter";



//! Configuration file reader.
class XmlReader
{
    typedef QMap<QString, QString>  AttritutesMap;

public:
    XmlReader(const QString &path, const QString &nameFirstNode, const TGroupItem &groupItem)
    {
        QFile xmlFile(path);
        if ( !xmlFile.open(QFile::ReadOnly | QFile::Text) )
            throw (QObject::tr("Unable to open UI configuration file ") + "\"%1\"").arg(path);

        QXmlStreamReader xml(&xmlFile);

        bool isUiSupported = false;

        while ( !xml.atEnd() && !xml.hasError() )
        {
            // Read next element.
            QXmlStreamReader::TokenType token = xml.readNext();
            // If token is just StartDocument, we'll go to next.
            if ( token == QXmlStreamReader::StartDocument )
                continue;

            // If token is StartElement, we'll see if we can read it.
            if(token == QXmlStreamReader::StartElement)
            {
                QString xmlName = xml.name().toString().toLower();
                if ( xmlName == g_uimName )
                {
                    AttritutesMap xmlMap;
                    QXmlStreamAttributes attributes = xml.attributes();
                    parseAttributes(attributes, xmlMap);

                    QString valueVersion = xmlMap[g_itemAttributeVersion];
                    if ( valueVersion == QString("1.0") )
                        isUiSupported = true;
                }
                // If this is a "nameFirstNode" we'll dig the information from there.
                else if ( xmlName == nameFirstNode )
                {
                    if ( !isUiSupported )
                        throw QObject::tr("Ui configuration file has incorrent structure or version of a file is not supported.");

                    // Make sure we do not parse similar xml node one more time.
                    // The last node that has nameFirstNode is used in the model.
                    groupItem->clear();

                    read(xml, groupItem, nameFirstNode);

                    // We have read all we need.
                    break;
                }
            }
        }
        if ( xml.hasError() )
            throw xml.errorString();
    }

private:
    void parseAttributes(QXmlStreamAttributes &attributes, AttritutesMap &stringMap)
    {
        foreach ( const QXmlStreamAttribute &attribute, attributes )
        {
            stringMap[attribute.name().toString().toLower()] = attribute.value().toString();
        }
    }

    void read(QXmlStreamReader &xml, TGroupItem groupItem, const QString &endElementName)
    {
        // Next element...
        xml.readNext();

        QString xmlName = xml.name().toString().toLower();

        // We'll continue the loop until we hit an EndElement named "endElementName".
        while ( !xml.atEnd() )
        {
            xmlName = xml.name().toString().toLower();

            if ( xml.tokenType() == QXmlStreamReader::EndElement &&
                 xmlName == endElementName )
                break;
            else if ( xml.tokenType() == QXmlStreamReader::StartElement )
            {
                AttritutesMap xmlMap;
                QXmlStreamAttributes attributes = xml.attributes();
                parseAttributes(attributes, xmlMap);

                // <group> or <submenu>
                if ( xmlName == g_groupName || xmlName == g_submenuName )
                {
                    QString valueText = xmlMap[g_itemAttributeText];
                    if ( valueText.isEmpty() )
                        qDebug() << QObject::tr("Node <%1> does not have an attribute \"%2\"").arg(xmlName).arg(g_itemAttributeText);
                    else
                    {
                        TGroupItem groupItemNew = groupItem->addGroup(valueText);
                        read(xml, groupItemNew, xmlName);
                    }
                }
                // <item> or <button> or <toolbutton>
                else if ( xmlName == g_itemName || xmlName == g_buttonName || xmlName == g_toolbuttonName )
                {
                    QString valueSeparator = xmlMap[g_itemAttributeSeparator];
                    QString valueText = xmlMap[g_itemAttributeText];
                    QString valueName = xmlMap[g_itemAttributeName];
                    QString valueToolTip = xmlMap[g_itemAttributeToolTip.toLower()];
                    QString valueStyle = xmlMap[g_itemAttributeStyle].toLower();
                    QString valueSubMenu = xmlMap[g_itemAttributeSubmenu].toLower();

                    if ( !valueSeparator.isEmpty() && valueSeparator == g_itemValueTrue )
                        groupItem->addSeparator();
                    else
                    {
                        if ( valueName.isEmpty() )
                        {
                            qDebug() << QObject::tr("Node <%1> does not have an attribute \"%2\"")
                                    .arg(xmlName).arg(g_itemAttributeName);
                        }
                        else
                        {
                            if ( xmlName != g_toolbuttonName )
                                groupItem->addAction(valueText, valueName, toActionStyle(valueStyle),
                                                     valueSubMenu == g_itemValueTrue, valueToolTip);
                            else
                            {
                                QString valueRow = xmlMap[g_itemAttributeRow];
                                QString valueColumn = xmlMap[g_itemAttributeColumn];
                                QString valueRowSpan = xmlMap[g_itemAttributeRowSpan.toLower()];
                                QString valueColumnSpan = xmlMap[g_itemAttributeColumnSpan.toLower()];
                                QString valueIconSize = xmlMap[g_itemAttributeIconSize.toLower()];
                                QString valueSizePolicy = xmlMap[g_itemAttributeSizePolicy.toLower()].toLower();
                                QString valueAlignment = xmlMap[g_itemAttributeAlignment].toLower();

                                int rowSpan = valueRowSpan.isEmpty() ? 1 : valueRowSpan.toInt();
                                int columnSpan = valueColumnSpan.isEmpty() ? 1 : valueColumnSpan.toInt();

                                groupItem->addToolbutton(valueText, valueName, toActionStyle(valueStyle),
                                                         valueSubMenu == g_itemValueTrue,
                                                         valueRow.toInt(), valueColumn.toInt(),
                                                         valueIconSize.toInt(), toSizePolicy(valueSizePolicy),
                                                         rowSpan, columnSpan, toAlignment(valueAlignment),
                                                         valueToolTip);
                            }
                        }
                    }
                }
            }

            // ...and next...
            xml.readNext();
        }
    }

    ActionItem::Style toActionStyle(const QString &value) const
    {
        if ( value == g_itemValueStyleIcO.toLower() )
            return ActionItem::IconOnly;
        else if ( value == g_itemValueStyleTeO.toLower() )
            return ActionItem::TextOnly;
        else if ( value == g_itemValueStyleTeBI.toLower() )
            return ActionItem::TextBesideIcon;
        else if ( value == g_itemValueStyleTeUI.toLower() )
            return ActionItem::TextUnderIcon;

        return ActionItem::TextBesideIcon;
    }

    ToolbuttonItem::Alignment toAlignment(const QString &value) const
    {
        if ( value == g_itemValueAlignLeft.toLower() || value.isEmpty() )
            return ToolbuttonItem::AlignLeft;
        else if ( value == g_itemValueAlignRight.toLower() )
            return ToolbuttonItem::AlignRight;
        else if ( value == g_itemValueAlignHCenter.toLower() )
            return ToolbuttonItem::AlignHCenter;
        else if ( value == g_itemValueAlignTop.toLower() )
            return ToolbuttonItem::AlignTop;
        else if ( value == g_itemValueAlignBottom.toLower() )
            return ToolbuttonItem::AlignBottom;
        else if ( value == g_itemValueAlignVCenter.toLower() )
            return ToolbuttonItem::AlignVCenter;
        else if ( value == g_itemValueAlignCenter.toLower() )
            return ToolbuttonItem::AlignCenter;
        return ToolbuttonItem::AlignLeft;
    }

    ToolbuttonItem::SizePolicy toSizePolicy(const QString &value) const
    {
        if ( value == g_itemValueSizePolicyFixed.toLower() )
            return ToolbuttonItem::Fixed;
        else if ( value == g_itemValueSizePolicyMinimum.toLower() )
            return ToolbuttonItem::Minimum;

        return ToolbuttonItem::Fixed;
    }
};


//! Configuration file writer.
class XmlWriter
{
public:
    XmlWriter(const QString &path, const UiModel &model)
        : mModel(model)
    {
        QFile xmlFile(path);
        if ( !xmlFile.open(QFile::WriteOnly | QFile::Text) )
            throw (QObject::tr("Unable to create UI configuration file ") + "\"%1\"").arg(path);

        QXmlStreamWriter stream(&xmlFile);

        stream.setAutoFormatting(true);
        stream.writeStartDocument();

        write(stream);

        stream.writeEndDocument();
    }

private:
    void write(QXmlStreamWriter &stream)
    {
        stream.writeStartElement( g_uimName );
        stream.writeAttribute(g_itemAttributeVersion, "1.0");

        {
            if ( mModel.mSubMenus->count() )
            {
                stream.writeStartElement( g_submenusName );
                const GroupItem::ItemsList &items = mModel.mSubMenus->children();
                foreach ( const TBaseItem &item, items )
                {
                    if ( item->isGroup() )
                        writeGroup( stream, g_submenuName, g_itemName, item.staticCast<GroupItem>() );
                }
                stream.writeEndElement(); // submenus
            }

            stream.writeStartElement( g_menuName );
            writeGroup( stream, g_groupName, g_itemName, mModel.mRootMenu, false );
            stream.writeEndElement(); // menu

            stream.writeStartElement( g_toolbarName );
            {
                stream.writeStartElement( g_mainmenuName );
                writeGroup( stream, g_groupName, g_itemName, mModel.mToolbarMenu, false );
                stream.writeEndElement(); // mainmenu

                stream.writeStartElement( g_tabcontrolName );
                writeGroup( stream, g_groupName, g_toolbuttonName, mModel.mToolbar, false );
                stream.writeEndElement(); // tabcontrol

                stream.writeStartElement( g_rightbarName );
                writeGroup( stream, g_groupName, g_buttonName, mModel.mRightBar, false );
                stream.writeEndElement(); // rightbar
            }
            stream.writeEndElement(); // toolbar
        }

        stream.writeEndElement(); // ui
    }

    void writeGroup(QXmlStreamWriter &stream, const QString &nameGroup,
                    const QString &nameItem, const TGroupItem &groupItem,
                    bool shouldCreateGroup = true)
    {
        Q_ASSERT(groupItem);

        if ( shouldCreateGroup )
        {
            stream.writeStartElement( nameGroup );
            if ( !groupItem->text().isEmpty() )
                stream.writeAttribute(g_itemAttributeText, groupItem->text());
        }

        const GroupItem::ItemsList &items = groupItem->children();
        foreach ( const TBaseItem &item, items )
        {
            if ( item->isGroup() )
                writeGroup( stream, g_groupName, nameItem, item.staticCast<GroupItem>() );
            else
            {
                if ( item->isSeparator() )
                {
                    stream.writeStartElement( nameItem );
                    stream.writeAttribute(g_itemAttributeSeparator, g_itemValueTrue);
                    stream.writeEndElement(); // g_itemName
                }
                else if ( item->isAction() )
                {
                    TActionItem actionItem = item.staticCast<ActionItem>();
                    Q_ASSERT(actionItem);

                    stream.writeStartElement( nameItem );
                    if ( !actionItem->mText.isEmpty() )
                        stream.writeAttribute(g_itemAttributeText, actionItem->mText);
                    stream.writeAttribute(g_itemAttributeName, actionItem->mName);
                    if ( actionItem->mIsSubMenuEnabled )
                        stream.writeAttribute(g_itemAttributeSubmenu, g_itemValueTrue);
                    stream.writeAttribute(g_itemAttributeStyle, fromActionStyle(actionItem->mStyle));
                    if ( actionItem->isToolbutton() )
                    {
                        TToolbuttonItem toolbuttonItem = actionItem.staticCast<ToolbuttonItem>();
                        stream.writeAttribute(g_itemAttributeRow, QString::number(toolbuttonItem->mRow));
                        stream.writeAttribute(g_itemAttributeColumn, QString::number(toolbuttonItem->mColumn));
                        if ( toolbuttonItem->mRowSpan > 1 )
                            stream.writeAttribute(g_itemAttributeRowSpan, QString::number(toolbuttonItem->mRowSpan));
                        if ( toolbuttonItem->mColumnSpan > 1 )
                            stream.writeAttribute(g_itemAttributeColumnSpan, QString::number(toolbuttonItem->mColumnSpan));
                        if ( toolbuttonItem->mAlignment != ToolbuttonItem::AlignLeft )
                            stream.writeAttribute(g_itemAttributeAlignment, fromAlignment(toolbuttonItem->mAlignment));
                        if ( toolbuttonItem->mIconSize > 0 )
                            stream.writeAttribute(g_itemAttributeIconSize, QString::number(toolbuttonItem->mIconSize));
                        if ( toolbuttonItem->mSizePolicy != ToolbuttonItem::Fixed )
                            stream.writeAttribute(g_itemAttributeSizePolicy, fromSizePolicy(toolbuttonItem->mSizePolicy));
                    }
                    if ( !actionItem->mToolTip.isEmpty() )
                        stream.writeAttribute(g_itemAttributeToolTip, actionItem->mToolTip);
                    stream.writeEndElement(); // g_itemName
                }
            }
        }

        if ( shouldCreateGroup )
            stream.writeEndElement(); // "name"
    }

    QString fromActionStyle(ActionItem::Style style) const
    {
        if ( style == ActionItem::IconOnly )
            return g_itemValueStyleIcO;
        else if ( style == ActionItem::TextOnly )
            return g_itemValueStyleTeO;
        else if ( style == ActionItem::TextBesideIcon )
            return g_itemValueStyleTeBI;
        else if ( style == ActionItem::TextUnderIcon )
            return g_itemValueStyleTeUI;

        return g_itemValueStyleTeBI;
    }

    QString fromAlignment(ToolbuttonItem::Alignment alignment) const
    {
        if ( alignment == ToolbuttonItem::AlignLeft )
            return g_itemValueAlignLeft;
        else if ( alignment == ToolbuttonItem::AlignRight )
            return g_itemValueAlignRight;
        else if ( alignment == ToolbuttonItem::AlignHCenter )
            return g_itemValueAlignHCenter;
        else if ( alignment == ToolbuttonItem::AlignTop )
            return g_itemValueAlignTop;
        else if ( alignment == ToolbuttonItem::AlignBottom )
            return g_itemValueAlignBottom;
        else if ( alignment == ToolbuttonItem::AlignVCenter )
            return g_itemValueAlignVCenter;
        else if ( alignment == ToolbuttonItem::AlignCenter )
            return g_itemValueAlignCenter;
        return g_itemValueAlignLeft;
    }

    QString fromSizePolicy(ToolbuttonItem::SizePolicy sizePolicy) const
    {
        if ( sizePolicy == ToolbuttonItem::Fixed )
            return g_itemValueSizePolicyFixed;
        else if ( sizePolicy == ToolbuttonItem::Minimum )
            return g_itemValueSizePolicyMinimum;

        return g_itemValueSizePolicyFixed;
    }

private:
    const UiModel   &mModel;
};


QString read(const QString &path, UiModel &model)
{
    // Read configuration by parts.
    try
    {
        XmlReader(path, g_submenusName, model.mSubMenus);
        XmlReader(path, g_menuName, model.mRootMenu);
        XmlReader(path, g_tabcontrolName, model.mToolbar);
        XmlReader(path, g_mainmenuName, model.mToolbarMenu);
        XmlReader(path, g_rightbarName, model.mRightBar);
    }
    catch ( const QString &error )
    {
        qDebug() << QObject::tr("Uim::Read error: %1").arg(error);
        return error;
    }

    return QString();
}

QString write(const QString &path, const UiModel &model)
{
    try
    {
        XmlWriter(path, model);
    }
    catch ( const QString &error )
    {
        qDebug() << QObject::tr("Uim::Write error: %1").arg(error);
        return error;
    }

    return QString();
}

}   // namespace UiXmlConfig

}   // namespace Uim
