#include "asterix.h"
#include "shareutility.h"
#include "asterixdata.h"

#include "eurocontrol/asterix001euro.h"
#include "eurocontrol/asterix002euro.h"
#include "eurocontrol/asterix004euro.h"
#include "eurocontrol/asterix008euro.h"
#include "eurocontrol/asterix020euro.h"
#include "eurocontrol/asterix021euro.h"
#include "eurocontrol/asterix034euro.h"
#include "eurocontrol/asterix048euro.h"
#include "eurocontrol/asterix062euro.h"
#include "eurocontrol/asterix063euro.h"
#include "eurocontrol/asterix253euro.h"

#include "aerotechnica/asterix001aero.h"
#include "aerotechnica/asterix002aero.h"
#include "aerotechnica/asterix062aero.h"
#include "aerotechnica/asterix250aero.h"
#include "aerotechnica/asterix253aero.h"
#include "aerotechnica/asterix254aero.h"

#include "peleng/asterix241peleng.h"
#include "peleng/asterix062peleng.h"
#include "peleng/asterix253peleng.h"


Factory<Asterix, AsterixKey> asterixFactory;

const QString Asterix::SAC = ASTERIX_SAC;
const QString Asterix::SIC = ASTERIX_SIC;

const QString Asterix::SpecialPurposeField = "asterix.SpecialPurposeField";
const QString Asterix::ReservedExpansionField = "asterix.ReservedExpansionField";

Asterix::Asterix()
    : mLength(0)
{}

Asterix::~Asterix()
{}

void Asterix::parse_sp(QDataStream &stream, AsterixData &map)
{
    quint8 len = read<quint8>(stream);
    if (1 < len)
        map.insert(SpecialPurposeField, readArray(stream, len - 1));
    else
        map.appendError(AsterixErrors::Error_SPFieldInvalidLength);
}

void Asterix::parse_re(QDataStream &stream, AsterixData &map)
{
    quint8 len = read<quint8>(stream);
    if (0 != len)
        map.insert(ReservedExpansionField, readArray(stream, len - 1));
    else
        map.appendError(AsterixErrors::Error_REFieldInvalidLength);
}

bool Asterix::generate_sp(AsterixData &map, QDataStream &stream)
{
    if (map.contains(SpecialPurposeField))
    {
        QByteArray data = map.value(SpecialPurposeField).toByteArray();
        if (!data.isEmpty())
        {
            quint8 size = data.size() + 1;
            if (size != data.size() + 1)
            {
                write8(stream, size);
                writeArray(stream, data);
                return true;
            }
            map.appendError(AsterixErrors::Error_SPFieldInvalidLength);
        }
    }
    return false;
}

bool Asterix::generate_re(AsterixData &map, QDataStream &stream)
{
    if (map.contains(ReservedExpansionField))
    {
        QByteArray data = map.value(ReservedExpansionField).toByteArray();
        if (!data.isEmpty())
        {
            quint8 size = data.size() + 1;
            if (size != data.size() + 1)
            {
                write8(stream, size);
                writeArray(stream, data);
                return true;
            }
            map.appendError(AsterixErrors::Error_REFieldInvalidLength);
        }
    }
    return false;
}

AsterixErrorMandatory *Asterix::mandatory(const QString &field)
{
    return new AsterixErrorMandatory(category(), field);
}

bool Asterix::isRegistered(int cat, AsterixVariety variety)
{
    return asterixFactory.isRegistered(AsterixKey(variety, cat));
}

