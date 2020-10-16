#include "ProfileSettings.h"
#include "main.h"
#include <QSettings>
const QString g_nameFolder = "UserProfiles";
//
const QString g_stgColorsGroup = "Colors";
const QString g_stgColorBackground = "Background";
const QString g_stgColorGrid = "Grid";

const QString g_stgColorFormularActive = "Formular_active";
const QString g_stgColorFormularPassive = "Formular_passive";

const QString g_stgColorControlled = "Controlled";
const QString g_stgColorIn = "InColor";
const QString g_stgColorRelated = "Related";
const QString g_stgColorUnrelated = "Unrelated";
const QString g_stgColorControlled_back = "Controlled_back";
const QString g_stgColorIn_back = "In_back";
const QString g_stgColorRelated_back = "Related_back";
const QString g_stgColorUnrelated_back = "Unrelated_back";

const QString g_stgColorMap = "Map";
const QString g_stgColorCorridor = "Corridor";
const QString g_stgColorMonitoringRoute = "MonitoringRoute";
const QString g_stgColorHolding = "Holding";
const QString g_stgColorFIR = "FIR";
const QString g_stgColorSector = "Sector";
const QString g_stgColorBorder = "Border";
const QString g_stgColorLegend = "Legend";
const QString g_stgColorApproach = "Approach";
const QString g_stgColorBlastoff = "Blastoff";
const QString g_stgColorFIRFlags = "FIR_Flags";
const QString g_stgColorBorderFlags = "Border_Flags";
const QString g_stgColorListAct = "Act";
const QString g_stgColorListAssume = "Assume";
const QString g_stgColorListTransfered = "Transfered";
const QString g_stgColorListIn = "In";
const QString g_stgColorListSelectedText = "SelectedText";
const QString g_stgColorGauge = "Gauge";
const QString g_stgColorMapNote = "Map_Note";
const QString g_stgColorMeteo = "Meteo";
QString g_stgColorFrequency = "Frequency";
//
const QString g_stgFocusGroup = "Focus";
const QString g_stgFocusCenterPointX = "CenterPointX";
const QString g_stgFocusCenterPointY = "CenterPointY";
const QString g_stgFocusScaleWidth = "ScaleWidth";
const QString g_stgFocusScaleHeight = "ScaleHeight";
const QString g_stgFocusScaleValue = "ScaleValue";
const QString g_stgFocusMainFIR = "MainFIR";
const QString g_stgFocusExtraFIRs = "ExtraFIRs";
//
const QString g_stgGridGroup = "Grid";
const QString g_stgGridAzimuthRange = "AzimuthRange";
const QString g_stgGridCartesian = "Cartesian";
const QString g_stgGridGeodesic = "Geodesic";
const QString g_stgGridFullScreen = "FullScreen";
const QString g_stgGridRange = "Range";
const QString g_stgGridAzimuth = "Azimuth";
const QString g_stgGridDistanceUnit = "DistanceUnit";
const QString g_stgGridScale = "Scale";
//
const QString g_stgEnviGroup = "Environment";
const QString g_stgEnviLanguage = "Language";
const QString g_stgEnviFullscreen = "FullScreen";
const QString g_stgEnviToolbar = "Toolbar";
const QString g_stgEnviStatusbar = "Statusbar";
//
const QString g_stgFonts = "Fonts";
const QString g_stgLayers = "Layers";



namespace UserProfile {

//! Base interface for settings storage.
class CSettingsStream
{
public:
    CSettingsStream(CSettings& stg)
        : m_stg(stg)
    {}
    virtual ~CSettingsStream() {}

    void SetUser(const QString& name)
    { m_nameUser = name; }

    bool IsValid() const
    { return !m_nameUser.isEmpty(); }

    virtual bool Load() = 0;
    virtual void Save() = 0;

protected:
    QString     m_nameUser;
    /** Reference to the user settings object */
    CSettings&  m_stg;
};

//! Storage stream is registry database (QSettings)
class CSettingsStreamQt : public CSettingsStream
{
public:
    CSettingsStreamQt(CSettings& stg)
        : CSettingsStream(stg)
    {}

