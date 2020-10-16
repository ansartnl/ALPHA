#ifndef COORD_EDIT_H
#define COORD_EDIT_H

#include <QLineEdit>
#include <QRegExpValidator>

namespace coordcommon
{
    QString getDegree();
}

//! Class extends QLineEdit for coordinates
class CoordEdit
    : public QLineEdit
{
    Q_OBJECT
public:
    enum CoordType { Latitude, Longitude, LatitudeLongitude, Azimuth };

    //! Constructor
    /*!
        \param type a type of coordinate
        \param parent a parent object
    */
    CoordEdit(const CoordType type, QWidget *parent = 0);

    QVariant getValue() const;
    void setValue(const QVariant& value);

    static QVariant fromString(const CoordType type, const QString& text);
    static QString toString(const CoordType type, const QVariant& value, const bool bUsePlus = false);

    //@{
    //! Inherited from QLineEdit
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
    //@}

private:
    CoordType m_CoordType;
};

class CoordValidator
    : public QRegExpValidator
{
    Q_OBJECT
public:
    //! Constructor
    /*!
        \param type a type of coordinate
        \param parent a parent object
    */
    CoordValidator(const CoordEdit::CoordType type, QWidget *parent = 0);
    //! Inherited from QRegExpValidator
    virtual QValidator::State validate(QString& input, int& pos) const;

    bool isEmpty(const QString& input) const;

private:
    CoordEdit::CoordType m_CoordType;
};

#endif // COORD_EDIT_H
