#include "c_processescontroller.h"
#include "c_processescontrollerthread.h"

c_processesController::c_processesController(QObject *parent)
    : QObject{parent}
{
    logs = w_logsWindow::Instance();

    mThread.reset(new c_processesControllerThread);
    moveToThread(mThread.get());
}

c_processesController::~c_processesController()
{
    if(thread()->getLocalServer()->isListening())
        thread()->stopServer();

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

    emit newLog(QString("c_processesController::newModuleProcess(c_moduleProcess *moduleProcess)  \n"
                        "dodano utworzony proces do listy \n"));
}

void c_processesController::removeModuleProcess(c_moduleProcess *moduleProcess)
{
    openedModulesProcesses.removeAll(moduleProcess);
    openedModulesProcesses.squeeze();
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
}

void c_processesController::removeModuleProcess(quint32 index)
{
    c_moduleProcess * temp;
    temp = openedModulesProcesses.takeAt(index);
    delete temp;
}

void c_processesController::removeAllModuleProcesses()
{
    while(openedModulesProcesses.size() != 0) {
        openedModulesProcesses.takeFirst()->close();

        //Wysłać request zamkniecia + QProcess::waitForFinish
    }
}

void c_processesController::connectProcessWithConnection(QMap<QString, QVariant> connectionSettings, qintptr socketDescriptor)
{
    QByteArray server_id = connectionSettings["server_id"].toByteArray();
    QByteArray module_id = connectionSettings["module_id"].toByteArray();
    qint32 thread_id = connectionSettings["thread_id"].toInt();

    c_moduleProcess * tempModProcess = nullptr;
    for(int i=0; i<openedModulesProcesses.size(); i++) {
        if( openedModulesProcesses[i]->getModuleProcessNameHash() == module_id && openedModulesProcesses[i]->getThreadId() == thread_id) {
            tempModProcess = openedModulesProcesses[i];
            break;
        }
    }
    c_moduleProcessConnection * tempModProcessConnection = nullptr;
    for(int i=0; i<openedModuleProcessConnections.size(); i++) {
        if(openedModuleProcessConnections[i]->getSocketDescriptor() == socketDescriptor) {
            tempModProcessConnection = openedModuleProcessConnections[i];
            tempModProcess->setConnection( tempModProcessConnection );
            tempModProcessConnection->setConnectedToProcess(true);

            //wysyłam potwierdzenie nawiazania połączenia
            c_Parser parser;
            QPair<QByteArray, QByteArray> pair = parser.prepareConnectionEstablishedConfirmationPacket(myTypes::CLINIC_MODULE_PROCESS_CONTROLLER, thread_id);

            myStructures::packet packet;
            packet.md5_hash = pair.first;
            packet.packet_to_send = pair.second;
            packet.wait_for_reply = false;

            emit tempModProcessConnection->sendDataToModuleProcessSignal(packet);
        }
    }
}

void c_processesController::moduleClosed(c_moduleProcess *proces, int exitCode, QProcess::ExitStatus exitStatus)
{
    if(exitCode == 0) {
        emit newLog(QString("c_processesController::moduleClosed(c_moduleProcess *proces, int exitCode, QProcess::ExitStatus exitStatus)  \n"
                            "Proces zatrzymany. Usuwam z listy. ExitCode: %1 \t ExitStatus: %2\n").arg( QString("%1").arg(exitCode), (exitStatus == QProcess::NormalExit ? QString("NormalExit") : QString("CrashExit ") ) ));

    } else {
        switch(exitCode) {
        case 0xff01: {emit newLog(QString("Argument error: ServerName \nModule close. \n")); break;}
        case 0xff02: {emit newLog(QString("Argument error: ModuleName \nModule close. \n")); break;}
        case 0xff03: {emit newLog(QString("Argument error: UserId or UserName \nModule close. \n")); break;}
        default: {break;}
        }
    }

    removeModuleProcess(proces);
}


c_processesControllerThread*c_processesController::thread() const
{
    return mThread.get();
}

void c_processesController::setThread(std::unique_ptr<c_processesControllerThread> newThread)
{
    mThread = std::move(newThread);
}

QString c_processesController::getHashServerName()
{
    return thread()->getLocalServer()->hashServerName();
}

c_moduleProcess *c_processesController::getProcess(qint32 threadID)
{
    for(int i=0; i<openedModulesProcesses.size(); i++) {
        if( openedModulesProcesses[i]->getThreadId() == threadID ) return openedModulesProcesses[i];
    }

    return nullptr;
}

void c_processesController::cleanUpThread()
{
    mThread->quit();
}

