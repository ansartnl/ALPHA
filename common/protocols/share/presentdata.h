#ifndef PRESENTDATA_H
#define PRESENTDATA_H

#include <QtCore/QDataStream>

class PresentData
{
public:
    PresentData();
    PresentData(QDataStream& stream);
    bool isSet(int bit) const;
    void set(int bit);
    inline bool isEmpty() const { return m_data.isEmpty(); }
    inline const QByteArray& data() const { return m_data; }
    inline int size() const { return m_data.size(); }
private:
    QByteArray m_data;

    friend QDataStream & operator<< (QDataStream& stream, const PresentData& out);
    friend QDataStream & operator>> (QDataStream& stream, PresentData& in);
};

#endif // PRESENTDATA_H
