#ifndef DIO_H
#define DIO_H

#include <QtCore/QString>

/*!
 * \namespace dio
 * \brief DIO команды
 */

namespace dio
{

const quint8 kVersion = 2;

/*!
 * \enum Error
 */

enum Error {
    E_NO = 0
    , E_VERSION_MISMATCH
    , E_HEADER_TOO_SHORT
    , E_HEADER_TOO_LONG
    , E_DATA_TOO_SHORT
    , E_DATA_TOO_LONG
};

inline const QString errorString(Error e)
{
    switch (e) {
    case E_NO:
        return "No error";
    case E_VERSION_MISMATCH:
        return "Version is not supported";
    case E_HEADER_TOO_SHORT:
        return "Header is less than 4 bytes";
    case E_HEADER_TOO_LONG:
        return "Header is more than 4 bytes";
    case E_DATA_TOO_SHORT:
        return "Data is shorter than header.length";
    case E_DATA_TOO_LONG:
        return "Data is longer than header.length";
    }
    return "Unhandled error";
}

/*!
 * \enum Status
 * \brief Код ошибки, возвращаемой устройством
 */

enum Status {
    S_OK = 0
    , S_COMMAND_ERROR = 1
    , S_VERSION_ERROR = 2
    , S_LENGTH_ERROR = 3
    , S_ACTION_ERROR = 4
    , S_PACKET_TOO_SHORT_ERROR = 5
    , S_DIO_NUMBER_ERROR = 6
    , S_UNKNOWN_ERROR = 0xFF
};

inline const QString statusString(Status s)
{
    switch (s) {
    case S_OK:
        return "Ok";
    case S_COMMAND_ERROR:
        return "Command error";
    case S_VERSION_ERROR:
        return "Version is not supported";
    case S_LENGTH_ERROR:
        return "Lenght error";
    case S_ACTION_ERROR:
        return "Bad status or unhandled mode";
    case S_PACKET_TOO_SHORT_ERROR:
        return "Packet is too short";
    case S_DIO_NUMBER_ERROR:
        return "DIO number is not supported";
    case S_UNKNOWN_ERROR:
        return "Unknown error";
    }
    return "Unhandled status";
}

/*!
 * \enum Channel
 */

enum Channel {
    CH_0 = 0
    , CH_1 = 1
    , CH_2 = 2
    , CH_3 = 3
};

/*!
 * \enum ModeInOut
 */

enum ModeInOut {
    MIO_IN = 0
    , MIO_OUT = 1
};

/*!
 * \enum ModeLowHigh
 *
 * Только, когда \a ModeInOut = MIO_OUT
 */

enum ModeLowHigh {
    MLH_LOW = 0
    , MLH_HIGH = 1

    , MLH_NOT_RELEVANT = 0
};

/*!
 * \enum Command
 */

enum Command {
    C_READ_STATE = 1
    , C_WRITE_STATE = 2
//    , C_READ_STATES = 5
//    , C_WRITE_STATES = 6
};

/*!
 * \struct DioHeader
 */

struct DioHeader
{
    quint8 command;
    quint8 version;
    quint8 status;
    quint8 length;

    Error lastError;

    DioHeader() :
        command(0)
      , version(kVersion)
      , status(0)
      , length(0)
      , lastError(E_NO)
    {}
    DioHeader(quint8 c) :
        command(c)
      , version(kVersion)
      , status(0)
      , length(0)
      , lastError(E_NO)
    {}
    DioHeader(const QByteArray &ba);

    const QByteArray toByteArray() const;
    static const DioHeader fromByteArray(const QByteArray &ba);
};

/*!
 * \struct DioPacket
 */

struct DioPacket
{
    DioHeader header;
    QByteArray data;

    Error lastError;

    DioPacket() :
        lastError(E_NO)
    {}
    DioPacket(const QByteArray &ba);

    const QByteArray toByteArray() const;
    static const DioPacket fromByteArray(const QByteArray &ba);
};

/*!
 * \struct ReadStateCommand
 */

struct ReadStateCommand : public DioPacket
{
    ReadStateCommand(Channel c);
};

/*!
 * \struct WriteStateCommand
 */

struct WriteStateCommand : public DioPacket
{
    WriteStateCommand(Channel c, ModeInOut mio, ModeLowHigh mlh = MLH_NOT_RELEVANT);
};

} // namespace dio

#endif // DIO_H
