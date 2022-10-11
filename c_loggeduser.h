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
#include <QMetaEnum>

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
    QMap<QString, QVariant> getUserProperties();
    QMap<QString, QVariant> getEmployeeProperties();
    QList<myStructures::myLog> getAuthLogs();
    QList<myStructures::myLog> getClinicLogs();


private:
    std::unique_ptr<c_loggedUserThread> mThread;

    w_logsWindow *logs;

    QList<myStructures::myLog> authLogs;
    QList<myStructures::myLog> ClinicLogs;

private slots:
    void cleanUpThread();
    void logsReceivedFromServer(QList<myStructures::myLog> authLogs, QList<myStructures::myLog> clinicLogs);

signals:
    void newLog(QString log);
    void threadAssigned(MyThread * thread);
    void logOutUser();
    void aboutToLogOut();
    void getLogsSignal(qint32 id, QString name, QString password);

};

#endif // C_LOGGEDUSER_H
