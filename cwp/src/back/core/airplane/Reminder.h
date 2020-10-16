#ifndef REMINDER_H
#define REMINDER_H

#include <QtCore/QSharedPointer>
#include <QDateTime>

//! Class contains the airplane's reminders
class CReminder
{
	CReminder();
public:
    enum ReminderType { FL, PassFL, Point, Local, Airport, SpecCodes, SPI
                        , TRACK, FL_BUST, CheckFL, Transfer_Com, Assume, Transfer
                        , TrHOP, TrROF, TimeOutROF, Restriction, LeaveRoute
                        , DoubleCode };
    enum ReminderLevel { Normal, Warning, Alarm };
    typedef QSharedPointer<CReminder> TReminder;
	typedef QList<TReminder> TListReminder;

	//! Constructor
	/*!
	  \param type a ReminderType of reminder
	  \param sValue a reminder value
	*/
	CReminder(ReminderType type, QString sValue);

    //! Primary key
    //@{
public:
    const quint32 &GetID() const { return m_ID; }
    void SetID(const quint32 &V) { m_ID = V; }
private:
    quint32 m_ID;
    //@}

    //! Reminder type
    //@{
public:
    const ReminderType &GetType() const { return m_Type; }
    void SetType(const ReminderType &V) { m_Type = V; }
private:
    ReminderType m_Type;
    //@}

    //! Reminder level
    //@{
public:
    const ReminderLevel &GetLevel() const { return m_Level; }
    void SetLevel(const ReminderLevel &V) { m_Level = V; }
private:
    ReminderLevel m_Level;
    //@}

    //! Reminder value
    //@{
public:
    const QString &GetValue() const { return m_Value; }
    void SetValue(const QString &V) { m_Value = V; }
private:
    QString m_Value;
    //@}

	//! Reminder status
    //@{
public:
    const bool &GetComplete() const { return m_Complete; }
    void SetComplete(const bool &V) { m_Complete = V; }
private:
    bool m_Complete;
    //@}

	//! Time when reminder completed
    //@{
public:
    const QDateTime &GetTime() const { return m_Time; }
    void SetTime(const QDateTime &V) { m_Time = V; }
private:
    QDateTime m_Time;
    //@}

    //! Can close reminder
    //@{
public:
    const bool IsClosable() const { return m_Closable; }
    void SetClosable(const bool V) { m_Closable = V; }
private:
    bool m_Closable;
    //@}

public:
	bool operator == (const CReminder& reminder) const;
	bool operator != (const CReminder& reminder) const;

public:
	//! Compare two reminder on less
	static bool LessReminder(const TReminder& pR1, const TReminder& pR2);
	//! Get displayed reminder name
	/*!
	  \param type a ReminderType
	  \return a string ( reminder name )
	*/
	static QString GetReminderName(ReminderType type);
};

//! Class describes the reminder's key
class CReminderKey
{
    CReminderKey();
public:
    //! Constructor
    /*!
     \param reminder a reminder for key
     */
    CReminderKey(const CReminder& reminder);
    CReminder::ReminderType GetType() const
    { return m_type; }
    QString GetValue() const
    { return m_value; }
    //! Comparison operator
    bool operator < (const CReminderKey& ref2nd) const;
private:
    CReminder::ReminderType m_type;
    QString                 m_value;
};

#endif // REMINDER_H
