#ifndef M_LOGGEDUSER_H
#define M_LOGGEDUSER_H

#include "c_mystructures.h"

#include <QObject>
#include <QPixmap>
#include <QDateTime>
#include <QCryptographicHash>
#include <QIODevice>

class m_loggedUser : public QObject
{
    Q_OBJECT
public:
    explicit m_loggedUser(QObject *parent = nullptr);

    qint32 getId() const;
    void setId(const qint32 &value);

    QString getName() const;
    void setName(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);

    QString getEncryptedPassword(bool toHex = true);

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

    UserRole getRole() const;
    void setRole(UserRole newRole);

    void clearProperties();
    authenticator getAuthorizationData();

signals:

public slots:

private:
    qint32 id;
    QString name;
    QString password;
    QString sessionId;
    QDateTime logInTime;
    bool isLogged;
    QString email;
    bool verified;
    bool blocked;
    UserRole role;
};


#endif // M_LOGGEDUSER_H
