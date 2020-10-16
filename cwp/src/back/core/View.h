#ifndef VIEW_H
#define VIEW_H

#include <QColor>
#include <QHash>
#include <QMultiHash>

#include "Scene.h"

#include "Airplane.h"

#include "Meteo.h"
#include "AirplaneState.h"
#include "AirplaneCollision.h"
#include "XtObject.h"

#include "back/ground/IScene.h"

//! Class includes scene list
class CView
        :public IScene
{
    struct TrackInfo
    {
        QDateTime appearance_time;
        int counter;
        quint16 track_num;

        bool dbl;
    };

    typedef QMultiHash<quint16, quint16> Mode3AToTrackNum;
    typedef QHash<quint16, TrackInfo> Mode3AToTrackInfo;
    //typedef QHash<quint16, QColor> Mode3AToDblColor;
    typedef QHash<quint16, quint16> TrackNumToMode3A;

    public:
        //! List of meteo phenomenon
        typedef QList<QSharedPointer<core::CMeteo> > TMeteoList;
        //! Map of airplanes. Key - airplane identifier
        typedef QHash<quint16, QSharedPointer<CAirplane> > TAirplanesMap;
        //! Pointer to scene
        typedef QSharedPointer<Scene> TScenePtr;
        //! Map of scene pointers. Key - unique scene name
        typedef QMap<QString, TScenePtr> TSceneList;

        //! Constructor
        CView();
        //! Destructor
        virtual ~CView();

        //! Add scene to list
        /*!
         \param sKey key name
         \param pscene pointer to scene
         */
        virtual void AddScene(const QString& sKey, const TScenePtr& pscene);
        //! Get scene list
        /*!
         \return scene list
         */
        TSceneList& GetScenes();

        //! Create airplane
        /*!
         \return pointer to airplane
         */
        virtual QSharedPointer<CAirplane> CreateAirplane() const;
        //! Create meteophenomenon
        /*!
         \return pointer to meteophenomenon
         */
        virtual QSharedPointer<core::CMeteo> CreateMeteo() const;

        //! Add pointer to airplane
        /*!
         \param pAirplane pointer to airplane
         */
        virtual void AddAirplane(const QSharedPointer<CAirplane>& pAirplane);
        //! Delete pointer to airplane
        /*!
         \param pAirplane pointer to airplane
         \param bSend if true - unsubscribe from airplane(send obelix command)
         */
        virtual void DelAirplane(QSharedPointer<CAirplane> pAirplane, bool bSend = true);
        //! Update airplane information
        /*!
         \param AirplaneState airplane information
         */
        virtual void UpdateAirplane(CAirplaneState& AirplaneState);
        //! Bearing information
        /*!
         \param AirplaneState bearing information
         */
        virtual void BearingData(CBearingData& BearingData) {}
        //! Add pointer to meteophenomenon
        /*!
         \param pMeteo pointer to meteophenomenon
         */
        virtual void AddMeteo(const QSharedPointer<core::CMeteo>& pMeteo);
        //! Delete meteo on FIR
        virtual void DelMeteo(uint uiZoneID);
        //! Get airplanes
        /*!
         \return airplanes
         */
        const TAirplanesMap& GetAirplanes() const
        {
            return m_Airplanes;
        }
        //! Get airplanes
        /*!
         \return airplanes
         */
        TAirplanesMap& GetAirplanes()
        {
            return m_Airplanes;
        }
        //! Get meteophenomenon
        /*!
         \return meteophenomenon
         */
        TMeteoList& GetMeteo()
        {
            return m_Meteo;
        }
        //! Event of reminder update*/
        virtual void OnUpdateReminder(const QSharedPointer<CAirplane>& pPlane)
        {
        };

        //! New object received (in internal data fromat)
        void OnObjectReceived(const Xt::TObject& obj);

        //! Set STCA
        virtual void SetSTCA(AirplaneCollision& ac) {};

        //! Set pointer to Master
        /*!
         \param pMaster pointer to Master
         */
        virtual void SetMaster(CMaster* pMaster);
        //! Get central point of view
        /*!
         \return central point  of view
         */
        const QSharedPointer<CPoint>& GetCentralPoint() const
        {
            return m_pViewCentralPoint;
        }
        //! Get main FIR
        TScenePtr GetMainFIR() const;
        //! Find sectors by point and altitude
        Scene::TSceneList FindSectorsByPoint(const QString &point_name, double dAltitudeMeters = 0, bool bInside = false);
        Scene::TSceneList FindSectorsByPoint(const QStringList &fir_names_list, const QString &point_name, double dAltitudeMeters = 0, bool bInside = false);

        virtual void SetFormularDefaultPos();
        virtual void setFormularsPos(qreal angle, int length);

        void assumeAirplanes(const QString &current_sector);
        void releaseAirplanes(const QString &current_sector);

    protected:
        /*! Central point */
        QSharedPointer<CPoint> m_pViewCentralPoint;
        //! Scene list
        TSceneList m_SceneList;
        //! Main FIR
        TScenePtr m_MainFIR;

    private:
        void RemoveSTCA(quint16 idPlane);

        void checkDblCode(CAirplaneState &airplane_state, QSharedPointer<CAirplane> &airplane);
        void updateDblFlag(quint16 mode3a, bool dbl/*, const QColor &color*/);

    private:
        /*! Pointer to database */

        /*! Pointer to Master */
        CMaster* m_pMaster;
        /*! Airplanes */
        TAirplanesMap m_Airplanes;
        /*! Meteo */
        TMeteoList m_Meteo;

        Mode3AToTrackNum mode3a_to_track_num_;
        Mode3AToTrackInfo mode3a_to_track_info_;
        TrackNumToMode3A track_num_to_mode3a_;

        //Mode3AToDblColor mode3a_to_dbl_color_;
};
#endif // VIEW_H
