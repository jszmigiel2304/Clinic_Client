#ifndef M_LOGGEDUSER_H
#define M_LOGGEDUSER_H

#include "c_mystructures.h"
#include "c_employee.h"

#include <QObject>
#include <QPixmap>
#include <QDateTime>
#include <QCryptographicHash>
#include <QIODevice>
#include <QMetaType>
#include <QMetaEnum>

class m_loggedUser : public QObject
{
    Q_OBJECT

public:
    enum UserRole { NO_ROLE = 0x00, GUEST = 0x01, USER = 0x02, MODERATOR = 0x03, ADMIN = 0x04, SUPER = 0x05 };
    Q_ENUM(UserRole)

    explicit m_loggedUser(QObject *parent = nullptr);

    qint32 getId() const;
    void setId(const qint32 &value);

    QString getName() const;
    void setName(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);

    QString getEncryptedPassword(bool toHex = true);
    QString getEncryptedPassword(bool hashName = false, bool toHex = true);

    QString getSessionId() const;
    void setSessionId(const QString &value);

    QDateTime getLoginTimeStamp() const;
    void setLoginTimeStamp(const QDateTime &value);

    bool getIsLogged() const;
    void setIsLogged(bool value);

    const QString &getEmail() const;
    void setEmail(const QString &newEmail);

    bool getVerified() const;
    void setVerified(bool newVerified);

    bool getBlocked() const;
    void setBlocked(bool newBlocked);

    m_loggedUser::UserRole getRole() const;
    QString getRoleString() const;
    void setRole(m_loggedUser::UserRole newRole);


    void clearProperties();
    myStructures::authenticator getAuthorizationData();

    c_employee *getEmployee() const;
    void setEmployee(c_employee *newEmployee);

    const QDateTime &getCreate_date() const;
    void setCreate_date(const QDateTime &newCreate_date);

    const QDateTime &getVerify_date() const;
    void setVerify_date(const QDateTime &newVerify_date);

    const QDateTime &getBlock_date() const;
    void setBlock_date(const QDateTime &newBlock_date);

    const QByteArray &getPhoto() const;
    void setPhoto(const QByteArray &newPhoto);

signals:

public slots:

private:
    qint32 id;
    QString name;
    QString password;
    QString email;
    bool verified;
    bool blocked;
    QDateTime create_date;
    QDateTime verify_date;
    QDateTime block_date;
    QByteArray photo;
    m_loggedUser::UserRole role;
    bool isLogged;

    c_employee * employee;
    QString sessionId;
    QDateTime logInTime;
};


#endif // M_LOGGEDUSER_H
