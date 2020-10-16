#ifndef HARDWAREPROVIDER_H
#define HARDWAREPROVIDER_H

#include "HardwareId.h"

class HardwareProvider
{
public:
    HardwareId getHardwareId();

private:
    virtual void fillBoardId(HardwareId &id);
    virtual void fillDriveId(HardwareId &id);
    virtual void fillEthernetId(HardwareId &id);
    virtual void fillMemoryId(HardwareId &id);

    QString readLineFromFile(const QString &path);
};

#endif // HARDWAREPROVIDER_H
