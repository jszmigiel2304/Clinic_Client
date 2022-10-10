#ifndef C_LOGGEDUSERTHREAD_H
#define C_LOGGEDUSERTHREAD_H

#include "mythread.h"
#include "c_parser.h"

#include <QObject>
#include <QTime>
#include <QTimer>
#include <QThread>

class c_loggedUserThread : public MyThread
{
    Q_OBJECT
public:
    explicit c_loggedUserThread(qint32 id = -1, QString parentIdent = QString(), QString note = QString(), QObject *parent = nullptr);

    void run() override;

    bool getLoggingState() const;
    void setLoggingState(bool newLoggingState);

public slots:
    void processData(threadData data) override;
    void sessionLocked();
    void logOut(qint32 id = -1, QString name = QString(""), QString password = QString(""));
    void getUserId(QString userName, QString userPassword);
    void unlockOnIdle(QString userName, QString userPassword);

private:
    bool loggingState;
    QTimer * loggingTimer;
    QTimer * loggingOutTimer;

private slots:
    void logIn(qint32 id, QString name, QString password);
    void logOutOffLine();
    void userIdReceivedFromServer(qint32 userID);
    void logInConfirmationReceivedFromServer(logInConfirmation confirmation);
    void logOutConfirmationReceivedFromServer(logOutConfirmation confirmation);
    void unlockConfirmationReceived(bool canUnlock);

    void loggingTimerTimeout();
    void loggingOutTimerTimeout();

    void threadStarted() override;

signals:
//    void sendToServer(QByteArray packet);

//    void processingError(QString error);
//    void processingStarted(QString text);
//    void processingFinished();

    void logInError(QString error);
    void logInProcessing(QString text);
    void logInFinished();

    void userLogged(QString name, UserRole role);
    void userNotLogged();
    void logInToServer(qint32 id, QString name, QString password);
    void getUserIdFromServer(QByteArray logInPacket);
    void unlockSession();
    void userAuthorizationDataNeeded();
    void userAuthorizationDataNeededToUnlock(QString username);
    void aboutToLogOut();

};

#endif // C_LOGGEDUSERTHREAD_H
