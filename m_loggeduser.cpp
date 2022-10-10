#include "m_loggeduser.h"

m_loggedUser::m_loggedUser(QObject *parent) : QObject(parent)
{

}

qint32 m_loggedUser::getId() const
{
    return id;
}

void m_loggedUser::setId(const qint32 &value)
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

QString m_loggedUser::getEncryptedPassword(bool toHex)
{
    QByteArray encryptedPassword;

    QByteArray encryptionSeed;
    QDataStream encryptionStream(&encryptionSeed, QIODevice::ReadWrite);
    encryptionStream.setVersion(QDataStream::Qt_6_0);
    encryptionStream << QString("test") << getPassword();

    QCryptographicHash hasher(QCryptographicHash::Md5);
    hasher.addData(encryptionSeed);
    encryptedPassword = hasher.result();

    return (toHex ? encryptedPassword.toHex() : encryptedPassword);
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
    return logInTime;
}

void m_loggedUser::setLoginTimeStamp(const QDateTime &value)
{
    logInTime = value;
}

bool m_loggedUser::getIsLogged() const
{
    return isLogged;
}

void m_loggedUser::setIsLogged(bool value)
{
    isLogged = value;
}

const QString &m_loggedUser::getEmail() const
{
    return email;
}

void m_loggedUser::setEmail(const QString &newEmail)
{
    email = newEmail;
}

bool m_loggedUser::getVerified() const
{
    return verified;
}

void m_loggedUser::setVerified(bool newVerified)
{
    verified = newVerified;
}

bool m_loggedUser::getBlocked() const
{
    return blocked;
}

void m_loggedUser::setBlocked(bool newBlocked)
{
    blocked = newBlocked;
}

UserRole m_loggedUser::getRole() const
{
    return role;
}

void m_loggedUser::setRole(UserRole newRole)
{
    role = newRole;
}

void m_loggedUser::clearProperties()
{
    setId(-1);
    name.clear();
    password.clear();
    sessionId.clear();
    logInTime = QDateTime();
    setIsLogged(false);
    email.clear();
    setVerified(false);
    setBlocked(false);
    role = NO_ROLE;
}

authenticator m_loggedUser::getAuthorizationData()
{
    authenticator data;
    data.id = getId();
    data.name = getName();
    data.password = getPassword();
    return data;
}
