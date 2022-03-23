#ifndef C_MYCONNECTIONTESTER_H
#define C_MYCONNECTIONTESTER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class c_MyConnectionTester : public QObject
{
    Q_OBJECT
public:
    explicit c_MyConnectionTester(QObject * parent = nullptr);
    explicit c_MyConnectionTester(QString address, quint16 port, QObject * parent = nullptr);


    QString getAddress() const;
    void setAddress(const QString &value);

    quint16 getPort() const;
    void setPort(quint16 value);

    virtual bool TestConnection(int time = 3000);

public slots:


private:
    QString address;
    quint16 port;
};

#endif // C_MYCONNECTIONTESTER_H
