#include "c_connectiontoservercontroller.h"


c_connectionToServerController::~c_connectionToServerController()
{
    tryReconnect = false;

    reconnectTimer->deleteLater();
    sendAgainPackets->deleteLater();

    if(packetsToSend.empty()) {

        socket->disconnectFromHost();
        socket->deleteLater();
    }

}

void c_connectionToServerController::start()
{
    reconnectTimer->start(100);
}

void c_connectionToServerController::processData(myStructures::threadData data)
{

}


void c_connectionToServerController::setConnection(QMap<QString, QVariant> settings)
{
    QString serverAddress = settings["address"].toString();
    int serverPort = settings["port"].toInt();

    setHost(serverAddress);
    setPort(quint16(serverPort));

    QString log = QString("Ustawiono: host= %1 , port= %2. \n").arg(serverAddress).arg(serverPort);
    emit newLog(log);
}

void c_connectionToServerController::runSocket()
{

    QString log = QString("Running socket. Server: %1:%2. \n "
                          "socket->connectToHost( %1, %2 ) \n").arg(getHost()).arg(getPort());
    emit newLog(log);

    socket->connectToHost( getHost(), getPort() );
    socket->open(QIODevice::ReadWrite);
}

c_connectionToServerController::c_connectionToServerController(QObject *parent) : QObject(parent)
{
    tryReconnect = true;

    socket = new QTcpSocket();

    connect(this, SIGNAL(packetInBuffer()), this, SLOT(sendPackets()));

    reconnectTimer = new QTimer(this);
    connect( reconnectTimer, SIGNAL(timeout()), this, SLOT(reconnectTimerTimeOut()) );

    sendAgainPackets = new QTimer(this);
    connect( sendAgainPackets, SIGNAL(timeout()), this, SLOT(sendAgainPacketsTimerTimeOut()) );

    connect(socket, SIGNAL( connected() ), this, SLOT( socketConnected() ) );
    connect(socket, SIGNAL( disconnected() ), this, SLOT( socketDisconnected() ) );
    connect(socket, SIGNAL( errorOccurred(QAbstractSocket::SocketError) ), this, SLOT( socketError(QAbstractSocket::SocketError) ) );
    connect(socket, SIGNAL( hostFound() ), this, SLOT( socketFoundHost() ) );
    connect(socket, SIGNAL( stateChanged(QAbstractSocket::SocketState) ), this, SLOT( socketStateChanged(QAbstractSocket::SocketState) ) );
    connect(socket, SIGNAL( readyRead() ), this, SLOT( socketReadyRead() ), Qt::DirectConnection );
    connect(socket, SIGNAL( bytesWritten(qint64) ), this, SLOT( socketBytesWritten(qint64) ) );

    QString log = QString("Socket connections configured. \n");
    emit newLog(log);
}

c_LogsController *c_connectionToServerController::getLogsController() const
{
    return logsController;
}

void c_connectionToServerController::setLogsController(c_LogsController *newLogsController)
{
    logsController = newLogsController;
}

w_logsWindow *c_connectionToServerController::getLogs() const
{
    return logs;
}

void c_connectionToServerController::setLogs(w_logsWindow *newLogs)
{
    logs = newLogs;
}

myTypes::ThreadDestination c_connectionToServerController::getNameThreadDestination() const
{
    return nameThreadDestination;
}

QTcpSocket *c_connectionToServerController::getSocket() const
{
    return socket;
}

void c_connectionToServerController::setSocket(QTcpSocket *value)
{
    socket = value;
}

void c_connectionToServerController::socketConnected()
{
    QString log = QString("Socket connected: %1:%2. \n").arg(getHost()).arg(getPort());
    emit newLog(log);
}

void c_connectionToServerController::socketDisconnected()
{    
    sendAgainPackets->stop();
    QString log = QString("Socket disconnected by the server: %1:%2. \n").arg(getHost()).arg(getPort());
    emit newLog(log);
    emit socketDisconnectedLogOutUser();
    if(tryReconnect)
        start();
}

void c_connectionToServerController::socketError(QAbstractSocket::SocketError socketError)
{
    QString log = QString("Socket error: %1. \n").arg(socketError);
    emit newLog(log);
}

