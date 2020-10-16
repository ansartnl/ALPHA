#include "ReFir.h"

namespace rewarn {

void ReFir::prepare()
{
    mBoundingBox = QRectF();

    for (auto it = mAreas.begin(); it != mAreas.end(); ++it)
    {
        mBoundingBox = mBoundingBox.united(it->mBoundingBox);
    }
    for (auto it = mMeteoAreas.begin(); it != mMeteoAreas.end(); ++it)
    {
        mBoundingBox = mBoundingBox.united(it->mBoundingBox);
    }
}

}   // namespace rewarn {
