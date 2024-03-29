#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "cv_processdata.h"
#include "c_mystructures.h"

#include <QThread>
#include <QObject>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(qint32 id = -1, QString parentIdent = QString(), QString note = QString(), QObject *parent = nullptr);

    void run() override;

    quint32 getId() const;
    void setId(quint32 newId);

    const QString &getParentIdentifier() const;
    void setParentIdentifier(const QString &newParentIdentifier);

    const QString &getNote() const;
    void setNote(const QString &newNote);

    QObject *getMyParentConnector() const;
    void setMyParentConnector(QObject *newMyParentConnector);

public slots:
    void dataReceived(myStructures::threadData data, qintptr socketDescriptor);
    virtual void processData(myStructures::threadData data, qintptr socketDescriptor);


protected:
    QObject * myParentConnector;
    quint32 idThread;
    QString parentIdentifier;
    QString note;

private slots:
    virtual void threadStarted();
    virtual void threadFinished();


signals:
    void resultReady(quint32 resultBefore, quint32 resultAfter);
    void newData(myStructures::threadData data, qintptr socketDescriptor);

    void sendToServer(myStructures::packet packet);
    void processingError(QString error);
    void processingStarted(QString text);
    void processingFinished();

};

#endif // MYTHREAD_H
