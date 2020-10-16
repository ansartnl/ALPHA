#ifndef PROFILE_SETTINGS_H
#define PROFILE_SETTINGS_H

#include <QtCore/QMap>
#include <QtCore/QSet>
#include <QtCore/QPointF>
#include <QtCore/QSharedPointer>

#include <QtGui/QColor>
#include <QtGui/QFont>


namespace UserProfile {

    //! Focus settings
    struct Focus
    {
            //! Map of extra opened FIRs (except main one)
            typedef QSet<QString> FIRList;

            /** Center point of the scene (scene coordinate) */
            QPointF m_CenterPoint;
            /** Viewport scale along window width */
            qreal m_ScaleWidth;
            /** Viewport scale along window height */
            qreal m_ScaleHeight;
            /** Slider value position */
            int m_ScaleValue;
            /** Main (center) FIR */
            QString m_mainFIR;
            /** Map of last opened FIRs */
            FIRList m_extraFIRs;
    };

    //! Grid settings
    struct Grid
    {
            //! Mesurement of distance (unit)
            enum DistanceUnit
            {
                Kilometers, NauticalMiles
            //!< 1,852 metres
            };

            bool m_isAzimuthRange;
            bool m_isCartesian;
            bool m_isGeodesic;
            bool m_isFullScreen;

            bool m_isScale;

            DistanceUnit m_distUnit;

            /** Range in kilometers */
            int m_Range;
            /** Azimuth in degree */
            int m_Azimuth;
    };

    //! Color data type
    enum ColorType
    {
        CtBackground = 1,
        CtGrid,
        CtFormularActive,
        CtFormularPassive,
        CtControlled,
        CtIn,
        CtRelated,
        CtUnrelated,
        CtControlled_back,
        CtIn_back,
        CtRelated_back,
        CtUnrelated_back,
        CtMap,
        CtCorridor,
        CtMonitoringRoute,
        CtHolding,
        CtFIR,
        CtSector,
        CtBorder,
        CtLegend,
        CtApproach,
        CtBlastoff,
        CtRestr,
        CtRestrUser,
        CtRestrUserObstacle,
        CtRestrUserNFZ,
        CtRestrUserMap,
        CtListAct,
        CtListAssume,
        CtListIn,
        CtListTransfered,
        CtListSelectedText,
        CtGauge,
        CtMapNote,
        CtMeteo,
        CtFrequency,
        CtExtended
    };

    //! Flags assotiated with the color
    enum ColorFlag
    {
        CfContour = 0x01
    };

    //! Language
    enum Language
    {
        English = 1, Russian
    };

    //! Environment settings
    struct Environment
    {
            /** Current language */
            Language m_lang;
            /** Is window in full screen mode */
            bool m_isFullScreen;
            /** True if toolbar is visible */
            bool m_isToolbar;
            /** True if statusbar is visible */
            bool m_isStatusBar;
    };

    //! Layers data buffer.
    typedef QByteArray Layers;

    //! Fonts data buffer.
    typedef QByteArray Fonts;

    class CSettingsStream;
    //! Main container for storing the specified user settings.
    class CSettings
    {
        public:
            //! Retrieve instanse of this settings.
            static CSettings& Instance();

            //! Set specified user which is owner of this settings.
            void SetUser(const QString& nameUser);

            //! Load settings from the stream.
            bool Load();

            //! Save settings to stream.
            void Save();

            void SetFrequency(QString frequency);
            //! Associate color with the type.
            /*!
             \param type Color type
             \param flags Flags associated with a color
             */
            void SetColorFlags(int type, int flags);

            //! Get color flags.
            /*!
             \param type Color type
             */
            int GetColorFlags(int type) const;

            //! Associate color with the type.
            void SetColor(int type, const QColor& color);

            //! Get color assiciated with a type.
            QColor GetColor(int type) const;

            //! Retrieve grid settings.
            const Grid& GetGrid() const
            {
                return m_grid;
            }
            //! Get or set grid settings.
            Grid& GetGrid()
            {
                return m_grid;
            }

            //! Retrieve focus settings.
            const Focus& GetFocus() const
            {
                return m_focus;
            }
            //! Set or get focus settings.
            Focus& GetFocus()
            {
                return m_focus;
            }

            //! Retrieve environment settings.
            const Environment& GetEnvi() const
            {
                return m_envi;
            }
            //! Get or set environment settings.
            Environment& GetEnvi()
            {
                return m_envi;
            }

            //! Set new font.
            void SetFonts(const Fonts& fonts)
            {
                m_fonts = fonts;
            }
            //! Retrieve current font.
            const Fonts& GetFonts() const
            {
                return m_fonts;
            }

            //! Set new layers buffer.
            void SetLayers(const Layers& layers)
            {
                m_layers = layers;
            }
            //! Retrieve current layers buffer.
            const Layers& GetLayers() const
            {
                return m_layers;
            }

        private:
            CSettings();

        private:
            typedef QMap<int, QColor> TColorsMap;
            typedef QMap<QString, QColor> TColorsMapDinamic;
            typedef QMap<int, int> TColorFlagsMap;
            typedef QSharedPointer<CSettingsStream> TStream;

            TColorsMap m_colors;
            /** Flags associated with the color */
            TColorFlagsMap m_colorFlags;

            Grid m_grid;
            Focus m_focus;
            Environment m_envi;

            Fonts m_fonts;

            Layers m_layers;

            /** Specified user settings */
            QString m_nameUser;

            /** Storage stream */
            TStream m_pStream;
    };

}

#endif // PROFILE_SETTINGS_H
