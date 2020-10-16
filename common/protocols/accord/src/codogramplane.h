#ifndef ACCORD_CODOGRAM_PLANE_H
#define ACCORD_CODOGRAM_PLANE_H

#include "accord.h"


//! Base class for Codogram 2 and 3 of Аккорд-ССПД protocol.
class CodogramPlane : public Accord
{
public:
    // Канал  ответа  ВРЛ (1-е, 2-е  ВС)
    enum EChannel
    {
        International,  //!< международный
        Domestic        //!< отечественный
    };

    // Вид  информации. Сообщения  о  координатах  ВС.
    enum EInformationType
    {
        IT_XY   = 1,    //!< X, Y
        IT_XYIB,        //!< X, Y, ИБ
        IT_XYNB,        //!< X,Y, НБ
        IT_XYIBNB       //!< X, Y, ИБ, НБ
    };

    // Бедствие
    enum EDisaster
    {
        Dis_None,
        Dis_Attack,         //!< нападение на экипаж
        Dis_RadioCommLose,  //!< потеря радиосвязи
        Dis_aster           //!< бедствие
    };

    // Неразрешение (по 1-му, 2-му  ВС)
    enum EProhibition
    {
        Proh_None           = 0,    //!< отсутствует
        Proh_FristChannel   = 2,    //!< по  первичному  каналу
        Proh_SecondChannel  = 1     //!< по  вторичному  каналу
    };

    // Источник информации о (1, 2) ВС
    enum EInformationSource
    {
        IS_PRL      = 0x02,     //!< ПРЛ
        IS_VRL      = 0x04,     //!< ВРЛ
        IS_PRL_VRL  = 0x06      //!< ПРЛ + ВРЛ
    };

protected:
    bool testInformationType(quint8 value) const;
    bool testInformationSource(quint8 value) const;
    bool testProhibition(quint8 value) const;
    bool testDisaster(quint8 value) const;
};

#endif // ACCORD_CODOGRAM_PLANE_H
