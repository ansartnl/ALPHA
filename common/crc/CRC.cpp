#include "CRC.h"

#include "lib_crc.h"


namespace CRC {

    ushort crc16(const QByteArray &ba)
    {
        ushort crc = 0;
        for ( int i = 0; i < ba.length(); i++ )
            crc = update_crc_16(crc, ba[i]);
        return crc;
    }

    ushort crc16Modbus(const QByteArray &ba)
    {
        ushort crc = 0xffff;
        for ( int i = 0; i < ba.length(); i++ )
            crc = update_crc_16(crc, ba[i]);
        return crc;
    }

    ushort crcDnp(const QByteArray &ba)
    {
        ushort crc = 0;
        for ( int i = 0; i < ba.length(); i++ )
            crc = update_crc_dnp(crc, ba[i]);
        return crc;
    }

    ushort crcSick(const QByteArray &ba)
    {
        ushort crc = 0;
        char prev_byte = 0;
        for ( int i = 0; i < ba.length(); i++ )
        {
            crc = update_crc_sick(crc, ba[i], prev_byte);
            prev_byte = ba[i];
        }
        return crc;
    }

    ushort crcCCITT(ushort crc, const QByteArray &ba)
    {
        for ( int i = 0; i < ba.length(); i++ )
            crc = update_crc_ccitt(crc, ba[i]);
        return crc;
    }

    ushort crcCCITT_0000(const QByteArray &ba)
    {
        ushort crc = 0;
        for ( int i = 0; i < ba.length(); i++ )
            crc = update_crc_ccitt(crc, ba[i]);
        return crc;
    }

    ushort crcCCITT_FFFF(const QByteArray &ba)
    {
        ushort crc = 0xffff;
        for ( int i = 0; i < ba.length(); i++ )
            crc = update_crc_ccitt(crc, ba[i]);
        return crc;
    }

    ushort crcCCITT_1D0F(const QByteArray &ba)
    {
        ushort crc = 0x1d0f;
        for ( int i = 0; i < ba.length(); i++ )
            crc = update_crc_ccitt(crc, ba[i]);
        return crc;
    }

    ushort crcKermit(const QByteArray &ba)
    {
        ushort crc = 0;
        for ( int i = 0; i < ba.length(); i++ )
            crc = update_crc_kermit(crc, ba[i]);
        return crc;
    }

    ulong crc32(const QByteArray &ba)
    {
        ulong crc = 0xffffffffL;
        for ( int i = 0; i < ba.length(); i++ )
            crc = update_crc_32(crc, ba[i]);
        return crc;
    }

}
