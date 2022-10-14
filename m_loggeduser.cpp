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
    encryptionStream << getPassword();

    QCryptographicHash hasher(QCryptographicHash::Sha256);
    hasher.addData(encryptionSeed);
    encryptedPassword = hasher.result();

    return (toHex ? encryptedPassword.toHex() : encryptedPassword);
}

QString m_loggedUser::getEncryptedPassword(bool hashName, bool toHex)
{
    QByteArray encryptedPassword;

    QByteArray encryptionSeed;
    QDataStream encryptionStream(&encryptionSeed, QIODevice::ReadWrite);
    encryptionStream.setVersion(QDataStream::Qt_6_0);
    encryptionStream << getPassword();
    if(hashName)
        encryptionStream << getName();

    QCryptographicHash hasher(QCryptographicHash::Sha256);
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

m_loggedUser::UserRole m_loggedUser::getRole() const
{
    return role;
}

QString m_loggedUser::getRoleString() const
{
    QMetaEnum metaEnum = QMetaEnum::fromType<m_loggedUser::UserRole>();
    return metaEnum.valueToKey( this->getRole() );
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

myStructures::authenticator m_loggedUser::getAuthorizationData()
{
    myStructures::authenticator data;
    data.id = getId();
    data.name = getName();
    data.password = getPassword();
    return data;
}

c_employee *m_loggedUser::getEmployee() const
{
    return employee;
}

void m_loggedUser::setEmployee(c_employee *newEmployee)
{
    employee = newEmployee;
}

const QDateTime &m_loggedUser::getCreate_date() const
{
    return create_date;
}

void m_loggedUser::setCreate_date(const QDateTime &newCreate_date)
{
    create_date = newCreate_date;
}

const QDateTime &m_loggedUser::getVerify_date() const
{
    return verify_date;
}

void m_loggedUser::setVerify_date(const QDateTime &newVerify_date)
{
    verify_date = newVerify_date;
}

const QDateTime &m_loggedUser::getBlock_date() const
{
    return block_date;
}

void m_loggedUser::setBlock_date(const QDateTime &newBlock_date)
{
    block_date = newBlock_date;
}

const QByteArray &m_loggedUser::getPhoto() const
{
    return photo;
}

void m_loggedUser::setPhoto(const QByteArray &newPhoto)
{
    photo = newPhoto;
}
