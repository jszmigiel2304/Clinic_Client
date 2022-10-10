#ifndef C_CONNECTIONTOSERVERCONTROLLER_H
#define C_CONNECTIONTOSERVERCONTROLLER_H


#include "w_logswindow.h"
#include "c_logscontroller.h"
#include "c_mystructures.h"
#include "mythread.h"
#include "c_threadcontroller.h"
#include "cv_processdata.h"
#include "c_parser.h"

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDataStream>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QList>
#include <QTime>
#include <QTimer>

class c_connectionToServerController : public QObject, public cv_ProcessData
{
    Q_OBJECT
public:
    explicit c_connectionToServerController(QObject *parent = nullptr);
    ~c_connectionToServerController();

    void start();

    void processData(threadData data) override;


    QString getHost() const;
    void setHost(const QString &value);

    quint16 getPort() const;
    void setPort(const quint16 &value);

    QTcpSocket *getSocket() const;
    void setSocket(QTcpSocket *value);


    ThreadDestination getNameThreadDestination() const;

    w_logsWindow *getLogs() const;
    void setLogs(w_logsWindow *newLogs);

    c_LogsController *getLogsController() const;
    void setLogsController(c_LogsController *newLogsController);


public slots:
    void setConnection(QMap<QString, QVariant> settings);
    void closeConnection();
    void sendData(packet packet);
    void passDataToBuffer(packet packet);
    void sendPackets();
    void receiveConfirmationReceived(threadData data);
    void replyReceivedRemoveFromList(QByteArray ref_md5);

private:
    QTcpSocket * socket;
    QString host;
    quint16 port;
    const ThreadDestination nameThreadDestination = CLINIC_CONNECTION_CONTROLLER;
    bool tryReconnect;

    QList<packet> packetsToSend;
    QList<packet> waitingForReplyPackets;
    QList<packet> waitingForReceiveConfirmation;

    QTimer * reconnectTimer;
    QTimer * sendAgainPackets;

    w_logsWindow *logs;
    c_LogsController *logsController;

    bool isWaitingForReply(packet packet);
    bool isWaitingForReceiveConfirmation(packet packet);

private slots:
    void runSocket();
    //from Qtcpsocket
    void socketConnected();
    void socketDisconnected();
    void socketError(QAbstractSocket::SocketError socketError);
    void socketStateChanged(QAbstractSocket::SocketState socketState);
    void socketFoundHost();

    //from QIODevice
    void socketReadyRead();
    void socketBytesWritten(qint64 bytes);

    void reconnectTimerTimeOut();
    void sendAgainPacketsTimerTimeOut();

signals:
    void newLog(QString log);
    void threadAssigned(MyThread * thread);
    void dataReceived(quint64 data_size, QByteArray data);
    void connectionToServerStateChanged(QAbstractSocket::SocketState state);
    void socketDisconnectedLogOutUser();
    void packetInBuffer();

};

#endif // C_CONNECTIONTOSERVERCONTROLLER_H
