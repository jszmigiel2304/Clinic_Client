#ifndef C_MODULEPROCESSCONNECTION_H
#define C_MODULEPROCESSCONNECTION_H

#include "cv_processdata.h"
#include "c_parser.h"

#include <QObject>
#include <QTextStream>
#include <QDataStream>
#include <QMessageBox>
#include <QJsonDocument>
#include <QByteArray>
#include <QBuffer>
#include <QIODeviceBase>
#include <QIODevice>
#include <QTimer>
#include <QJsonDocument>
#include <QDataStream>
#include <QLocalSocket>

class c_moduleProcessConnection : public QObject, public cv_ProcessData
{
    Q_OBJECT
public:
    enum LocalJsonContent { EMPTY = 0x00000000, ERRORS = 0x00000001, CONNECTION_TO_PROCESS_REQUEST = 0x00000002};
    explicit c_moduleProcessConnection(qintptr ID, QObject *parent = nullptr);

    qintptr getSocketDescriptor() const;
    void setSocketDescriptor(qintptr newSocketDescriptor);

    bool getConnectedToProcess() const;
    void setConnectedToProcess(bool newConnectedToProcess);

public slots:
    void sendConnectionToProcessSettingsRequest();

private:
    QLocalSocket * socket;
    qintptr socketDescriptor;
    bool connectedToProcess;

private slots:
    //from QLocalSocket
    void connected();
    void disconnected();
    void errorOccurred(QLocalSocket::LocalSocketError socketError);
    void stateChanged(QLocalSocket::LocalSocketState socketState);
    //from QIODevice
    void aboutToClose();
    void bytesWritten(qint64 bytes);
    void channelBytesWritten(int channel, qint64 bytes);
    void channelReadyRead(int channel);
    void readChannelFinished();
    void readyRead();
    //------------------
    void parseReceivedPacket(quint64 size, QByteArray data, qintptr socketDescriptor);
    void sendDataToModuleProcess(myStructures::packet);

signals:
    void newLog(QString log);
    void error(QLocalSocket::LocalSocketError error);
    void dataRead(quint64 size, QByteArray data, qintptr socketDescriptor);
    void sendDataToModuleProcessSignal(myStructures::packet packet);
    void moduleProcessConnectionFinished(c_moduleProcessConnection * moduleProcessConnection);

};

#endif // C_MODULEPROCESSCONNECTION_H
