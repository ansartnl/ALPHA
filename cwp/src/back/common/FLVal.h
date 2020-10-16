#ifndef FL_VAL_H
#define FL_VAL_H

#include <QtCore/QMetaType>
#include <QtCore/QString>

//! Flight level values
class FLVal
{
public:
    FLVal();
    //! Construct from string
    /*!
      \param sLevel as F300 or S0915
    */
    FLVal(const QString& sLevel);
    FLVal(double feet, double meters, bool bFeet);
    FLVal(double dVal, bool bFeet = true);

    inline operator double() const { return m_valF; }
    inline double GetF() const { return m_valF; }
    inline double GetM() const { return m_valM; }
    inline bool isFeet() const { return m_bIsFeet; }
    //! string value
    /*!
      \return string as F300 or S0915
    */
    QString toString() const;

private:
    /** FL (hundreds of feet) */
    double  m_valF;
    /** meters */
    double  m_valM;
    bool    m_bIsFeet;
};

Q_DECLARE_METATYPE(FLVal)

#endif // FL_VAL_H
