#ifndef C_THREADCONTROLLER_H
#define C_THREADCONTROLLER_H

#include "c_mystructures.h"

#include <QApplication>
#include <QObject>
#include <QActionGroup>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QString>
#include <QDataStream>
#include <QByteArray>
#include <QList>

class c_ThreadController : public QObject
{
    Q_OBJECT
public:
    static c_ThreadController *Instance();
    ~ c_ThreadController();

    void AddReceivedData(threadData data);

private:
    explicit c_ThreadController(QObject *parent = nullptr);

    QList<threadData> dataReceivedFromServer;

signals:

};

#endif // C_THREADCONTROLLER_H