void Asterix::registre()
{
    // Eurocontrol standard protocols
    asterixFactory.reg<Eurocontrol::Asterix001>(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix001::mCategory));
    asterixFactory.reg<Eurocontrol::Asterix002>(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix002::mCategory));
    asterixFactory.reg<Eurocontrol::Asterix004>(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix004::mCategory));
    asterixFactory.reg<Eurocontrol::Asterix008>(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix008::mCategory));
    asterixFactory.reg<Eurocontrol::Asterix020>(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix020::mCategory));
    asterixFactory.reg<Eurocontrol::Asterix021>(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix021::mCategory));
    asterixFactory.reg<Eurocontrol::Asterix034>(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix034::mCategory));
    asterixFactory.reg<Eurocontrol::Asterix048>(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix048::mCategory));
    asterixFactory.reg<Eurocontrol::Asterix062>(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix062::mCategory));
    asterixFactory.reg<Eurocontrol::Asterix063>(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix063::mCategory));
    asterixFactory.reg<Eurocontrol::Asterix253>(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix253::mCategory));

    // Aerotechnica standard protocols
    asterixFactory.reg<Aerotechnica::Asterix001>(AsterixKey(AsterixAerotechnica, Aerotechnica::Asterix001::mCategory));
    asterixFactory.reg<Aerotechnica::Asterix002>(AsterixKey(AsterixAerotechnica, Aerotechnica::Asterix002::mCategory));
    asterixFactory.reg<Aerotechnica::Asterix062>(AsterixKey(AsterixAerotechnica, Aerotechnica::Asterix062::mCategory));
    asterixFactory.reg<Aerotechnica::Asterix250>(AsterixKey(AsterixAerotechnica, Aerotechnica::Asterix250::mCategory));
    asterixFactory.reg<Aerotechnica::Asterix253>(AsterixKey(AsterixAerotechnica, Aerotechnica::Asterix253::mCategory));
    asterixFactory.reg<Aerotechnica::Asterix254>(AsterixKey(AsterixAerotechnica, Aerotechnica::Asterix254::mCategory));

    // Peleng standard protocols
    asterixFactory.reg<Peleng::Asterix241>(AsterixKey(AsterixPeleng, Peleng::Asterix241::mCategory));
    asterixFactory.reg<Peleng::Asterix062>(AsterixKey(AsterixPeleng, Peleng::Asterix062::mCategory));
    asterixFactory.reg<Peleng::Asterix253>(AsterixKey(AsterixPeleng, Peleng::Asterix253::mCategory));
}

void Asterix::unRegistre()
{
    // Eurocontrol standard protocols
    asterixFactory.unRegister(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix001::mCategory));
    asterixFactory.unRegister(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix002::mCategory));
    asterixFactory.unRegister(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix004::mCategory));
    asterixFactory.unRegister(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix008::mCategory));
    asterixFactory.unRegister(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix020::mCategory));
    asterixFactory.unRegister(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix021::mCategory));
    asterixFactory.unRegister(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix034::mCategory));
    asterixFactory.unRegister(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix048::mCategory));
    asterixFactory.unRegister(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix062::mCategory));
    asterixFactory.unRegister(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix063::mCategory));
    asterixFactory.unRegister(AsterixKey(AsterixEurocontrol, Eurocontrol::Asterix253::mCategory));

    // Aerotechnica standard protocols
    asterixFactory.unRegister(AsterixKey(AsterixAerotechnica, Aerotechnica::Asterix001::mCategory));
    asterixFactory.unRegister(AsterixKey(AsterixAerotechnica, Aerotechnica::Asterix002::mCategory));
    asterixFactory.unRegister(AsterixKey(AsterixAerotechnica, Aerotechnica::Asterix062::mCategory));
    asterixFactory.unRegister(AsterixKey(AsterixAerotechnica, Aerotechnica::Asterix250::mCategory));
    asterixFactory.unRegister(AsterixKey(AsterixAerotechnica, Aerotechnica::Asterix253::mCategory));
    asterixFactory.unRegister(AsterixKey(AsterixAerotechnica, Aerotechnica::Asterix254::mCategory));

    // Peleng standard protocols
    asterixFactory.unRegister(AsterixKey(AsterixPeleng, Peleng::Asterix241::mCategory));
    asterixFactory.unRegister(AsterixKey(AsterixPeleng, Peleng::Asterix062::mCategory));
    asterixFactory.unRegister(AsterixKey(AsterixPeleng, Peleng::Asterix253::mCategory));
}
