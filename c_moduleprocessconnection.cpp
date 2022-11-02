#include "c_moduleprocessconnection.h"

c_moduleProcessConnection::c_moduleProcessConnection(qintptr ID, QObject *parent)
    : QObject{parent}
{
    this->socketDescriptor = ID;
    connectedToProcess = false;
    this->socket = new QLocalSocket(this);

    if(!this->socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(this->socket->error());

        QString log = QString("c_ClientConnection::run() \n"
                              "error(this->socket->error())");
        emit newLog(log);

        return;
    }

    //from QLocalSocket
    connect(this->socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(this->socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(this->socket, SIGNAL(errorOccurred(QLocalSocket::LocalSocketError)), this, SLOT(errorOccurred(QLocalSocket::LocalSocketError)));
    connect(this->socket, SIGNAL(stateChanged(QLocalSocket::LocalSocketState)), this, SLOT(stateChanged(QLocalSocket::LocalSocketState)));
    //from QIODevice
    connect(this->socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(this->socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    //from c_moduleProcessConnection
//    connect(this, SIGNAL(dataRead(quint64, QByteArray, qintptr)), this, SLOT(parseReceivedPacket(quint64, QByteArray, qintptr)));
    connect(this, SIGNAL(sendDataToModuleProcessSignal(myStructures::packet)), this, SLOT(sendDataToModuleProcess(myStructures::packet)), Qt::DirectConnection );


    socket->open(QIODeviceBase::ReadWrite);
}

qintptr c_moduleProcessConnection::getSocketDescriptor() const
{
    return socketDescriptor;
}

void c_moduleProcessConnection::setSocketDescriptor(qintptr newSocketDescriptor)
{
    socketDescriptor = newSocketDescriptor;
}

bool c_moduleProcessConnection::getConnectedToProcess() const
{
    return connectedToProcess;
}

void c_moduleProcessConnection::setConnectedToProcess(bool newConnectedToProcess)
{
    connectedToProcess = newConnectedToProcess;
}

void c_moduleProcessConnection::sendConnectionToProcessSettingsRequest()
{
    c_Parser parser;
    QPair<QByteArray, QByteArray> pair = parser.prepareRequestConnectionToProcessPacket(myTypes::CLINIC_MODULE_PROCESS_CONTROLLER);

    myStructures::packet packet;
    packet.md5_hash = pair.first;
    packet.packet_to_send = pair.second;
    packet.wait_for_reply = true;

    sendDataToModuleProcess(packet);
}

void c_moduleProcessConnection::connected()
{

}

void c_moduleProcessConnection::disconnected()
{
    emit moduleProcessConnectionFinished(this);
}

void c_moduleProcessConnection::errorOccurred(QLocalSocket::LocalSocketError socketError)
{
    emit newLog( QString("c_clientProcessConnection::errorOccurred(QLocalSocket::LocalSocketError socketError) \n") );
    //LocalSocketError { ConnectionRefusedError, PeerClosedError, ServerNotFoundError, SocketAccessError, SocketResourceError, …, UnknownSocketError }
    switch(socketError) {
    case QLocalSocket::ConnectionRefusedError: {emit newLog( QString("ConnectionRefusedError \n") ); return;}
    case QLocalSocket::PeerClosedError: {emit newLog( QString("PeerClosedError \n") ); return;}
    case QLocalSocket::ServerNotFoundError: {emit newLog( QString("ServerNotFoundError \n") ); return;}
    case QLocalSocket::SocketAccessError: {emit newLog( QString("SocketAccessError \n") ); return;}
    case QLocalSocket::SocketResourceError: {emit newLog( QString("SocketResourceError \n") ); return;}
    case QLocalSocket::SocketTimeoutError: {emit newLog( QString("SocketTimeoutError \n") ); return;}
    case QLocalSocket::DatagramTooLargeError: {emit newLog( QString("DatagramTooLargeError \n") ); return;}
    case QLocalSocket::ConnectionError: {emit newLog( QString("ConnectionError \n") ); return;}
    case QLocalSocket::UnsupportedSocketOperationError: {emit newLog( QString("UnsupportedSocketOperationError \n") ); return;}
    case QLocalSocket::OperationError: {emit newLog( QString("OperationError \n") ); return;}
    case QLocalSocket::UnknownSocketError: {emit newLog( QString("UnknownSocketError \n") ); return;}
    default: {emit newLog( QString("UnknownSocketError \n") ); return;}
    }
}

void c_moduleProcessConnection::stateChanged(QLocalSocket::LocalSocketState socketState)
{
    emit newLog( QString("c_clientProcessConnection::stateChanged(QLocalSocket::LocalSocketState socketState) \n") );
    switch (socketState) {
    case QLocalSocket::UnconnectedState: {emit newLog( QString("UnconnectedState \n") ); return;}
    case QLocalSocket::ConnectingState: {emit newLog( QString("ConnectingState \n") ); return;}
    case QLocalSocket::ConnectedState: {emit newLog( QString("ConnectedState \n") ); return;}
    case QLocalSocket::ClosingState: {emit newLog( QString("ClosingState \n") ); return;}
    default: {emit newLog( QString("No state att. \n") ); return;}
    }
}


void c_moduleProcessConnection::bytesWritten(qint64 bytes)
{

}


void c_moduleProcessConnection::readyRead()
{
    QByteArray myPack;

    while(socket->canReadLine()) {
        QByteArray line = QByteArray( socket->readLine() );
        if( QString::fromUtf8(line) == QString("PACKET_BEGINNING\n") ) {
            myPack.clear();
        } else if( QString::fromUtf8(line) == QString("PACKET_END\n") ) {
            QString log = QString("%1 has been read. \n").arg(myPack.size());
            emit newLog(log);

            emit dataRead(myPack.size(), myPack, this->getSocketDescriptor());
        } else {
            myPack.append(line);
        }
    }
}


void c_moduleProcessConnection::sendDataToModuleProcess(myStructures::packet packet)
{
    socket->write( QString("PACKET_BEGINNING\n").toUtf8() );
    socket->write( packet.packet_to_send );
    socket->write( QString("PACKET_END\n").toUtf8() );

    QString log = QString("%1 has been written. \n").arg(packet.packet_to_send.size());
}
