#ifndef M_LOGGEDUSER_H
#define M_LOGGEDUSER_H

#include <QObject>
#include <QPixmap>
#include <QDateEdit>

class m_loggedUser : public QObject
{
    Q_OBJECT
public:
    explicit m_loggedUser(QObject *parent = nullptr);

    quint8 getId() const;
    void setId(const quint8 &value);

    QString getName() const;
    void setName(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);

    QString getSessionId() const;
    void setSessionId(const QString &value);

    QDateTime getLoginTimeStamp() const;
    void setLoginTimeStamp(const QDateTime &value);

    bool getIsLogged() const;
    void setIsLogged(bool value);

signals:

public slots:


private:
    quint8 id;
    QString name;
    QString password;
    QString sessionId;
    QDateTime loginTimeStamp;
    bool isLogged;
};

#endif // M_LOGGEDUSER_H
