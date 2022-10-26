#include "c_threadcontroller.h"

//c_ThreadController *c_ThreadController::Instance()
//{
//    static c_ThreadController * instance = nullptr;
//    if ( instance == nullptr ) {
//        instance = new c_ThreadController();
//    }

//    return instance;
//}

c_ThreadController::~c_ThreadController()
{
    cleanUp();
    QMetaObject::invokeMethod(this, "cleanUpThread");
    mThread->wait();
}

qint32 c_ThreadController::generateThreadId()
{
    return QRandomGenerator::global()->bounded(quint32(100000));
}

void c_ThreadController::newThread(MyThread *thread)
{
    threadsList.push_front(thread);
    connect(thread, SIGNAL(newData(myStructures::threadData, qintptr)), thread, SLOT(dataReceived(myStructures::threadData, qintptr)));
}

void c_ThreadController::dataReceived(myStructures::threadData data, qintptr socketDescriptor)
{
    for(int i = 0; i < threadsList.length(); i++)
    {
        if(threadsList[i]->getId() == data.thread_id)
        {
            emit threadsList[i]->newData(data, socketDescriptor);
            break;
        }
    }

}

c_ThreadController::c_ThreadController(QObject *parent) : QObject(parent)
{
    mThread.reset(new MyThread);
    moveToThread(mThread.get());
    mThread->start();

    mThread->setMyParentConnector(this);
    mThread->setId( c_ThreadController::generateThreadId() );
    mThread->setParentIdentifier( QString("c_ThreadController") );
    mThread->setNote("Proces zarządzania aktywnymi procesami.");

    newThread(mThread.get());
}

void c_ThreadController::cleanUp()
{
    MyThread * temp;
    for(int i = 0; i < this->threadsList.size();)
    {
        temp = threadsList.takeAt(0);
        delete temp;
    }

}

void c_ThreadController::cleanUpThread()
{
    mThread->quit();
}

void c_ThreadController::stopAllThreads()
{
    emit forceLogOutUser();

    MyThread * temp;
    for(int i = 0; i < this->threadsList.size();)
    {
        temp = threadsList.takeAt(0);
        temp->exit(0);
    }
}

const QList<MyThread *> &c_ThreadController::getThreadsList() const
{
    return threadsList;
}

void c_ThreadController::setThreadsList(const QList<MyThread *> &newThreadsList)
{
    threadsList = newThreadsList;
}
