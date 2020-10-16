#ifndef EDITHEADERINTERFACE_H1
#define EDITHEADERINTERFACE_H1

#include <QtCore/QDataStream>
#include <QtCore/QStringList>

/*!
 * \struct EditHeaderItem
 * \brief Элемент заголовка
 *
 * \a identifier - произвольный идентификатор
 * \a texts - список текстов на разных языках
 * \a number - порядковый номер элемента в списке для показа (начиная с 1)
 * \a visible - показывать ли данный элемент в заголовке
 */

struct EditHeaderItem
{
    static const int version = 1;
    QString identifier;
    QStringList texts;
    int number;
    bool visible;
};

inline QDataStream & operator<< (QDataStream &stream, const EditHeaderItem &msg)
{
    stream << msg.version;
    stream << msg.identifier;
    stream << msg.texts;
    stream << msg.number;
    stream << msg.visible;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, EditHeaderItem &msg)
{
    int version;
    stream >> version;
    if (version != msg.version)
        return stream;
    stream >> msg.identifier;
    stream >> msg.texts;
    stream >> msg.number;
    stream >> msg.visible;
    return stream;
}

inline void swap(EditHeaderItem &i1, EditHeaderItem &i2)
{
    qSwap(i1.identifier, i2.identifier);
    qSwap(i1.texts, i2.texts);
    qSwap(i1.visible, i2.visible);

//    EditHeaderItem tmp = i2;

//    i2.identifier = i1.identifier;
//    i2.texts = i1.texts;
//    i2.visible = i1.visible;

//    i1.identifier = tmp.identifier;
//    i1.texts = tmp.texts;
//    i1.visible = tmp.visible;
}

/*!
 * \struct EditHeaderItems
 * \brief Список элементов заголовка
 */

typedef QList < EditHeaderItem > EditHeaderItems;

/*!
 * \struct EditHeaderIntreface
 * \brief Интерфейс редактирования заголовка
 *
 * Данный интерфейс необходим для редактирования списка элементов заголовка. Указатель на объект класса,
 * реализовавшего данный интерфейс, надо передать объекту заголовка через функцию \a setEditHeaderInterface().
 */

class EditHeaderInterface
{
public:
    EditHeaderInterface() {}
    virtual ~EditHeaderInterface() {}

    virtual QStringList languages() const = 0;
    virtual EditHeaderItems items() const = 0;
    virtual void setItems(const EditHeaderItems &) = 0;
};

#endif // EDITHEADERINTERFACE_H1
