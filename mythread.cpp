#include "mythread.h"

MyThread::MyThread(qint32 id, QString parentIdent, QString note,  QObject *parent)
    : QThread{parent}, idThread(id), parentIdentifier(parentIdent), note(note)
{
    connect(this, SIGNAL(started()), this, SLOT(threadStarted()));
    connect(this, SIGNAL(finished()), this, SLOT(threadFinished()));
}

void MyThread::run()
{
    exec();
}

void MyThread::processData(threadData data)
{

}

quint32 MyThread::getId() const
{
    return idThread;
}

void MyThread::setId(quint32 newId)
{
    idThread = newId;
}

const QString &MyThread::getParentIdentifier() const
{
    return parentIdentifier;
}

void MyThread::setParentIdentifier(const QString &newParentIdentifier)
{
    parentIdentifier = newParentIdentifier;
}

const QString &MyThread::getNote() const
{
    return note;
}

void MyThread::setNote(const QString &newNote)
{
    note = newNote;
}

void MyThread::dataReceived(threadData data)
{
    //przetworzenie otrzymanych danych z ThreadController

    dynamic_cast<MyThread *>(myParentConnector->thread())->processData(data);
}

QObject *MyThread::getMyParentConnector() const
{
    return myParentConnector;
}

void MyThread::setMyParentConnector(QObject *newMyParentConnector)
{
    myParentConnector = newMyParentConnector;
}


void MyThread::threadStarted()
{
}

void MyThread::threadFinished()
{
    deleteLater();
}

