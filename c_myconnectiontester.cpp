#include "c_myconnectiontester.h"


c_MyConnectionTester::c_MyConnectionTester(QObject *parent)  : QObject(parent)
{

}

c_MyConnectionTester::c_MyConnectionTester(QString address, quint16 port, QObject *parent)  : QObject(parent)
{
    this->address = address;
    this->port = port;
}

QString c_MyConnectionTester::getAddress() const
{
    return address;
}

void c_MyConnectionTester::setAddress(const QString &value)
{
    address = value;
}

quint16 c_MyConnectionTester::getPort() const
{
    return port;
}

void c_MyConnectionTester::setPort(quint16 value)
{
    port = value;
}

bool c_MyConnectionTester::TestConnection(int time)
{
    QTcpSocket * socket  = new QTcpSocket(this);
    bool testResult;

    socket->connectToHost(this->address, this->port);

    if(socket->waitForConnected(time))
    {
        testResult = true;
    }
    else
    {
        testResult = false;
    }

    socket->close();
    socket->deleteLater();

    return testResult;
}

