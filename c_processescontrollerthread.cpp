#include "c_processescontrollerthread.h"
#include "c_processescontroller.h"

#define _SERVER_PARENT_PROC_CTRLR_T_ dynamic_cast<c_processesController *>(dynamic_cast<c_processesControllerThread *>( parent() )->getMyParentConnector())
#define _PARENT_PROC_CTRLR_T dynamic_cast<c_processesControllerThread *>(parent())
#define _PARENT_PROC_CTRLR_ dynamic_cast<c_processesController *>(myParentConnector)

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

void c_processesControllerThread::processData(myStructures::threadData data, qintptr socketDescriptor)
{
    if(data.thread_dest == myTypes::CLINIC_MODULE) {
        c_moduleProcess * process = _PARENT_PROC_CTRLR_->getProcess( data.thread_id );
        if(process) {
            c_Parser parser;
            QPair<QByteArray, QByteArray> pair = parser.preparePacket(data);

            myStructures::packet packet;
            packet.md5_hash = pair.first;
            packet.packet_to_send = pair.second;
            packet.wait_for_reply = false;

            emit process->getConnection()->sendDataToModuleProcessSignal( packet );
        }

        return;
    }

    if( this->getId() == data.thread_id && (data.thread_dest == myTypes::CLINIC_MODULE_PROCESS_CONTROLLER  || data.thread_dest == myTypes::CLINIC_ERROR_CONTROLLER) )
    {
        c_actionExecutive *executive = new c_actionExecutive();
        connect( executive, SIGNAL(connectionSettingsReceived(QMap<QString, QVariant>, qintptr)), _PARENT_PROC_CTRLR_, SLOT(connectProcessWithConnection(QMap<QString, QVariant>, qintptr)));
        executive->processData(data, socketDescriptor);

        executive->deleteLater();
    }
    else
    {
        //błąd złegodopasownia wątku
    }
}

void c_processesControllerThread::parseReceivedPacket(quint64 size, QByteArray data, qintptr socketDescriptor)
{
    emit passDataToClinicClient(size, data, socketDescriptor);
}

void c_processesControllerThread::startServer()
{    
    localServer->listen( localServer->hashServerName() );

//    if(localServer->isListening())
//        emit _PARENT_PROC_CTRLR_->newLog(QString("c_processesControllerThread::startServer()\nSerwer uruchomiony. %1\n").arg( localServer->serverName() ));
//    else
//        emit _PARENT_PROC_CTRLR_->newLog(QString("c_processesControllerThread::startServer()\nSerwer NIE uruchomiony. %1\n").arg( localServer->serverName() ));
}

void c_processesControllerThread::stopServer()
{
    localServer->close();
}

myTypes::ThreadDestination c_processesControllerThread::getNameThreadDestination() const
{
    return nameThreadDestination;
}

c_myLocalServer *c_processesControllerThread::getLocalServer() const
{
    return localServer;
}

void c_processesControllerThread::threadStarted()
{
    startServer();
}

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
        moduleProcessConnection->setSocketDescriptor(socketDescriptor);

        connect(this, SIGNAL(needConnectionToProcessSettings()), moduleProcessConnection, SLOT(sendConnectionToProcessSettingsRequest()));


        connect(this, SIGNAL(newModuleConnectedToServer(c_moduleProcessConnection*)), _SERVER_PARENT_PROC_CTRLR_T_, SLOT(newModuleProcessConnection(c_moduleProcessConnection*)), Qt::UniqueConnection);
        connect(moduleProcessConnection, SIGNAL(moduleProcessConnectionFinished(c_moduleProcessConnection*)),  _SERVER_PARENT_PROC_CTRLR_T_, SLOT(removeModuleProcessConnection(c_moduleProcessConnection*)));
        connect(moduleProcessConnection, SIGNAL(dataRead(quint64, QByteArray, qintptr)), _PARENT_PROC_CTRLR_T, SLOT(parseReceivedPacket(quint64, QByteArray, qintptr)));


        emit newModuleConnectedToServer(moduleProcessConnection);

        c_Parser parser;
        QPair<QByteArray, QByteArray> pair = parser.prepareRequestConnectionToProcessPacket(_PARENT_PROC_CTRLR_T->getNameThreadDestination(),
                                                                                            _PARENT_PROC_CTRLR_T->getId() );

        myStructures::packet packet;
        packet.md5_hash = pair.first;
        packet.packet_to_send = pair.second;
        packet.wait_for_reply = true;

        emit moduleProcessConnection->sendDataToModuleProcessSignal(packet);
}
