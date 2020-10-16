#ifndef LAMMESSAGE_H
#define LAMMESSAGE_H

#include "AtsMessage.h"

/*!
 * \struct LamStruct
 * \brief Структура LAM
 *
 * Данная структура содержит все поля телеграммы LAM.
 */

struct LamStruct
{
};

class LamMessage : public AtsMessage
{
public:
    LamMessage(const LamStruct &lam);

    virtual QByteArray toAftn() const;
    virtual QByteArray toJson() const;

    virtual LamMessage * lam() const;

    LamStruct mLam;
};

#endif // LAMMESSAGE_H
