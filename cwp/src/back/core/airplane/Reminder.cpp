#include "Reminder.h"

CReminder::CReminder(ReminderType type, QString sValue)
    : m_ID(0), m_Type(type), m_Level(Alarm), m_Value(sValue), m_Complete(false)
    , m_Closable(true)
{

}

QString CReminder::GetReminderName(ReminderType type)
{
	QString sText;
	switch(type)
	{
	case FL:
		sText = QObject::tr("Report FL");
		break;
	case PassFL:
		sText = QObject::tr("Report passing FL");
		break;
	case Point:
		sText = QObject::tr("Report point");
		break;
	case Local:
		sText = QObject::tr("Report established on localizer");
		break;
    case Airport:
        sText = QObject::tr("Report aerodrome in sight");
        break;
    case SpecCodes:
        sText = QObject::tr("Special codes");
        break;
    case SPI:
        sText = QObject::tr("SPI");
        break;
    case TRACK:
        sText = QObject::tr("TRACK");
        break;
    case FL_BUST:
        sText = QObject::tr("FL BUST");
        break;
    case CheckFL:
        sText = QObject::tr("Check FL");
        break;
    case Transfer_Com:
        sText = QObject::tr("Phone");
        break;
    case Assume:
        sText = QObject::tr("Assume");
        break;
    case Transfer:
        sText = QObject::tr("Transfer");
        break;
    case TrHOP:
        sText = QObject::tr("HOP");
        break;
    case TrROF:
        sText = QObject::tr("ROF");
        break;
    case TimeOutROF:
        sText = QObject::tr("Phone");
        break;
    case Restriction:
        sText = QObject::tr("R");
        break;
    case LeaveRoute:
        sText = QObject::tr("Leave route");
        break;
    case DoubleCode:
        sText = QObject::tr("Double code");
        break;
    }
	return sText;
}

bool CReminder::LessReminder(const TReminder& pR1, const TReminder& pR2)
{
	if(pR1 && pR2)
		return pR1->GetTime() < pR2->GetTime();
	return false;
}


bool CReminder::operator == (const CReminder& reminder) const
{
	return ((GetType() == reminder.GetType())
            && (GetLevel() == reminder.GetLevel())
            && (GetValue() == reminder.GetValue())
            && (GetComplete() == reminder.GetComplete())
            && (IsClosable() == reminder.IsClosable())
            && (GetTime() == reminder.GetTime()));
}

bool CReminder::operator != (const CReminder& reminder) const
{
	return !(*this == reminder);
}

CReminderKey::CReminderKey(const CReminder &reminder)
    : m_type(reminder.GetType()), m_value(reminder.GetValue())
{

}

bool CReminderKey::operator < (const CReminderKey& ref2nd) const
{
    if(GetType() != ref2nd.GetType())
        return GetType() < ref2nd.GetType();
    if(GetValue() != ref2nd.GetValue())
        return GetValue() < ref2nd.GetValue();
    return false;
}
