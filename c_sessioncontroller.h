#ifndef C_SESSIONCONTROLLER_H
#define C_SESSIONCONTROLLER_H

#include "c_mystructures.h"
#include "c_loggeduser.h"
#include "c_connectiontoservercontroller.h"
#include "mythread.h"
#include "c_sessioncontrollerthread.h"
#include "c_threadcontroller.h"
#include "cv_processdata.h"
#include "c_parser.h"
#include "c_sendingpacketscontroller.h"

#include <QObject>
#include <QWidget>
#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <QUuid>
#include <QDateTime>
#include <QFile>
#include <QList>
#include <QThread>
#include <QEvent>
#include <QTimer>
#include <QMessageBox>
#include <QTime>

class c_SessionController : public QObject/*, public cv_ProcessData */
{
    Q_OBJECT
public:
    explicit c_SessionController(QObject *parent = nullptr);
    ~ c_SessionController();


    void setUpSession(QMap<QString, QVariant> settings);

    void setIdentifier(QUuid uuid = QUuid());
    QUuid getIdentifier() const;

    void setUpThread();
    void closeSession();
    int createSessionStateFile(quint32 fileCN = 0, double fileVN = 0, int dsVersion = QDataStream::Qt_6_0, QString filePathName = QString(""));
    QByteArray readEntireSessionFile(QString filePathName = QString(""));
    bool isOpened();
    bool isValid();
    int addNotSendedNoteToFile(QString filePathName = QString(""));




    c_loggedUser *getLoggedUser() const;
    void setLoggedUser(c_loggedUser *newLoggedUser);

    c_SessionControllerThread*thread() const;
    void setThread(std::unique_ptr<c_SessionControllerThread> newThread);

    w_logsWindow *getLogs() const;
    void setLogs(w_logsWindow *newLogs);

    SessionState getState() const;
    void setState(SessionState newState);    

    void setOpened(bool newOpened);

public slots:
    //void sessionRun();
    //void resetIDLEtimer();

private:
    bool opened;
    QUuid identifier;
    quint32 fileControlNumber;
    double fileVersionNumber;
    QString fileName;
    QString filePath;
    QHostAddress hostAddress;

    QDateTime startTime;
    QDateTime endTime;
    QDateTime currentOpenTime;

    quint32 sessionExpireTime;

    SessionState state;

    QFile sessionFile;
    QByteArray fileData;

    std::unique_ptr<c_SessionControllerThread> mThread;    

//    QTimer * idleTimer;
//    QTimer * secsTimer;

//    QTime currentSessionExpireTime;
//    QTime sessionExpireTime;
//    quint32 sessionExpireSeconds;

    c_loggedUser * loggedUser;


    w_logsWindow *logs;

private slots:
    void cleanUpThread();
    void resetSession();
//    void appIDLEdetected();
//    void timeOut();
    //void replyTimerTimeOut();

signals:
    void threadAssigned(MyThread * thread);
    void newLog(QString text);

    void sessionFileCreated(QUuid id_session, QByteArray file);
    void sessionReady();
    void sessionStateChanged(QUuid id_session, qint32 state);
    void sessionClosed();
};

#endif // C_SESSIONCONTROLLER_H