    virtual bool Load();
    virtual void Save();
};

bool CSettingsStreamQt::Load()
{
    if ( !IsValid() )
        return false;

    QSettings &settings = *globalSettings();
    QVariant var;
    QString userFolder = g_nameFolder + "/" + m_nameUser;

    // Read colors settings ...

    settings.beginGroup(QString(userFolder + "/" + g_stgColorsGroup));

    var = settings.value(g_stgColorBackground);
    if ( !var.isNull() )
        m_stg.SetColor( CtBackground, var.value<QColor>() );
    var = settings.value(g_stgColorGrid);
    if ( !var.isNull() )
        m_stg.SetColor( CtGrid, var.value<QColor>() );

    var = settings.value(g_stgColorFormularActive);
    if ( !var.isNull() )
        m_stg.SetColor( CtFormularActive, var.value<QColor>() );
    var = settings.value(g_stgColorFormularPassive);
    if ( !var.isNull() )
        m_stg.SetColor( CtFormularPassive, var.value<QColor>() );

    var = settings.value(g_stgColorControlled);
    if ( !var.isNull() )
        m_stg.SetColor( CtControlled, var.value<QColor>() );
    var = settings.value(g_stgColorIn);
    if ( !var.isNull() )
        m_stg.SetColor( CtIn, var.value<QColor>() );

    var = settings.value(g_stgColorRelated);
    if ( !var.isNull() )
        m_stg.SetColor( CtRelated, var.value<QColor>() );

    var = settings.value(g_stgColorUnrelated);
    if ( !var.isNull() )
        m_stg.SetColor( CtUnrelated, var.value<QColor>() );

    var = settings.value(g_stgColorControlled_back);
    if ( !var.isNull() )
        m_stg.SetColor( CtControlled_back, var.value<QColor>() );

    var = settings.value(g_stgColorIn_back);
    if ( !var.isNull() )
        m_stg.SetColor( CtIn_back, var.value<QColor>() );

    var = settings.value(g_stgColorRelated_back);
    if ( !var.isNull() )
        m_stg.SetColor( CtRelated_back, var.value<QColor>() );

    var = settings.value(g_stgColorUnrelated_back);
    if ( !var.isNull() )
        m_stg.SetColor( CtUnrelated_back, var.value<QColor>() );

    var = settings.value(g_stgColorMap);
    if ( !var.isNull() )
        m_stg.SetColor( CtMap, var.value<QColor>() );
    var = settings.value(g_stgColorCorridor);
    if ( !var.isNull() )
        m_stg.SetColor( CtCorridor, var.value<QColor>() );
    var = settings.value(g_stgColorMonitoringRoute);
    if ( !var.isNull() )
        m_stg.SetColor( CtMonitoringRoute, var.value<QColor>() );
    var = settings.value(g_stgColorHolding);
    if ( !var.isNull() )
        m_stg.SetColor( CtHolding, var.value<QColor>() );
    var = settings.value(g_stgColorFIR);
    if ( !var.isNull() )
        m_stg.SetColor( CtFIR, var.value<QColor>() );
    var = settings.value(g_stgColorSector);
    if ( !var.isNull() )
        m_stg.SetColor( CtSector, var.value<QColor>() );
    var = settings.value(g_stgColorBorder);
    if ( !var.isNull() )
        m_stg.SetColor( CtBorder, var.value<QColor>() );
    var = settings.value(g_stgColorLegend);
    if ( !var.isNull() )
        m_stg.SetColor( CtLegend, var.value<QColor>() );
    var = settings.value(g_stgColorApproach);
    if ( !var.isNull() )
        m_stg.SetColor( CtApproach, var.value<QColor>() );
    var = settings.value(g_stgColorBlastoff);
    if ( !var.isNull() )
        m_stg.SetColor( CtBlastoff, var.value<QColor>() );
    var = settings.value(g_stgColorListAct);
    if ( !var.isNull() )
        m_stg.SetColor( CtListAct, var.value<QColor>() );
    var = settings.value(g_stgColorListAssume);
    if ( !var.isNull() )
        m_stg.SetColor( CtListAssume, var.value<QColor>() );
    var = settings.value(g_stgColorListIn);
    if ( !var.isNull() )
        m_stg.SetColor( CtListIn, var.value<QColor>() );
    var = settings.value(g_stgColorListSelectedText);
    if ( !var.isNull() )
        m_stg.SetColor( CtListSelectedText, var.value<QColor>() );
    var = settings.value(g_stgColorListTransfered);
    if ( !var.isNull() )
        m_stg.SetColor( CtListTransfered, var.value<QColor>() );
    var = settings.value(g_stgColorGauge);
    if ( !var.isNull() )
        m_stg.SetColor( CtGauge, var.value<QColor>() );
    var = settings.value(g_stgColorMapNote);
    if ( !var.isNull() )
        m_stg.SetColor( CtMapNote, var.value<QColor>() );
    var = settings.value(g_stgColorMeteo);
    if ( !var.isNull() )
        m_stg.SetColor( CtMeteo, var.value<QColor>() );
    var = settings.value(g_stgColorFrequency);
    if ( !var.isNull() )
        m_stg.SetColor( CtFrequency, var.value<QColor>() );

    var = settings.value(g_stgColorFIRFlags);
    if ( !var.isNull() )
        m_stg.SetColorFlags( CtFIR, var.toInt() );
    var = settings.value(g_stgColorBorderFlags);
    if ( !var.isNull() )
        m_stg.SetColorFlags( CtBorder, var.toInt() );

    settings.endGroup();

    // Read focus settings ...

    settings.beginGroup(QString(userFolder + "/" + g_stgFocusGroup));

    Focus& focus = m_stg.GetFocus();
    focus.m_CenterPoint.rx() = settings.value(g_stgFocusCenterPointX, QVariant(0.0)).toDouble();
    focus.m_CenterPoint.ry() = settings.value(g_stgFocusCenterPointY, QVariant(0.0)).toDouble();
    focus.m_ScaleWidth = settings.value(g_stgFocusScaleWidth, QVariant(1.0)).toDouble();
    focus.m_ScaleHeight = settings.value(g_stgFocusScaleHeight, QVariant(1.0)).toDouble();
    focus.m_ScaleValue = settings.value(g_stgFocusScaleValue, QVariant(0)).toInt();
    focus.m_mainFIR = settings.value(g_stgFocusMainFIR).toString();
    QByteArray baExtraFIRs = settings.value(g_stgFocusExtraFIRs).toByteArray();
    QDataStream streamExtraFIRs(baExtraFIRs);
    streamExtraFIRs >> focus.m_extraFIRs;

    settings.endGroup();

    // Read grid settings ...

    settings.beginGroup(QString(userFolder + "/" + g_stgGridGroup));

    Grid& grid = m_stg.GetGrid();
    grid.m_isAzimuthRange = settings.value(g_stgGridAzimuthRange, QVariant(false)).toBool();
    grid.m_isCartesian = settings.value(g_stgGridCartesian, QVariant(false)).toBool();
    grid.m_isGeodesic = settings.value(g_stgGridGeodesic, QVariant(false)).toBool();
    grid.m_isFullScreen = settings.value(g_stgGridFullScreen, QVariant(false)).toBool();
    grid.m_isScale = settings.value(g_stgGridScale, QVariant(false)).toBool();
    grid.m_Range = settings.value(g_stgGridRange, QVariant(int(50))).toInt();
    grid.m_Azimuth = settings.value(g_stgGridAzimuth, QVariant(int(30))).toInt();
    grid.m_distUnit = (Grid::DistanceUnit)settings.value(g_stgGridDistanceUnit, QVariant(int(0))).toInt();

    settings.endGroup();

    // Read environment settings ...

    settings.beginGroup(QString(userFolder + "/" + g_stgEnviGroup));

    Environment& envi = m_stg.GetEnvi();
    envi.m_lang = (Language)settings.value(g_stgEnviLanguage, QVariant((int)Russian)).toInt();
    envi.m_isFullScreen = settings.value(g_stgEnviFullscreen, QVariant(false)).toBool();
    envi.m_isToolbar = settings.value(g_stgEnviToolbar, QVariant(true)).toBool();
    envi.m_isStatusBar = settings.value(g_stgEnviStatusbar, QVariant(true)).toBool();

    settings.endGroup();

    // General settings ...

    settings.beginGroup(userFolder);

    // Fonts
    QByteArray baFonts;
    baFonts = settings.value(g_stgFonts).toByteArray();
    m_stg.SetFonts(baFonts);

    // Layers
    QByteArray baLayers;
    baLayers = settings.value(g_stgLayers).toByteArray();
    m_stg.SetLayers(baLayers);

    settings.endGroup();

    return true;
}

void CSettingsStreamQt::Save()
{
    if ( !IsValid() )
        return;

    QSettings &settings = *globalSettings();
    QString userFolder = g_nameFolder + "/" + m_nameUser;

    // Save colors settings ...

    settings.beginGroup(QString(userFolder + "/" + g_stgColorsGroup));

    settings.setValue(g_stgColorBackground, m_stg.GetColor(CtBackground));
    settings.setValue(g_stgColorGrid, m_stg.GetColor(CtGrid));
    settings.setValue(g_stgColorFormularActive, m_stg.GetColor(CtFormularActive));
    settings.setValue(g_stgColorFormularPassive, m_stg.GetColor(CtFormularPassive));

    settings.setValue(g_stgColorControlled, m_stg.GetColor(CtControlled));
    //settings.setValue(g_stgColorControlledSelected, m_stg.GetColor(CtControlledSelected));
    settings.setValue(g_stgColorIn, m_stg.GetColor(CtIn));
    //settings.setValue(g_stgColorInSelected, m_stg.GetColor(CtInSelected));
    settings.setValue(g_stgColorRelated, m_stg.GetColor(CtRelated));
    //settings.setValue(g_stgColorRelatedSelected, m_stg.GetColor(CtRelatedSelected));
    settings.setValue(g_stgColorUnrelated, m_stg.GetColor(CtUnrelated));
    //settings.setValue(g_stgColorUnrelatedSelected, m_stg.GetColor(CtUnrelatedSelected));

    settings.setValue(g_stgColorControlled_back, m_stg.GetColor(CtControlled_back));
    //settings.setValue(g_stgColorControlledSelected_back, m_stg.GetColor(CtControlledSelected_back));
    settings.setValue(g_stgColorIn_back, m_stg.GetColor(CtIn_back));
    //settings.setValue(g_stgColorInSelected_back, m_stg.GetColor(CtInSelected_back));
    settings.setValue(g_stgColorRelated_back, m_stg.GetColor(CtRelated_back));
    //settings.setValue(g_stgColorRelatedSelected_back, m_stg.GetColor(CtRelatedSelected_back));
    settings.setValue(g_stgColorUnrelated_back, m_stg.GetColor(CtUnrelated_back));
    //settings.setValue(g_stgColorUnrelatedSelected_back, m_stg.GetColor(CtUnrelatedSelected_back));

    settings.setValue(g_stgColorMap, m_stg.GetColor(CtMap));
    settings.setValue(g_stgColorCorridor, m_stg.GetColor(CtCorridor));
    settings.setValue(g_stgColorMonitoringRoute, m_stg.GetColor(CtMonitoringRoute));
    settings.setValue(g_stgColorHolding, m_stg.GetColor(CtHolding));
    settings.setValue(g_stgColorFIR, m_stg.GetColor(CtFIR));
    settings.setValue(g_stgColorSector, m_stg.GetColor(CtSector));
    settings.setValue(g_stgColorBorder, m_stg.GetColor(CtBorder));
    settings.setValue(g_stgColorLegend, m_stg.GetColor(CtLegend));
    settings.setValue(g_stgColorApproach, m_stg.GetColor(CtApproach));
    settings.setValue(g_stgColorBlastoff, m_stg.GetColor(CtBlastoff));
    settings.setValue(g_stgColorListAct, m_stg.GetColor(CtListAct));
    settings.setValue(g_stgColorListAssume, m_stg.GetColor(CtListAssume));
    settings.setValue(g_stgColorListIn, m_stg.GetColor(CtListIn));
    settings.setValue(g_stgColorListSelectedText, m_stg.GetColor(CtListSelectedText));
    settings.setValue(g_stgColorListTransfered, m_stg.GetColor(CtListTransfered));
    settings.setValue(g_stgColorGauge, m_stg.GetColor(CtGauge));
    settings.setValue(g_stgColorMapNote, m_stg.GetColor(CtMapNote));
    settings.setValue(g_stgColorMeteo, m_stg.GetColor(CtMeteo));
    settings.setValue(g_stgColorFrequency, m_stg.GetColor(CtFrequency));

    settings.setValue(g_stgColorFIRFlags, m_stg.GetColorFlags(CtFIR));
    settings.setValue(g_stgColorBorderFlags, m_stg.GetColorFlags(CtBorder));

    settings.endGroup();

    // Save focus settings ...

    settings.beginGroup(QString(userFolder + "/" + g_stgFocusGroup));

    Focus& focus = m_stg.GetFocus();
    settings.setValue(g_stgFocusCenterPointX, (double)focus.m_CenterPoint.x());
    settings.setValue(g_stgFocusCenterPointY, (double)focus.m_CenterPoint.y());
    settings.setValue(g_stgFocusScaleWidth, (double)focus.m_ScaleWidth);
    settings.setValue(g_stgFocusScaleHeight, (double)focus.m_ScaleHeight);
    settings.setValue(g_stgFocusScaleValue, focus.m_ScaleValue);
    settings.setValue(g_stgFocusMainFIR, focus.m_mainFIR);
    QByteArray baExtraFIRs;
    QDataStream streamExtraFIRs(&baExtraFIRs, QIODevice::WriteOnly);
    streamExtraFIRs << focus.m_extraFIRs;
    settings.setValue(g_stgFocusExtraFIRs, baExtraFIRs);

    settings.endGroup();

    // Save grid settings ...

    settings.beginGroup(QString(userFolder + "/" + g_stgGridGroup));

    Grid& grid = m_stg.GetGrid();
    settings.setValue(g_stgGridAzimuthRange, grid.m_isAzimuthRange);
    settings.setValue(g_stgGridCartesian, grid.m_isCartesian);
    settings.setValue(g_stgGridGeodesic, grid.m_isGeodesic);
    settings.setValue(g_stgGridFullScreen, grid.m_isFullScreen);
    settings.setValue(g_stgGridScale, grid.m_isScale);
    settings.setValue(g_stgGridRange, grid.m_Range);
    settings.setValue(g_stgGridAzimuth, grid.m_Azimuth);
    settings.setValue(g_stgGridDistanceUnit, (int)grid.m_distUnit);

    settings.endGroup();

    // Save environment settings ...

    settings.beginGroup(QString(userFolder + "/" + g_stgEnviGroup));

    Environment& envi = m_stg.GetEnvi();
    settings.setValue(g_stgEnviLanguage, (int)envi.m_lang);
    settings.setValue(g_stgEnviFullscreen, envi.m_isFullScreen);
    settings.setValue(g_stgEnviToolbar, envi.m_isToolbar);
    settings.setValue(g_stgEnviStatusbar, envi.m_isStatusBar);

    settings.endGroup();

    // General settings ...

    settings.beginGroup(userFolder);

    settings.setValue(g_stgFonts, m_stg.GetFonts());
    settings.setValue(g_stgLayers, m_stg.GetLayers());

    settings.endGroup();
}


CSettings& CSettings::Instance()
{
    static CSettings stg;
    return stg;
}

CSettings::CSettings()
{
    m_pStream = TStream(new CSettingsStreamQt(*this));

    // Default initialization ...

    SetColor(CtBackground, QColor(24, 32, 64));
    SetColor(CtGrid, QColor(70, 100, 80));
    SetColor(CtFormularActive, Qt::green);
    SetColor(CtFormularPassive, Qt::green);
    SetColor(CtControlled, Qt::black);
    SetColor(CtControlled_back, Qt::transparent);
    //SetColor(CtControlledSelected, Qt::black);
    //SetColor(CtControlledSelected_back, Qt::lightGray);
    SetColor(CtIn, Qt::blue);
    SetColor(CtIn_back, Qt::transparent);
    //SetColor(CtInSelected, Qt::blue);
    //SetColor(CtInSelected_back, Qt::lightGray);
    SetColor(CtRelated, QColor(139, 69, 19));
    SetColor(CtRelated_back, Qt::transparent);
    //SetColor(CtRelatedSelected, QColor(139, 69, 19));
    //SetColor(CtRelatedSelected_back, Qt::lightGray);
    SetColor(CtUnrelated, QColor(100, 100, 100));
    SetColor(CtUnrelated_back, Qt::transparent);
    //SetColor(CtUnrelatedSelected, QColor(100, 100, 100));
    //SetColor(CtUnrelatedSelected_back, Qt::lightGray);

    SetColor(CtMap, QColor(100, 100, 100));
    SetColor(CtCorridor, QColor(1, 52, 73));
    SetColor(CtMonitoringRoute, QColor(120, 120, 120));
    SetColor(CtHolding, QColor(100, 100, 100));
    SetColor(CtFIR, QColor(235, 40, 70));
    SetColor(CtSector, Qt::gray);
    SetColor(CtBorder, QColor(150, 80, 80));
    SetColor(CtLegend, QColor(200, 200, 50));
    SetColor(CtApproach, QColor(150, 150, 150));
    SetColor(CtBlastoff, QColor(150, 150, 150));
    SetColor(CtListAct, QColor(100,100,100,120));
    SetColor(CtListAssume, QColor(0,150,255,120));
    SetColor(CtListIn, QColor(0,0,0,0));
    SetColor(CtListSelectedText, QColor(0,127,0,255));
    SetColor(CtListTransfered, QColor(100,100,100,120));
    SetColor(CtGauge, QColor(0,0,0,255));
    SetColor(CtMapNote, QColor(0,0,0,255));
    SetColor(CtMeteo, QColor(255,0,0));
    SetColor(CtFrequency, QColor(150,150,150));
    SetColorFlags(CtFIR, CfContour);
    SetColorFlags(CtBorder, CfContour);

    m_grid.m_isAzimuthRange = m_grid.m_isCartesian = m_grid.m_isGeodesic =
    m_grid.m_isFullScreen = m_grid.m_isScale = false;
    m_grid.m_Range = 50;
    m_grid.m_Azimuth = 30;
    m_grid.m_distUnit = Grid::Kilometers;

    m_focus.m_CenterPoint = QPointF(0.0, 0.0);
    m_focus.m_ScaleWidth = 1.0;
    m_focus.m_ScaleHeight = 1.0;
    m_focus.m_ScaleValue = 0;

    m_envi.m_lang = Russian;
    m_envi.m_isFullScreen = false;
    m_envi.m_isToolbar = true;
    m_envi.m_isStatusBar = true;
}

void CSettings::SetUser(const QString& nameUser)
{
    m_nameUser = nameUser;
    m_pStream->SetUser(nameUser);
}

void CSettings::SetColor(int type, const QColor& color)
{
    TColorsMap::iterator it = m_colors.find(type);
    if ( it != m_colors.end() )
        *it = color;
    else
        m_colors[type] = color;
}

QColor CSettings::GetColor(int type) const
{
    TColorsMap::const_iterator it = m_colors.find(type);
    if ( it != m_colors.end() )
    {
        return *it;
    }
    return QColor();
}

void CSettings::SetColorFlags(int type, int flags)
{
    TColorFlagsMap::iterator it = m_colorFlags.find(type);
    if ( it != m_colorFlags.end() )
        *it = flags;
    else
        m_colorFlags[type] = flags;
}

int CSettings::GetColorFlags(int type) const
{
    TColorFlagsMap::const_iterator it = m_colorFlags.find(type);
    if ( it != m_colorFlags.end() )
        return *it;
    return 0;
}

bool CSettings::Load()
{
    return m_pStream->Load();
}

void CSettings::Save()
{
    m_pStream->Save();
}

void CSettings::SetFrequency(QString frequency)
{
    //g_stgColorFrequency.clear();
    g_stgColorFrequency = "Frequency_" + frequency;
}

}   // namespace UserProfile
