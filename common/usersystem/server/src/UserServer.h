#ifndef USERSERVER_H
#define USERSERVER_H

#include "usProto.h"

#include <QtCore/QObject>
#include <QtCore/QVariant>

namespace UserSystem
{

class Transport;

class UserServer : public QObject
{
    Q_OBJECT
public:
    explicit UserServer(QObject *parent = 0);

    void addTransport(Transport *transport);
    void removeTransport(Transport *transport);

    void fromByteArray(const QByteArray &groups, const QByteArray &users);
    QByteArray groupsToByteArray();
    QByteArray usersToByteArray();

    void addGroup(Group *group);

    User * user(const QString &name) const;

    bool checkPermission(const QString &user, const Permission &p) const;
    bool checkPermissions(const QString &user, const Permissions &p) const;
    bool userIsInGroup(const QString &user, const QString &g) const;

    void sendReply(Transport *t, const QVariant &reply);

    bool groupsIsEmpty();

protected slots:
    void onReceived(const QVariant &data);

signals:
    void groupsEmpty();
    void logined(UserSystem::Transport *t, const QString &name);
    void logouted(UserSystem::Transport *t, const QString &name);

    void received(UserSystem::Transport *t, const QString &name, const QVariant &data);

    void dataReceived(const QVariant &data);

protected:
    bool tryProceedStandard(Transport *t, const QString &name, const QVariant &request);
    void login_p(Transport *t, const QString &name, const QString &password);
    void logout_p(Transport *t, const QString &name);

    void clearUser(const QString &name);
    void createRoot();

protected:
    Transport *mTransport;
    QList < Transport * > mTransportPool;
    Users mUsers;
    Groups mGroups;
};

UserServer * userServer(QObject *parent = 0);

} // namespace

#endif // USERSERVER_H
