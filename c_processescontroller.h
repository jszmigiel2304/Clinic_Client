#ifndef C_PROCESSESCONTROLLER_H
#define C_PROCESSESCONTROLLER_H

#include "c_mystructures.h"
#include "c_moduleprocess.h"
#include "mythread.h"
#include "w_logswindow.h"
#include "c_processescontrollerthread.h"
#include "c_moduleprocessconnection.h"

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
#include <QLocalServer>
#include <QLocalSocket>

class c_processesController : public QObject
{
    Q_OBJECT
public:
    explicit c_processesController(QObject *parent = nullptr);
    ~c_processesController();

    void setUpThread();

    void removeOpenedModuleProcessConnections();
    void removeOpenedModulesProcesses();

    w_logsWindow *getLogs() const;
    void setLogs(w_logsWindow *newLogs);

    c_processesControllerThread*thread() const;
    void setThread(std::unique_ptr<c_processesControllerThread> newThread);

    QString getHashServerName();

    c_moduleProcess* getProcess(qint32 threadID);

public slots:
    void newModuleProcessConnection(c_moduleProcessConnection * moduleConnection);
    void removeModuleProcessConnection(c_moduleProcessConnection * moduleConnection);
    void removeModuleProcessConnection(qintptr id);
    void removeModuleProcessConnection(quint32 index);

    void newModuleProcess(c_moduleProcess * moduleProcess);
    void removeModuleProcess(c_moduleProcess * moduleProcess);
    void removeModuleProcess(QByteArray hashedName);
    void removeModuleProcess(quint32 index);    
    void removeAllModuleProcesses();

    void connectProcessWithConnection(QMap<QString, QVariant> connectionSettings, qintptr socketDescriptor);

    void moduleClosed(c_moduleProcess * proces, int exitCode, QProcess::ExitStatus exitStatus = QProcess::NormalExit);



private:
    QList<c_moduleProcess *> openedModulesProcesses;
    QList<c_moduleProcessConnection *> openedModuleProcessConnections;

    std::unique_ptr<c_processesControllerThread> mThread;

    w_logsWindow *logs;

private slots:
    void cleanUpThread();

signals:
    void threadAssigned(MyThread * thread);
    void newLog(QString log); // log on window

};

#endif // C_PROCESSESCONTROLLER_H
