#ifndef COMPARE_H
#define COMPARE_H

#include <QtCore/QVariantHash>
#include <QtCore/QVariantList>
#include <QtCore/QVariantMap>

/*!
  * \namespace Compare
  * \brief Функции сравнения
  *
  * Предоставляет более точные, но, вероятно, более медленные функции сравнения
  * типов данных, связанных с QVariant, не использующие информацию о внутреннем
  * устройстве данного типа.
  */

namespace Compare
{

#ifndef REG_COMPARE
#define REG_COMPARE(T) \
inline bool eq(const T &v1, const T &v2) { return equal(v1, v2); } \
inline bool notEqual(const T &v1, const T &v2) { return !equal(v1, v2); } \
inline bool notEq(const T &v1, const T &v2) { return !equal(v1, v2); }
#endif // REG_COMPARE

bool equal(const QVariant &v1, const QVariant &v2);
REG_COMPARE(QVariant)

bool equal(const QVariantList &v1, const QVariantList &v2);
REG_COMPARE(QVariantList)

bool alike(const QVariantList &v1, const QVariantList &v2);
inline bool notAlike(const QVariantList &v1, const QVariantList &v2)
{
    return !alike(v1, v2);
}

bool equal(const QVariantHash &v1, const QVariantHash &v2);
REG_COMPARE(QVariantHash)

bool equal(const QVariantMap &v1, const QVariantMap &v2);
REG_COMPARE(QVariantMap)

} //namespace

#endif // COMPARE_H
