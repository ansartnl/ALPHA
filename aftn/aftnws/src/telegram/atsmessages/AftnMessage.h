#ifndef AFTNMESSAGE_H_2
#define AFTNMESSAGE_H_2

#include <QtCore/QStringList>
#include <QtCore/QVariant>

struct AftnStruct
{
    QString transmissionIdentificator;  // 3 letters + 3 digits
    QString additionServiceIndication;  // 0-10 characters
    QString priorityIndicator;          // 2 letters: KK FF GG DD SS
    QStringList receiverAddresses;      // 8 letters each
    // filling time: 6 digits: ddhhmm
    int fillingDate;
    int fillingHour;
    int fillingMinute;
    QString senderAddress;              // 8 letters

    // beggining of the text
    QString text; // any text

    QString confirmation;               // text
    QString correction;                 // text
    int version;
};
Q_DECLARE_METATYPE(AftnStruct)

inline QDataStream & operator<< (QDataStream &stream, const AftnStruct &msg)
{
    stream << msg.transmissionIdentificator;
    stream << msg.additionServiceIndication;
    stream << msg.priorityIndicator;
    stream << msg.receiverAddresses;
    stream << msg.fillingDate;
    stream << msg.fillingHour;
    stream << msg.fillingMinute;
    stream << msg.senderAddress;
    stream << msg.text;
    stream << msg.confirmation;
    stream << msg.correction;
    stream << msg.version;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AftnStruct &msg)
{
    stream >> msg.transmissionIdentificator;
    stream >> msg.additionServiceIndication;
    stream >> msg.priorityIndicator;
    stream >> msg.receiverAddresses;
    stream >> msg.fillingDate;
    stream >> msg.fillingHour;
    stream >> msg.fillingMinute;
    stream >> msg.senderAddress;
    stream >> msg.text;
    stream >> msg.confirmation;
    stream >> msg.correction;
    stream >> msg.version;
    return stream;
}

class AftnMessage
{
public:
    AftnMessage(const AftnStruct &aftn);

    QByteArray toByteArray() const;

    AftnStruct mAftn;
};

#endif // AFTNMESSAGE_H_2
