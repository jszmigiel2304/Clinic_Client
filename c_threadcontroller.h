#ifndef C_THREADCONTROLLER_H
#define C_THREADCONTROLLER_H

#include "c_mystructures.h"
#include "mythread.h"

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
#include <QRandomGenerator>

class c_ThreadController : public QObject
{
    Q_OBJECT
public:
    explicit c_ThreadController(QObject *parent = nullptr);
    ~c_ThreadController();

    static qint32 generateThreadId();

    const QList<MyThread *> &getThreadsList() const;
    void setThreadsList(const QList<MyThread *> &newThreadsList);

public slots:
    void newThread(MyThread * thread);
    void dataReceived(myStructures::threadData data);
    void cleanUpThread();
    void stopAllThreads();


private:
    void cleanUp();

    QList<MyThread *> threadsList;

    std::unique_ptr<MyThread> mThread;

private slots:

signals:
    void newLog(QString log);
    void passDataToThread(myStructures::threadData data);
    void forceLogOutUser();



signals:

};

#endif // C_THREADCONTROLLER_H
