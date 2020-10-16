#ifndef ITEMHANDLER_H
#define ITEMHANDLER_H

//! Interface to items
class ItemHandler
{
public:
    ItemHandler(): m_bActive(true){};
    virtual ~ItemHandler(){};

    //! Check whether item is active
    /*!
     \return true if item is active
    */
    virtual bool IsItemActive() const { return m_bActive; }
    //! Set item activity
    /*!
        \param bActive item activity
    */
    virtual void MarkItemActive(bool bActive) { m_bActive = bActive;}
    //! Set item activity & all subelemnts
    /*!
        \param bActive item activity
    */
    virtual void MarkItemAllActive(bool bActive) { MarkItemActive(bActive); }

private:
    bool        m_bActive;
};

#endif // ITEMHANDLER_H
