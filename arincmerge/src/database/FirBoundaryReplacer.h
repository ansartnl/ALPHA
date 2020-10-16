#ifndef _FIR_BOUNDARY_REPLACER_H__
#define _FIR_BOUNDARY_REPLACER_H__

#include <QtCore/QCoreApplication>
#include <QtCore/QMap>

class ArincNativeStorage;


//! Class used to replace arinc native storage with boundary points from
//! database (from similar FIR)
class FirBoundaryReplacer
{
    Q_DECLARE_TR_FUNCTIONS(FirBoundaryReplacer)

public:
    typedef QMap<QString, int>  TFirMap;

    FirBoundaryReplacer(int indexFir, ArincNativeStorage *st);

    //! Try to load identical to specified in constructor FIR in the database.
    TFirMap loadExisted() const;

    //! Read FIR's boundary points from database and replace them in the storage.
    bool replace(int id = -1);

private:
    /** Index of an intersected FIR from the list in the storage. */
    int                 mIndexFir;
    /** Reference to storage */
    ArincNativeStorage  *mStorage;
};

#endif // _FIR_BOUNDARY_REPLACER_H__
