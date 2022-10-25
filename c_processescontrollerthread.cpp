#include "c_processescontrollerthread.h"
#include "c_processescontroller.h"

c_processesControllerThread::c_processesControllerThread(qint32 id, QString parentIdent, QString note, QObject *parent)
    : MyThread{id, parentIdent, note, parent}
{
    connect(this, SIGNAL(started()), this, SLOT(threadStarted()));


    localServer = new c_myLocalServer(this);
}

c_processesControllerThread::~c_processesControllerThread()
{
    localServer->deleteLater();
}

void c_processesControllerThread::run()
{
    exec();
}

//QByteArray c_processesControllerThread::hashServerName(QCryptographicHash::Algorithm algorithm, bool toHex)
//{
//    QByteArray encryptedName;

//    QByteArray encryptionSeed;
//    QDataStream encryptionStream(&encryptionSeed, QIODevice::ReadWrite);
//    encryptionStream.setVersion(QDataStream::Qt_6_0);
//    encryptionStream << serverName.toStdString().c_str();

//    QCryptographicHash hasher(algorithm);
//    hasher.addData(encryptionSeed);
//    encryptedName = hasher.result();

//    QString hax = encryptedName.toHex();

//    return (toHex ? encryptedName.toHex() : encryptedName);
//}

void c_processesControllerThread::processData(myStructures::threadData data)
{
    if( this->getId() == data.thread_id && (data.thread_dest == myTypes::CLINIC_LOGGED_USER_CONTROLLER  || data.thread_dest == myTypes::CLINIC_ERROR_CONTROLLER) )
    {
        c_actionExecutive *executive = new c_actionExecutive();
        //executive->moveToThread(mThread.get());
        connect( executive, SIGNAL(newLog(QString)), dynamic_cast<c_processesController *>(myParentConnector)->getLogs(), SLOT(addLog(QString)) );


        executive->processData(data);

        executive->deleteLater();
    }
    else
    {
        //błąd złegodopasownia wątku

        QString errMsg = QString("Thread ERROR. \n Wrong THREAD DESTINATION or THREAD ID");
        emit dynamic_cast<c_processesController *>(myParentConnector)->newLog(errMsg);
    }
}

void c_processesControllerThread::startServer()
{    
    localServer->listen( localServer->hashServerName() );
    if(localServer->isListening())
        emit dynamic_cast<c_processesController *>(myParentConnector)->newLog(QString("c_processesControllerThread::startServer()\nSerwer uruchomiony. %1\n").arg( localServer->serverName() ));
    else
        emit dynamic_cast<c_processesController *>(myParentConnector)->newLog(QString("c_processesControllerThread::startServer()\nSerwer NIE uruchomiony. %1\n").arg( localServer->serverName() ));
}

void c_processesControllerThread::stopServer()
{
    localServer->close();
    emit dynamic_cast<c_processesController *>(myParentConnector)->newLog(QString("c_processesControllerThread::stopServer()  \n"
                                                                                  "Serwer zatrzymany. \n"));
}

c_myLocalServer *c_processesControllerThread::getLocalServer() const
{
    return localServer;
}

void c_processesControllerThread::threadStarted()
{
    startServer();
}

//void c_processesControllerThread::incomingConnection(qintptr socketDescriptor)
//{
//    c_moduleProcessConnection *moduleProcessConnection = new c_moduleProcessConnection(socketDescriptor, this);
//    moduleProcessConnection->setConnectedToProcess(false);

//    connect(moduleProcessConnection, SIGNAL(newLog(QString)), dynamic_cast<c_processesController *>(myParentConnector)->getLogs(), SLOT(addLog(QString)));

//    connect(this, SIGNAL(needConnectionToProcessSettings()), moduleProcessConnection, SLOT(sendConnectionToProcessSettingsRequest()));
//    connect(this, SIGNAL(newModuleConnectedToServer(c_moduleProcessConnection*)), dynamic_cast<c_processesController *>(myParentConnector), SLOT(newModuleProcess(c_moduleProcessConnection*)));
//    connect(moduleProcessConnection, SIGNAL(moduleProcessConnectionFinished(c_moduleProcessConnection*)), dynamic_cast<c_processesController *>(myParentConnector), SLOT(removeModuleProcess(c_moduleProcessConnection*)));


//    QString log = QString("incomingConnection(qintptr socketDescriptor) \n"
//                          "connection->start(); \n");


//    emit newModuleConnectedToServer(moduleProcessConnection);
//    emit dynamic_cast<c_processesController *>(myParentConnector)->newLog(log);

////    c_Parser parser;
////    QPair<QByteArray, QByteArray> pair = parser.prepareRequestConnectionToProcessPacket(getId());

////    myStructures::packet packet;
////    packet.md5_hash = pair.first;
////    packet.packet_to_send = pair.second;
////    packet.wait_for_reply = true;

////    emit moduleProcessConnection->sendDataToModuleProcessSignal(packet);
//}

c_myLocalServer::c_myLocalServer(QObject *parent) : QLocalServer(parent)
{

}

c_myLocalServer::~c_myLocalServer()
{

}

QByteArray c_myLocalServer::hashServerName(QCryptographicHash::Algorithm algorithm, bool toHex)
{
    QByteArray encryptedName;

    QByteArray encryptionSeed;
    QDataStream encryptionStream(&encryptionSeed, QIODevice::ReadWrite);
    encryptionStream.setVersion(QDataStream::Qt_6_0);
    encryptionStream << srvrName.toStdString().c_str();

    QCryptographicHash hasher(algorithm);
    hasher.addData(encryptionSeed);
    encryptedName = hasher.result();

    QString hax = encryptedName.toHex();

    return (toHex ? encryptedName.toHex() : encryptedName);
}

void c_myLocalServer::incomingConnection(quintptr  socketDescriptor)
{
        c_moduleProcessConnection *moduleProcessConnection = new c_moduleProcessConnection(socketDescriptor, this);
        moduleProcessConnection->setConnectedToProcess(false);


        connect(moduleProcessConnection, SIGNAL(newLog(QString)), w_logsWindow::Instance(), SLOT(addLog(QString)));

        connect(this, SIGNAL(needConnectionToProcessSettings()), moduleProcessConnection, SLOT(sendConnectionToProcessSettingsRequest()));


        connect(this, SIGNAL(newModuleConnectedToServer(c_moduleProcessConnection*)), (dynamic_cast<c_processesControllerThread *>( parent() ))->getMyParentConnector(), SLOT(newModuleProcessConnection(c_moduleProcessConnection*)));
        connect(moduleProcessConnection, SIGNAL(moduleProcessConnectionFinished(c_moduleProcessConnection*)),  (dynamic_cast<c_processesControllerThread *>( parent() ))->getMyParentConnector(), SLOT(removeModuleProcessConnection(c_moduleProcessConnection*)));


        QString log = QString("incomingConnection(qintptr socketDescriptor) \n"
                              "connection->start(); \n");


        emit newModuleConnectedToServer(moduleProcessConnection);
        emit moduleProcessConnection->newLog(log);

        c_Parser parser;
        QPair<QByteArray, QByteArray> pair = parser.prepareRequestConnectionToProcessPacket( dynamic_cast<c_processesControllerThread *>(parent())->getId() );

        myStructures::packet packet;
        packet.md5_hash = pair.first;
        packet.packet_to_send = pair.second;
        packet.wait_for_reply = true;

        emit moduleProcessConnection->sendDataToModuleProcessSignal(packet);
}
