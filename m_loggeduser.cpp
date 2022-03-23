#include "m_loggeduser.h"

m_loggedUser::m_loggedUser(QObject *parent) : QObject(parent)
{

}

quint8 m_loggedUser::getId() const
{
    return id;
}

void m_loggedUser::setId(const quint8 &value)
{
    id = value;
}

QString m_loggedUser::getName() const
{
    return name;
}

void m_loggedUser::setName(const QString &value)
{
    name = value;
}

QString m_loggedUser::getPassword() const
{
    return password;
}

void m_loggedUser::setPassword(const QString &value)
{
    password = value;
}

QString m_loggedUser::getSessionId() const
{
    return sessionId;
}

void m_loggedUser::setSessionId(const QString &value)
{
    sessionId = value;
}

QDateTime m_loggedUser::getLoginTimeStamp() const
{
    return loginTimeStamp;
}

void m_loggedUser::setLoginTimeStamp(const QDateTime &value)
{
    loginTimeStamp = value;
}

bool m_loggedUser::getIsLogged() const
{
    return isLogged;
}

void m_loggedUser::setIsLogged(bool value)
{
    isLogged = value;
}
