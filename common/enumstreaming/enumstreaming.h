#ifndef ENUMSTREAMING_H
#define ENUMSTREAMING_H

#include <QtCore/QDataStream>
#include <QtCore/QVariant>

/*!
  * \file enumstreaming.h
  * \brief Передача множеств по сети
  *
  * Содержит макрос, позволяющий сохранение/загрузку множеств в класс/из класса QVariant и,
  * как следствие, передачу множеств по сети с использованием класса QtTcpDatagramProtocolVariant.
  */

// simple macros
#define QT_DECLARE_ENUM_STREAMING(x) \
    inline QDataStream &operator<<(QDataStream &stream, const x &val) \
    { \
        stream << qint32(val); \
        return stream; \
    } \
    inline QDataStream &operator>>(QDataStream &stream, x &val) \
    { \
        qint32 temp; \
        stream >> temp; \
        val = x(temp); \
        return stream; \
    }
	
#endif
