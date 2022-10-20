#include "c_processescontroller.h"
#include "c_processescontrollerthread.h"

c_processesController::c_processesController(QObject *parent)
    : QObject{parent}
{
    mThread.reset(new c_processesControllerThread);
    moveToThread(mThread.get());

    connect(this, SIGNAL(openedModuleProcessesNumberChanged(int)), this, SLOT(processesNumberChanged(int)));
    connect(this, SIGNAL(newModuleProcessStartServer()), this->thread(), SLOT(startServer()));
    connect(this, SIGNAL(noModuleProcessStopServer()), this->thread(), SLOT(stopServer()));
}

c_processesController::~c_processesController()
{
    QMetaObject::invokeMethod(this, "cleanUpThread");
    mThread->wait();
}

void c_processesController::setUpThread()
{
    mThread->setMyParentConnector(this);
    mThread->setId( c_ThreadController::generateThreadId() );
    mThread->setParentIdentifier( QString("c_processesController") );
    mThread->setNote("Proces zarządzania uruchomionymi modułami.");

    emit threadAssigned(mThread.get());
}

void c_processesController::removeOpenedModuleProcessConnections()
{
    c_moduleProcessConnection * temp;
    for(int i = 0; i < this->openedModuleProcessConnections.size();)
    {
        temp = openedModuleProcessConnections.takeAt(0);
        delete temp;
    }
}

void c_processesController::removeOpenedModulesProcesses()
{
    c_moduleProcess * temp;
    for(int i = 0; i < this->openedModulesProcesses.size();)
    {
        temp = openedModulesProcesses.takeAt(0);
        delete temp;
    }
}

w_logsWindow *c_processesController::getLogs() const
{
    return logs;
}

void c_processesController::setLogs(w_logsWindow *newLogs)
{
    logs = newLogs;
}

void c_processesController::newModuleProcessConnection(c_moduleProcessConnection *moduleConnection)
{
    this->openedModuleProcessConnections.append(moduleConnection);
    QString log = QString("c_processesController::newModuleProcessConnection(c_moduleProcessConnection *moduleConnection)  \n"
                          "dodano połączenie \n");
    emit newLog(log);
}

void c_processesController::removeModuleProcessConnection(c_moduleProcessConnection *moduleConnection)
{
    openedModuleProcessConnections.removeAll(moduleConnection);
    openedModuleProcessConnections.squeeze();

}

void c_processesController::removeModuleProcessConnection(qintptr id)
{
    c_moduleProcessConnection * temp;
    for(int i = 0; i < this->openedModuleProcessConnections.size();)
    {
        if(openedModuleProcessConnections[i]->getSocketDescriptor() == id)
        {
            temp = openedModuleProcessConnections.takeAt(i);
            delete temp;
            break;
        }
    }
}

void c_processesController::removeModuleProcessConnection(quint32 index)
{
    c_moduleProcessConnection * temp;
    temp = openedModuleProcessConnections.takeAt(index);
    delete temp;
}

void c_processesController::newModuleProcess(c_moduleProcess *moduleProcess)
{
    this->openedModulesProcesses.append(moduleProcess);
    QString log = QString("c_processesController::newModuleProcess(c_moduleProcess *moduleProcess)  \n"
                          "dodano połączenie \n");
    emit newLog(log);
    emit openedModuleProcessesNumberChanged(openedModulesProcesses.size());
}

void c_processesController::removeModuleProcess(c_moduleProcess *moduleProcess)
{
    openedModulesProcesses.removeAll(moduleProcess);
    openedModulesProcesses.squeeze();
    emit openedModuleProcessesNumberChanged(openedModulesProcesses.size());
}

void c_processesController::removeModuleProcess(QByteArray hashedName)
{
    c_moduleProcess * temp;
    for(int i = 0; i < this->openedModulesProcesses.size();)
    {
        if(openedModulesProcesses[i]->getModuleProcessNameHash() == hashedName)
        {
            temp = openedModulesProcesses.takeAt(i);
            delete temp;
            break;
        }
    }
    emit openedModuleProcessesNumberChanged(openedModulesProcesses.size());
}

void c_processesController::removeModuleProcess(quint32 index)
{
    c_moduleProcess * temp;
    temp = openedModulesProcesses.takeAt(index);
    delete temp;
    emit openedModuleProcessesNumberChanged(openedModulesProcesses.size());
}


c_processesControllerThread*c_processesController::thread() const
{
    return mThread.get();
}

void c_processesController::setThread(std::unique_ptr<c_processesControllerThread> newThread)
{
    mThread = std::move(newThread);
}

void c_processesController::cleanUpThread()
{
    mThread->quit();
}

void c_processesController::processesNumberChanged(int processesNumber)
{
    if(processesNumber == 0) {
        emit noModuleProcessStopServer();
    }
    if(processesNumber == 1) {
        emit newModuleProcessStartServer();
    }
}
