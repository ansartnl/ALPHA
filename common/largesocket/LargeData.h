#ifndef LARGEDATA_H
#define LARGEDATA_H

#include <QtCore/QByteArray>
#include <QtCore/QVariant>

/*!
 * \namespace LargeData
 *
 * \brief Передача больших данных
 */

namespace LargeData
{

/*!
 * \brief Размер заголовка
 */

const size_t HEADER_SIZE = 8;

/*!
 * \brief Константа заголовка
 *
 * Уникальная константа, искользуемая для отличия структуры \a DataPacket
 * от других типов данных, а так же для различия версий структуры
 */
const char SIG[HEADER_SIZE] = "ldp0002";

/*!
 * \brief Константа плохого идентификатора данных
 *
 * Данная константа служит для обозначения идентификатора данных,
 * который не может быть передан в пакете
 */
const quint32 BAD_IDENTIFIER = quint32(-1);

/*!
 * \brief Максимальный размер данных
 *
 * Максимальный размер куска, на которые делятся передаваемые данные.
 */
const size_t MAX_DATA_SIZE = 30000;



/*!
 * \struct DataPacket
 * \brief Структура пакета данных
 *
 * Данная структура содержит всю информацию, необходимую для передачи,
 * приема и склеивания данных большого объема.
 */

struct DataPacket
{
    /*!
     * \struct Header
     *
     * \brief Структура заголовка пакета данных
     *
     * Данная структура содержит всю служебную информацию пакета и имеет постоянный размер.
     */
    struct Header
    {
        /*!
         * \brief Сигнатура пакета
         *
         * Должна быть равна \a LargeData::SIG.
         */
        char signature[HEADER_SIZE];

        /*!
         * \brief Идентификатор данных
         *
         * Должен быть одинаков для всех пакетов в пределах одного передаваемого данного,
         * и различен для разных данных.
         */
        quint32 identifier;

        /*!
         * \brief Номер пакета
         *
         * Порядковый номер пакета в списке пакетов передаваемых данных.
         * Должен принимать значения от нуля до \a DataPacket::count-1.
         * Данные из пакетов склеиваются согласно порядковым номерам.
         */
        int number;

        /*!
         * \brief Количество пакетов
         *
         * Общее количество передаваемых пакетов для одного передаваемого данного.
         * Количество частей, на которые делится данное при передаче.
         */
        int count;

        /*!
         * \brief Размер данных
         *
         * Реальный размер переданного куска данных.
         * Не может превышать \a LargeData::MAX_DATA_SIZE
         */
        int size;
    };

    /*!
     * \brief Заголовок пакета данных
     */
    Header header;

    /*!
     * \brief Данные
     *
     * Переданный кусок данных.
     */
    char data[MAX_DATA_SIZE];
};

QByteArray toByteArray(const LargeData::DataPacket &data);
LargeData::DataPacket fromByteArray(const QByteArray &arr);

int packetSize(const QByteArray &arr);

bool fullPacket(const QByteArray &arr);

} // namespace LargeData

Q_DECLARE_METATYPE(LargeData::DataPacket)

QDataStream & operator<< (QDataStream &stream, const LargeData::DataPacket &data);
QDataStream & operator>> (QDataStream &stream, LargeData::DataPacket &data);

#endif // LARGEDATA_H
