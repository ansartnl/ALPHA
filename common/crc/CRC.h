#ifndef _CRC_H__
#define _CRC_H__

#include <QtCore/QByteArray>

/*!
  * \namespace CRC
  * \brief Используется для рассчета CRC (Cyclic redundancy check)
  *
  * Возможны следующие алгоритмы:
  * - CRC-16
  * - CRC-16 (Modbus)
  * - CRC-16 (Sick)
  * - CRC-CCITT (XModem)
  * - CRC-CCITT (0xFFFF)
  * - CRC-CCITT (0x1D0F)
  * - CRC-CCITT (Kermit)
  * - CRC-DNP
  * - CRC-32
  */

namespace CRC {

    // \param ba Input array of ASCII symbols.
    // \param crc Initial crc value

    ushort crc16(const QByteArray &ba);
    ushort crc16Modbus(const QByteArray &ba);
    ushort crcDnp(const QByteArray &ba);
    ushort crcSick(const QByteArray &ba);
    ushort crcSick(const QByteArray &ba);
    ushort crcCCITT(ushort crc, const QByteArray &ba);
    ushort crcCCITT_0000(const QByteArray &ba);
    ushort crcCCITT_FFFF(const QByteArray &ba);
    ushort crcCCITT_1D0F(const QByteArray &ba);
    ushort crcKermit(const QByteArray &ba);
    ulong crc32(const QByteArray &ba);

}

#endif // _CRC_H__
