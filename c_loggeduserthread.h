#ifndef C_LOGGEDUSERTHREAD_H
#define C_LOGGEDUSERTHREAD_H

#include "mythread.h"
#include "c_parser.h"
#include "m_loggeduser.h"
#include "m_employee.h"

#include <QObject>
#include <QTime>
#include <QTimer>
#include <QThread>
#include <QMetaType>

class c_loggedUserThread : public MyThread
{
    Q_OBJECT
public:
    explicit c_loggedUserThread(qint32 id = -1, QString parentIdent = QString(), QString note = QString(), QObject *parent = nullptr);

    void run() override;

    bool getLoggingState() const;
    void setLoggingState(bool newLoggingState);

    bool getLoggingOutState() const;
    void setLoggingOutState(bool newLoggingOutState);

    myTypes::ThreadDestination getNameThreadDestination() const;

public slots:
    void processData(myStructures::threadData data, qintptr socketDescriptor) override;
    void sessionLocked();
    void logOut(qint32 id = -1, QString name = QString(""), QString password = QString(""));
    void getUserId(QString userName, QString userPassword);
    void unlockOnIdle(QString userName, QString userPassword);
    void getProperties(QMap<QString, QVariant> *userProperties, QMap<QString, QVariant> *employeeProperties, QList<myStructures::myLog> * Logs);
    void getUserPropertiesFromServer(QString name = QString(""), QString password = QString(""));
    void getEmployeePropertiesFromServer(QString name = QString(""), QString password = QString(""));
    void getLogsFromServer(qint32 id, QString name = QString(""), QString password = QString(""));

private:
    const myTypes::ThreadDestination nameThreadDestination = myTypes::CLINIC_LOGGED_USER_CONTROLLER;
    bool loggingState;
    bool loggingOutState;
    QTimer * loggingTimer;
    QTimer * loggingOutTimer;

private slots:
    void logIn(qint32 id, QString name, QString password);
    void logOutOffLine();
    void userIdReceivedFromServer(qint32 userID);
    void logInConfirmationReceivedFromServer(myStructures::logInConfirmation confirmation);
    void logOutConfirmationReceivedFromServer(myStructures::logOutConfirmation confirmation);
    void userPropertiesReceivedFromServer(QMap<QString, QVariant> properties);
    void employeePropertiesReceivedFromServer(QMap<QString, QVariant> properties);
    void userLogsReceivedFromServer(QList<QMap<QString, QVariant>> logs);
    void userEmployeeLogsReceivedFromServer(QList<QMap<QString, QVariant>> logs);
    void emlpoyeeLogsReceivedFromServer(QList<QMap<QString, QVariant>> logs);
    void unlockConfirmationReceived(bool canUnlock);

    void loggingTimerTimeout();
    void loggingOutTimerTimeout();

    void threadStarted() override;

signals:
    void logInError(QString error);
    void logInProcessing(QString text);
    void logInFinished();

    void userLogged(QString name, QString role);
    void userNotLogged();
    void logInToServer(qint32 id, QString name, QString password);
    void getUserIdFromServer(QByteArray logInPacket);
    void unlockSession();
    void userAuthorizationDataNeeded();
    void userAuthorizationDataNeededToUnlock(QString username);
    void aboutToLogOut();

    void propertiesSaved();
    void logsDbSaved();

};

#endif // C_LOGGEDUSERTHREAD_H
