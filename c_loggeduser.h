#ifndef C_LOGGEDUSER_H
#define C_LOGGEDUSER_H

#include "m_loggeduser.h"
#include "w_logswindow.h"
#include "c_mystructures.h"
#include "c_threadcontroller.h"
#include "mythread.h"
#include "cv_processdata.h"
#include "c_loggeduserthread.h"

#include <QApplication>
#include <QIODevice>
#include <QObject>
#include <QAuthenticator>
#include <QString>
#include <QDataStream>
#include <QByteArray>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>
#include <QMetaMethod>

class c_loggedUser : public m_loggedUser /*, public cv_ProcessData */
{
    Q_OBJECT

public:
    c_loggedUser();
    ~c_loggedUser();

    QAuthenticator getAuthenticator();
    void setUpThread();


    w_logsWindow *getLogs() const;
    void setLogs(w_logsWindow *newLogs);

    c_loggedUserThread*thread() const;
    void setThread(std::unique_ptr<c_loggedUserThread> newThread);

public slots:
    void forceLogOut();
//    void getUserId(QString userName, QString userPassword);
//    void logIn(qint32 id, QString name, QString password);
//    void logOut(qint32 id = -1, QString name = QString(""), QString password = QString(""));
//    void userIdReceivedFromServer(qint32 userID);
//    void logInConfirmationReceivedFromServer(logInConfirmation confirmation);
//    void logOutConfirmationReceivedFromServer(logOutConfirmation confirmation);
//    void processData(threadData data) override;
//    void loggingTimerTimeout();
//    void loggingOutTimerTimeout();

private:
    std::unique_ptr<c_loggedUserThread> mThread;

//    bool loggingState;
//    QTimer * loggingTimer;
//    QTimer * loggingOutTimer;

    w_logsWindow *logs;

private slots:
    void cleanUpThread();

signals:
    void newLog(QString log);
    void threadAssigned(MyThread * thread);
    void logOutUser();
    void aboutToLogOut();
//    void sendToServer(QByteArray packet);
//    void getUserIdFromServer(QByteArray logInPacket);
//    void logInError(QString error);
//    void logInProcessing(QString text);
//    void logInFinished();
//    void userLogged(QString name, UserRole role);
//    void userNotLogged();
//    void logInToServer(qint32 id, QString name, QString password);
};

#endif // C_LOGGEDUSER_H