void c_connectionToServerController::socketStateChanged(QAbstractSocket::SocketState socketState)
{
    QString log = QString("Socket changed state: %1. \n").arg(socketState);
    emit newLog(log);
    emit connectionToServerStateChanged(socketState);
}

void c_connectionToServerController::socketFoundHost()
{
    QString log = QString("Socket found host \n");
    emit newLog(log);
}



void c_connectionToServerController::socketReadyRead()
{
//    QDataStream rs(socket);
//    rs.setVersion(QDataStream::Qt_6_0);


//    QByteArray data;
//    quint64 data_size;


//    rs.startTransaction();


//    rs >> data_size >> data; // try to read packet atomically

//    if (!rs.commitTransaction())
//        return;     // wait for more data

//    c_LogsController::Instance()->saveLogToFile(QString("c_connectionToServerController::socketReadyRead()"), QString("%1").arg(getSocket()->socketDescriptor()), data);
//    QString log = QString("%1 has been read. \n").arg(data_size);
//    emit newLog(log);

//    emit dataReceived(data_size, data);

    /*----------------READ 2 ------------------------------------*/

    QByteArray myPack;

    while(socket->canReadLine()) {
        QByteArray line = QByteArray( socket->readLine() );
        if( QString::fromUtf8(line) == QString("PACKET_BEGINNING\n") ) {
            myPack.clear();
        } else if( QString::fromUtf8(line) == QString("PACKET_END\n") ) {
            c_LogsController::Instance()->saveLogToFile(QString("c_connectionToServerController::socketReadyRead()"), QString("%1").arg(getSocket()->socketDescriptor()), myPack);
            QString log = QString("%1 has been read. \n").arg(myPack.size());
            emit newLog(log);

            emit dataReceived(myPack.size(), myPack);
        } else {
            myPack.append(line);
        }
    }

    /*----------------READ 2 ------------------------------------*/

}

void c_connectionToServerController::socketBytesWritten(qint64 bytes)
{
    QString log = QString("%1 has been written. \n").arg(bytes);
    emit newLog(log);
}


void c_connectionToServerController::reconnectTimerTimeOut()
{
    if(socket->state() != QAbstractSocket::ConnectedState) {
        runSocket();
        reconnectTimer->start(5000);
    } else {
        reconnectTimer->stop();
    }
}

void c_connectionToServerController::sendAgainPacketsTimerTimeOut()
{    
    sendAgainPackets->stop();
    while(!waitingForReceiveConfirmation.isEmpty()) {
        packetsToSend.append(waitingForReceiveConfirmation.takeFirst());
    }

    while(!waitingForReplyPackets.isEmpty()) {
        packetsToSend.append(waitingForReplyPackets.takeFirst());
    }

    if (!packetsToSend.isEmpty()) {
        //sendAgainPackets->start(10000);
        emit packetInBuffer();
    }
}

quint16 c_connectionToServerController::getPort() const
{
    return port;
}

void c_connectionToServerController::setPort(const quint16 &value)
{
    port = value;
}

void c_connectionToServerController::closeConnection()
{
    QString log = QString("CloseConnection \n socket->close() \n");
    emit newLog(log);

    socket->close();
}


void c_connectionToServerController::sendData(myStructures::packet packet)
{
//    if(getSocket()->state() == QAbstractSocket::ConnectedState && getSocket()->isWritable() ) {
//        QDataStream socketStream(socket);
//        socketStream.setVersion(QDataStream::Qt_6_0);

//        socketStream << static_cast<quint64>(packet.packet_to_send.size()) << packet.packet_to_send;
//        waitingForReceiveConfirmation.append(packet);

//        c_LogsController::Instance()->saveLogToFile(QString("c_connectionToServerController::sendData"), QString("%1").arg(getSocket()->socketDescriptor()), packet.packet_to_send);
//    } else {
//    }

////    waitingForReceiveConfirmation.append(packet);
///
///
    /*----------------------SEND 2--------------------------*/
    if(getSocket()->state() == QAbstractSocket::ConnectedState && getSocket()->isWritable() ) {
        socket->write( QString("PACKET_BEGINNING\n").toUtf8() );
        socket->write( packet.packet_to_send );
        socket->write( QString("PACKET_END\n").toUtf8() );
        waitingForReceiveConfirmation.append(packet);

        c_LogsController::Instance()->saveLogToFile(QString("c_connectionToServerController::sendData"), QString("%1").arg(getSocket()->socketDescriptor()), packet.packet_to_send);
        QString log = QString("%1 has been written. \n").arg(packet.packet_to_send.size());
     } else {
     }
    /*----------------------SEND 2--------------------------*/
}

