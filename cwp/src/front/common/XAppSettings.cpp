#include "XAppSettings.h"
#include <QDataStream>

//AppSettings::TObjMap  AppSettings::m_mapObj = AppSettings::TObjMap();
AppSettings::TFontMap AppSettings::m_mapFont = AppSettings::TFontMap();
quint8 AppSettings::m_qSI(1);

CTextSize AppSettings::m_textSize = CTextSize();

static QMap<AppSettings*, AppSettings*> & m_mapObj()
{
    static QMap<AppSettings*, AppSettings*>  m_mapObj;
    return m_mapObj;
}

AppSettings::AppSettings()
{
    m_mapObj()[this] = this;
}

AppSettings::~AppSettings()
{
    m_mapObj().remove(this);
}

const QFont AppSettings::GetXFont(const XFontType type)
{
    return m_mapFont[type];
}

void AppSettings::SetXFont(const XFontType type, const QFont font)
{
    m_mapFont[type] = font;
}

void AppSettings::UpdateXFonts()
{
    QMap<AppSettings*, AppSettings*>::const_iterator iter = m_mapObj().begin();
    for(; iter != m_mapObj().end(); ++iter)
        iter.value()->UpdateXFont();
}

void AppSettings::UpdateTranslate()
{
    QMap<AppSettings*, AppSettings*>::const_iterator iter = m_mapObj().begin();
    for(; iter != m_mapObj().end(); ++iter)
        iter.value()->Translate();
}

void AppSettings::UpdateSI()
{
    QMap<AppSettings*, AppSettings*>::const_iterator iter = m_mapObj().begin();
    for(; iter != m_mapObj().end(); ++iter)
        iter.value()->SI();
}

QDataStream& operator<< (QDataStream& out, const AppSettings& value)
{
    if(out.status() == QDataStream::Ok)
    {
        out << value.GetMapXFont();
        out << value.GetSI();
    }
    return out;
}

QDataStream& operator>> (QDataStream& in, AppSettings& value)
{
    AppSettings::TFontMap mapFonts;
    if(in.status() == QDataStream::Ok) in >> mapFonts;
    value.SetMapXFont(mapFonts);
    quint8 qSI(1);
    if(in.status() == QDataStream::Ok) in >> qSI;
    value.SetSI(qSI);
    return in;
}

// CTextSize

CTextSize::CTextSize()
{
    m_curSize = Small;
    m_mapSize.insert(Small, 8);
    m_mapSize.insert(Medium, 10);
    m_mapSize.insert(Large, 12);
}

int CTextSize::GetCurrentTextSize()
{
    return m_mapSize.value(m_curSize, 10);
}

void CTextSize::SetCurrentSize(XFontSize size)
{
    if(size == Small)
        m_curSize = Small;
    else if(size == Medium)
        m_curSize = Medium;
    else if(size == Large)
        m_curSize = Large;
    else
        m_curSize = Small;
}

void CTextSize::NextCurrentSize()
{
    if(m_curSize == Small)
        m_curSize = Medium;
    else if(m_curSize == Medium)
        m_curSize = Large;
    else if(m_curSize == Large)
        m_curSize = Small;
    else
        m_curSize = Small;
}
