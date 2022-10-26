#ifndef C_PROCESSESCONTROLLERTHREAD_H
#define C_PROCESSESCONTROLLERTHREAD_H

#include "mythread.h"
#include "c_actionexecutive.h"
#include "c_moduleprocess.h"
#include "c_moduleprocessconnection.h"
#include "w_logswindow.h"


#include <QObject>
#include <QTime>
#include <QTimer>
#include <QThread>
#include <QMetaType>
#include <QLocalServer>
#include <QLocalSocket>
#include <QCryptographicHash>

class c_myLocalServer : public QLocalServer
{
    Q_OBJECT
public:
    explicit c_myLocalServer(QObject * parent = nullptr);
    ~c_myLocalServer();
    QByteArray hashServerName(QCryptographicHash::Algorithm algorithm = QCryptographicHash::Md5, bool toHex = true);

private:
    const QString srvrName = "ClinicClientLocalServer";

protected:
    void incomingConnection(quintptr  socketDescriptor);

signals:
    void newModuleConnectedToServer(c_moduleProcessConnection *processConnection);
    void needConnectionToProcessSettings();

};

class c_processesControllerThread : public MyThread
{
    Q_OBJECT
public:
    explicit c_processesControllerThread(qint32 id = -1, QString parentIdent = QString(), QString note = QString(), QObject *parent = nullptr);
    ~c_processesControllerThread();

    void run() override;


    c_myLocalServer *getLocalServer() const;
//    QByteArray hashServerName(QCryptographicHash::Algorithm algorithm = QCryptographicHash::Md5, bool toHex = true);

public slots:
    void processData(myStructures::threadData data, qintptr socketDescriptor) override;
    void parseReceivedPacket(quint64 size, QByteArray data, qintptr socketDescriptor);

    void startServer();
    void stopServer();

private:
    c_myLocalServer *localServer;
//    const QString serverName = "ClinicClientLocalServer";



private slots:
    void threadStarted() override;

signals:
    void passDataToClinicClient(quint64 size, QByteArray data, qintptr socketDescriptor);
//    void newModuleConnectedToServer(c_moduleProcessConnection *processConnection);
//    void moduleProcessConnectionFinished(c_moduleProcessConnection *processConnection);
//    void needConnectionToProcessSettings();

protected:
    void incomingConnection(qintptr socketDescriptor);

};

#endif // C_PROCESSESCONTROLLERTHREAD_H
