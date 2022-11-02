#ifndef C_SESSIONCONTROLLERTHREAD_H
#define C_SESSIONCONTROLLERTHREAD_H

#include "mythread.h"
#include "c_parser.h"
#include "c_loggeduser.h"


#include <QObject>
#include <QThread>
#include <QTimer>
#include <QEventLoop>

class c_SessionControllerThread : public MyThread
{
    Q_OBJECT
public:
    explicit c_SessionControllerThread(qint32 id = -1, QString parentIdent = QString(), QString note = QString(), QObject *parent = nullptr);
    ~c_SessionControllerThread();

    void run() override;
    void getSessionSettingsFromServer();


    QTimer * getsessionSettingsTimer;


    quint32 getSessionExpireSeconds() const;
    void setSessionExpireSeconds(quint32 newSessionExpireSeconds);

//    const QTime &getCurrentSessionExpireTime() const;
//    void setCurrentSessionExpireTime(const QTime &newCurrentSessionExpireTime);

//    const QTime &getSessionExpireTime() const;
//    void setSessionExpireTime(const QTime &newSessionExpireTime);

    myTypes::ThreadDestination getNameThreadDestination() const;

public slots:
    void processData(myStructures::threadData data, qintptr socketDescriptor) override;
    void sessionRun();
    void unlockSession();
//    void resetIDLEtimer();
    void sendSessionFileToServer(QUuid id_session, QByteArray fileData);
    void sendSessionStateToServer(QUuid id_session, qint32 state);
    void appIDLEdetected();
    void sessionClose();
    void sessionUnlockConfirmationReceived();
    void configureSession(QString username, QString role);

private:
    const myTypes::ThreadDestination nameThreadDestination = myTypes::CLINIC_SESSION_CONTROLLER;
//    QTimer * idleTimer;
//    QTimer * secsTimer;

//    QTime currentSessionExpireTime;
//    QTime sessionExpireTime;
    quint32 sessionExpireSeconds;

    bool waitForSessionSavedBeforeClose;
    bool sessionSavedBeforeClose;

private slots:
//    void secsTimerTimeOut();
    void getsessionSettingsTimerTimeOut();
    void threadStarted() override;
    void sessionSettingsReceivedFromServer(QMap<QString, QVariant> settings);
    void sessionStateUpdateConfirmationReceivedFromServer();
    void sessionFileUpdateConfirmationReceivedFromServer();


signals:
//    void sessionTimeExpireChanged(QTime time);
    void idleDetected();
    void sessionUnlocked();
    void sessionTimeSet(quint32 time);
    void sessionClosedCorrectly();
//    void sendToServer(QByteArray packet);
//    void processingError(QString error);
//    void processingStarted(QString text);
//    void processingFinished();

};

#endif // C_SESSIONCONTROLLERTHREAD_H
