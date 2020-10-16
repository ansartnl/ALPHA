#include "dio.h"

namespace dio
{

/*!
 * \struct DioHeader
 */

DioHeader::DioHeader(const QByteArray &ba) :
    command(0)
  , version(kVersion)
  , status(0)
  , length(0)
  , lastError(E_NO)
{
    if (ba.size() < 4) {
        lastError = E_HEADER_TOO_SHORT;
        return;
    }
    if (ba.size() > 4) {
        lastError = E_HEADER_TOO_LONG;
        return;
    }
    if (kVersion != ba[1]) {
        lastError = E_VERSION_MISMATCH;
        return;
    }
    command = ba[0];
    status = ba[2];
    length = ba[3];
}

const QByteArray DioHeader::toByteArray() const
{
    QByteArray ret(4, '\0');
    ret[0] = command;
    ret[1] = version;
    ret[2] = status;
    ret[3] = length;
    return ret;
}

const DioHeader DioHeader::fromByteArray(const QByteArray &ba)
{
    return DioHeader(ba);
}

/*!
 * \struct DioPacket
 */

DioPacket::DioPacket(const QByteArray &ba) :
    lastError(E_NO)
{
    header = DioHeader::fromByteArray(ba.left(4));
    if (header.lastError != E_NO) {
        lastError = header.lastError;
        return;
    }
    if (header.length + 4 > ba.size()) {
        lastError = E_DATA_TOO_SHORT;
        return;
    }
    if (header.length + 4 < ba.size()) {
        lastError = E_DATA_TOO_LONG;
        return;
    }
    data = ba.mid(4);
}

const QByteArray DioPacket::toByteArray() const
{
    DioHeader h(header);
    h.length = data.size();
    return h.toByteArray() + data;
}

const DioPacket DioPacket::fromByteArray(const QByteArray &ba)
{
    return DioPacket(ba);
}

/*!
 * \struct ReadStateCommand
 */

ReadStateCommand::ReadStateCommand(Channel c) :
    DioPacket()
{
    header.command = C_READ_STATE;
    header.length = 1;
    data.resize(header.length);
    data[0] = c;
}

/*!
 * \struct WriteStateCommand
 */

WriteStateCommand::WriteStateCommand(Channel c, ModeInOut mio, ModeLowHigh mlh) :
    DioPacket()
{
    header.command = C_WRITE_STATE;
    header.length = 3;
    data.resize(header.length);
    data[0] = c;
    data[1] = mio;
    if (mio == MIO_OUT)
        data[2] = mlh;
    else
        data[2] = MLH_NOT_RELEVANT;
}

} // namespace dio
