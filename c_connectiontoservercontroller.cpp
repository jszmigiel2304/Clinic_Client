#include "c_connectiontoservercontroller.h"

c_connectionToServerController *c_connectionToServerController::Instance()
{
    static c_connectionToServerController * instance = nullptr;
    if ( instance == nullptr ) {
        instance = new c_connectionToServerController();
    }
    return instance;
}

c_connectionToServerController::~c_connectionToServerController()
{

}

void c_connectionToServerController::setConnection(QMap<QString, QVariant> settings)
{
    QString serverAddress = settings["address"].toString();
    int serverPort = settings["port"].toInt();

    setHost(serverAddress);
    setPort(quint16(serverPort));

    QString log = QString("Ustawiono: host= %1 , port= %2. \n").arg(serverAddress).arg(serverPort);
    w_logsWindow::Instance()->addLog(log);
}

void c_connectionToServerController::runSocket()
{    
    w_logsWindow::Instance()->addSeparator();

    QString log = QString("Running socket. Server: %1:%2. \n "
                          "socket->connectToHost( %1, %2 ) \n").arg(getHost()).arg(getPort());
    w_logsWindow::Instance()->addLog(log);

    w_logsWindow::Instance()->addSeparator();

    socket->connectToHost( getHost(), getPort() );
    socket->open(QIODevice::ReadWrite);

    w_logsWindow::Instance()->addSeparator();
}

c_connectionToServerController::c_connectionToServerController(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket( this );

    connect(socket, SIGNAL( connected() ), this, SLOT( socketConnected() ) );
    connect(socket, SIGNAL( disconnected() ), this, SLOT( socketDisconnected() ) );
    connect(socket, SIGNAL( error(QAbstractSocket::SocketError) ), this, SLOT( socketError(QAbstractSocket::SocketError) ) );
    connect(socket, SIGNAL( hostFound() ), this, SLOT( socketFoundHost() ) );
    connect(socket, SIGNAL( stateChanged(QAbstractSocket::SocketState) ), this, SLOT( socketStateChanged(QAbstractSocket::SocketState) ) );
    connect(socket, SIGNAL( readyRead() ), this, SLOT( socketReadyRead() ) );
    connect(socket, SIGNAL( bytesWritten(qint64) ), this, SLOT( socketBytesWritten(qint64) ) );


    QString log = QString("Socket connections configured. \n");
    w_logsWindow::Instance()->addLog(log);
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
    w_logsWindow::Instance()->addLog(log);
}

void c_connectionToServerController::socketDisconnected()
{
    QString log = QString("Socket disconnected by the server: %1:%2. \n").arg(getHost()).arg(getPort());
    w_logsWindow::Instance()->addLog(log);
}

void c_connectionToServerController::socketError(QAbstractSocket::SocketError socketError)
{
    QString log = QString("Socket error: %1. \n").arg(socketError);
    w_logsWindow::Instance()->addLog(log);
}

void c_connectionToServerController::socketStateChanged(QAbstractSocket::SocketState socketState)
{
    QString log = QString("Socket changed state: %1. \n").arg(socketState);
    w_logsWindow::Instance()->addLog(log);
}

void c_connectionToServerController::socketFoundHost()
{
    QString log = QString("Socket found host \n");
    w_logsWindow::Instance()->addLog(log);
}

void c_connectionToServerController::socketReadyRead()
{
    QString log = QString("Socket Reading: \n");
    w_logsWindow::Instance()->addLog(log);

    QDataStream stream(socket);

    QDataStream rs(socket);
    rs.setVersion(QDataStream::Qt_6_0);


    QByteArray data;
    quint32 data_size;

    rs.startTransaction();

    rs >> data_size >> data; // try to read packet atomically

    if (!rs.commitTransaction())
        return;     // wait for more data

    w_logsWindow::Instance()->addLog(data);

    emit(this->dataReceived(data_size, data));
}

void c_connectionToServerController::socketBytesWritten(qint64 bytes)
{
    QString log = QString("%1 has been written. \n").arg(bytes);
    w_logsWindow::Instance()->addLog(log);
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
    w_logsWindow::Instance()->addLog(log);

    socket->close();
}

void c_connectionToServerController::sendData(QByteArray data)
{    
    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_6_0);

    c_LogsController::Instance()->saveLogToFile(QString("c_connectionToServerController::sendData"), QString("%1").arg(getSocket()->socketDescriptor()), data);

    socketStream << static_cast<quint32>(data.size());
    socketStream << data;
}

QString c_connectionToServerController::getHost() const
{
    return host;
}

void c_connectionToServerController::setHost(const QString &value)
{
    host = value;
}
