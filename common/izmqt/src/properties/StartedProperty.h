#ifndef IZMQT_STARTED_PROPERTY_H
#define IZMQT_STARTED_PROPERTY_H

namespace izmqt {

//! Property state holder (started state - shows start/stop client/server state).
class StartedProperty {
public:
    //! Returns true if property was set.
    inline bool isStarted() const { return mIsStarted; }

protected:
    StartedProperty()
        : mIsStarted(false)
    {}

    //! Set new property state.
    inline void setStarted(bool on) { mIsStarted = on; }

private:
    /** True when state is on */
    bool        mIsStarted;
};

}   // namespace izmqt

#endif // IZMQT_STARTED_PROPERTY_H
