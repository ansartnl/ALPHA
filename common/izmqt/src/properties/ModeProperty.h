#ifndef IZMQT_MODE_PROPERTY_H
#define IZMQT_MODE_PROPERTY_H

namespace izmqt {

enum {
    CLIENT = 1,
    SERVER
};

//! Property state holder (client/server mode of connection).
class ModeProperty {
public:
    //! Returns current mode.
    inline int mode() const { return mMode; }

protected:
    ModeProperty(int mode = CLIENT)
        : mMode(mode)
    {}

    //! Set new state.
    inline void setMode(int m) { mMode = m; }

private:
    /** Client/server mode */
    int     mMode;
};

}   // namespace izmqt

#endif // IZMQT_MODE_PROPERTY_H
