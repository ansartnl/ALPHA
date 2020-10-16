#include "AntioverlapThread.h"

#include <QMutex>
#include <QMutexLocker>

void AntioverlapThread::add(quint16 id, const FormularInfo &formular_info)
{
    {
        QMutexLocker locker(&formular_infos_mutex_);
        formular_infos_[id] = formular_info;
        id_queue_.insert(id);
    }

    if (!stopped_)
        wait_condition_.wakeOne();
}

void AntioverlapThread::remove(quint16 id)
{
    {
        QMutexLocker locker(&formular_infos_mutex_);
        AntioverlapStrategy::FormularInfos::iterator it = formular_infos_.find(id);
        if (it != formular_infos_.end())
            formular_infos_.erase(it);
        id_queue_.remove(id);
    }

    if (!stopped_)
        wait_condition_.wakeOne();
}

void AntioverlapThread::setFixed(quint16 id, bool fixed)
{
    {
        QMutexLocker locker(&formular_infos_mutex_);
        AntioverlapStrategy::FormularInfos::iterator it = formular_infos_.find(id);
        if (it != formular_infos_.end())
            it->fixed = fixed;
    }

    if (!stopped_)
        wait_condition_.wakeOne();
}

void AntioverlapThread::setRepositionRect(const QRectF &reposition_rect)
{
    reposition_rect_ = reposition_rect;

    /*qreal width_half = reposition_rect_.width() / 2.f;
    qreal height_half = reposition_rect_.height() / 2.f;
    reposition_rect_.adjust(-width_half, -height_half, width_half, height_half);*/
}

void AntioverlapThread::stop()
{
    if (isRunning())
        stopped_ = true;

    wait_condition_.wakeOne();
}

void AntioverlapThread::run()
{
    stopped_ = false;
    qreal angle;
    int length;
    do {
        QMutexLocker locker(&formular_infos_mutex_);
        wait_condition_.wait(locker.mutex());

        while (!id_queue_.empty() && !stopped_) {
            quint16 id = *id_queue_.begin();
            id_queue_.remove(id);
            bool result = strategy_->separate(id, reposition_rect_, formular_infos_, angle, length);
            if (result)
                emit repositionRequired(id, angle, length);
        }

    } while (!stopped_);
}

