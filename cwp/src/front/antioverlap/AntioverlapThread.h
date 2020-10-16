#ifndef PROCESSING_THREAD_H_
#define PROCESSING_THREAD_H_

#include <QSet>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QRectF>

#include "AntioverlapStrategy.h"


class AntioverlapThread : public QThread
{
    Q_OBJECT

public:

    explicit AntioverlapThread(const AntioverlapStrategyPtr &strategy, QObject *parent = 0)
        : QThread(parent), /*id_queue_(10000),*/ strategy_(strategy), stopped_(true)
    {}

    void stop();

    void add(quint16 id, const FormularInfo &formular_info);

    void remove(quint16 id);

    void setFixed(quint16 id, bool fixed);

    inline void setStrategy(const AntioverlapStrategyPtr &strategy) {strategy_ = strategy;}

    void setRepositionRect(const QRectF &rect);

signals:
    void repositionRequired(quint16 id, qreal angle, int length);

protected:

    void run();

private:

    AntioverlapStrategy::FormularInfos formular_infos_;

    QRectF reposition_rect_;

    QSet<quint16> id_queue_;

    AntioverlapStrategyPtr strategy_;

    volatile bool stopped_;

    QWaitCondition wait_condition_;

    QMutex formular_infos_mutex_;
};


#endif // PROCESSING_THREAD_H_
