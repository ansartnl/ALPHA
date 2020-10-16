#ifndef _RE_TYPES_H__
#define _RE_TYPES_H__

namespace rewarn {

//! Restriction type
enum ReType
{
    enAlert = 'A',          //!< Alert
    enCaution = 'C',        //!< Caution
    enDanger = 'D',         //!< Danger
    enMilitary = 'M',       //!< Military
    enProhibited = 'P',     //!< Prohibited
    enRestricted = 'R',     //!< Restricted
    enTraining = 'T',       //!< Training
    enWarning = 'W',        //!< Warning
    enUser = 'X',           //!< User defined restriction zone
    enUserObstacle = 'Y',   //!< User defined obstacle
    enUserNFZ = 'Z',        //!< User defined "No Conflict" zone
    enUnknown = 'U'         //!< Unknown
};

}   // namespace rewarn {

#endif // _RE_TYPES_H__
