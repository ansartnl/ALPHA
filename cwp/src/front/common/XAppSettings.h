#ifndef XAPPSETTINGS_H
#define XAPPSETTINGS_H

#include <QtCore/QMap>
#include <QtGui/QFont>

//! Class for text font size
class CTextSize
{
public:
    //! Constructor
    CTextSize();
    virtual ~CTextSize() {}

    //! Sizes
    enum XFontSize { Small, Medium, Large };
    typedef QMap<int, int>	TSizeMap;

    TSizeMap& GetSizes()
    { return m_mapSize; }
    void SetSizes(const TSizeMap& map)
    { m_mapSize = map; }

    XFontSize GetCurrentSize() const
    { return m_curSize; }
    void SetCurrentSize(XFontSize size);

    int GetCurrentTextSize();

    void NextCurrentSize();

private:
    XFontSize   m_curSize;
    TSizeMap    m_mapSize;
};

//! Class contains settings for application
class AppSettings
{
public:
    //! FL -> meters
    //@{
    //static const int METER_DIVIDER = 10;
    static const int METER_DIVIDER = 1;
    static const int METER_CAPACITY = 4;
    static const int CFL_METER_DIVIDER = 10;
    static const int CFL_METER_CAPACITY = 4;
    //@}

    //! Constructor
    AppSettings();
    virtual ~AppSettings();

    //! Fonts
    //@{
    enum XFontType { COMMON_FONT, FONT_CTRL_DLG_LIST };

    typedef QMap<int, QFont>	TFontMap;

    //! Get current font
    /*!
      \param type a type of font
    */
    static const QFont GetXFont(const XFontType type);

    //! Set current font
    /*!
      \param type a type of font
      \param font a font
    */
    static void SetXFont(const XFontType type, const QFont font);

    //! Get list of CXFont
    /*!
      \return a reference on TFontMap
    */
    static const TFontMap& GetMapXFont() { return m_mapFont; }
    //! Set list of CXFont
    /*!
      \param map a reference on TFontMap
    */
    static void SetMapXFont(const TFontMap& map) { m_mapFont = map; }

    //! Get text font size
    /*!
      \return a reference on CTextSize
    */
    static CTextSize& GetTextSize() { return m_textSize; }
    //@}

    //! International System of Units ( SI )
    /*! 1 - meters, 2 - feet, 4 - knots */
    //{@
    enum TypeSI { SIMeters = 1, SIFeet = 2, SIKnot = 4 };
    static const quint8 GetSI() { return m_qSI; }
    static void SetSI(const quint8 qSI) { m_qSI = qSI; }
    static bool IsSISet(TypeSI val) { return m_qSI & val; }
    //@}

    //! Update all objects inherited from CXAppSettings ( call virtual function )
    //@{
    //! Update font ( call UpdateXFont() )
    static void UpdateXFonts();
    //! Update translation ( call Translate() )
    static void UpdateTranslate();
    //! Update SI ( call SI() )
    static void UpdateSI();
    //@}

protected:
    //! Function called after change font
    virtual void UpdateXFont() = 0;
    //! Function called after change language
    virtual void Translate() = 0;
    //! Function called after change SI
    virtual void SI() = 0;

private:
//    typedef QMap<AppSettings*, AppSettings*>	TObjMap;

//    static TObjMap      m_mapObj;
    static TFontMap     m_mapFont;
    static quint8       m_qSI;
    static CTextSize    m_textSize;
};

QDataStream& operator<< (QDataStream& out, const AppSettings& value);
QDataStream& operator>> (QDataStream& in, AppSettings& value);

#endif // XAPPSETTINGS_H