void c_connectionToServerController::passDataToBuffer(myStructures::packet packet)
{
    if( !isWaitingForReceiveConfirmation(packet) && !isWaitingForReply(packet) ) {
        //waitingForReceiveConfirmation.append(packet);
        packetsToSend.append(packet);
    }

    if (!packetsToSend.isEmpty()) {
        emit packetInBuffer();
    }
}

void c_connectionToServerController::sendPackets()
{
    sendAgainPackets->stop();
    while(!packetsToSend.empty()) {
        sendData( packetsToSend.takeFirst() );
        QString log = QString("Wysyłam pakiet. Pozostało %1 \n").arg(packetsToSend.size());
        emit newLog(log);
    }

//    if(!waitingForReceiveConfirmation.isEmpty() || !waitingForReplyPackets.isEmpty())
//        sendAgainPackets->start(10000);
}

void c_connectionToServerController::receiveConfirmationReceived(myStructures::threadData data)
{
    QString log = QString("Potwierdzenie otrzymania pakietu przez serwer. %1\n").arg(data.ref_md5);
    emit newLog(log);

    c_Parser parser;
    QByteArray md5Confirmation;
    parser.parseForMd5(&data, &md5Confirmation);

    for(int i = 0; i < waitingForReceiveConfirmation.length(); i++) {
        if( waitingForReceiveConfirmation[i].md5_hash == md5Confirmation ) {
            if( waitingForReceiveConfirmation[i].wait_for_reply ) {
                waitingForReplyPackets.append( waitingForReceiveConfirmation.takeAt(i) );
                QString log = QString("Usunięto z listy: Oczekuje na potweirdzenie otrzymania przez serwer. %1\n").arg(data.ref_md5);
                emit newLog(log);
                log = QString("Dodano do listy: Oczekuje na odpowiedz z serwera. %1\n").arg(data.ref_md5);
                emit newLog(log);
            } else {
                waitingForReceiveConfirmation.removeAt(i);
                QString log = QString("Usunięto z listy: Oczekuje na potweirdzenie otrzymania przez serwer. %1\n").arg(data.ref_md5);
                emit newLog(log);
            }
            break;
        }
    }
}

void c_connectionToServerController::replyReceivedRemoveFromList(QByteArray ref_md5)
{
    for(int i = 0; i < waitingForReplyPackets.length(); i++) {
        if( waitingForReplyPackets[i].md5_hash == ref_md5 ) {
            waitingForReplyPackets.removeAt(i);
            QString log = QString("Usunięto z listy: Oczekuje na odpowiedz z serwera. %1\n").arg(ref_md5);
            emit newLog(log);

            break;
        }
    }
}

bool c_connectionToServerController::isWaitingForReply(myStructures::packet packet)
{
    bool answer = false;

    for(int i = 0; i < waitingForReplyPackets.length(); i++) {
        if( packet.md5_hash == waitingForReplyPackets[i].md5_hash ) {
            answer = true;
            break;
        }
    }

    return answer;
}

bool c_connectionToServerController::isWaitingForReceiveConfirmation(myStructures::packet packet)
{
    bool answer = false;

    for(int i = 0; i < waitingForReceiveConfirmation.length(); i++) {
        if( packet.md5_hash == waitingForReceiveConfirmation[i].md5_hash ) {
            answer = true;
            break;
        }
    }

    return answer;
}

QString c_connectionToServerController::getHost() const
{
    return host;
}

void c_connectionToServerController::setHost(const QString &value)
{
    host = value;
}
