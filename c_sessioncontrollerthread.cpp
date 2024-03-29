#include "c_sessioncontrollerthread.h"
#include "c_sessioncontroller.h"
#include "c_actionexecutive.h"

c_SessionControllerThread::c_SessionControllerThread(qint32 id, QString parentIdent, QString note, QObject *parent)
    : MyThread{id, parentIdent, note, parent}
{
    getsessionSettingsTimer = new QTimer();
    connect(getsessionSettingsTimer, SIGNAL(timeout()), this, SLOT(getsessionSettingsTimerTimeOut()));


    connect(this, SIGNAL(started()), this, SLOT(threadStarted()));
}

c_SessionControllerThread::~c_SessionControllerThread()
{

    if(getsessionSettingsTimer != nullptr) getsessionSettingsTimer->deleteLater();
}

void c_SessionControllerThread::run()
{
    exec();
}

void c_SessionControllerThread::getSessionSettingsFromServer()
{
    c_Parser parser;


    QPair<QByteArray, QByteArray> pair = parser.prepareGetSessionSettingsPacket(dynamic_cast<c_SessionController *>(myParentConnector)->getLoggedUser()->getId(),
                                                                                dynamic_cast<c_SessionController *>(myParentConnector)->getLoggedUser()->getName(),
                                                                                dynamic_cast<c_SessionController *>(myParentConnector)->getLoggedUser()->getEncryptedPassword(false, true),
                                                                                dynamic_cast<c_SessionController *>(myParentConnector)->getIdentifier(),
                                                                                getNameThreadDestination(),
                                                                                getId());

    myStructures::packet packet;
    packet.md5_hash = pair.first;
    packet.packet_to_send = pair.second;
    packet.wait_for_reply = true;

    emit processingStarted(QString("Pobieranie wymaganych do utworzenia sesji danych z serwera."));
    emit sendToServer(packet);
}


quint32 c_SessionControllerThread::getSessionExpireSeconds() const
{
    return sessionExpireSeconds;
}

void c_SessionControllerThread::setSessionExpireSeconds(quint32 newSessionExpireSeconds)
{
    sessionExpireSeconds = newSessionExpireSeconds;

    emit sessionTimeSet(sessionExpireSeconds);
}


void c_SessionControllerThread::processData(myStructures::threadData data, qintptr socketDescriptor)
{
    if( this->getId() == data.thread_id && (data.thread_dest == myTypes::CLINIC_SESSION_CONTROLLER  || data.thread_dest == myTypes::CLINIC_ERROR_CONTROLLER) )
    {
        c_actionExecutive *executive = new c_actionExecutive();
        connect( executive, SIGNAL(getSessionSettingsFromServerResultReady(QMap<QString, QVariant>)), this, SLOT(sessionSettingsReceivedFromServer(QMap<QString, QVariant>)) );
        connect( executive, SIGNAL(updateSessionStatusConfirmationReceived()), this, SLOT(sessionStateUpdateConfirmationReceivedFromServer()) );
        connect( executive, SIGNAL(updateSessionFileConfirmationReceived()), this, SLOT(sessionFileUpdateConfirmationReceivedFromServer()) );

        executive->processData(data, socketDescriptor);

        executive->deleteLater();
    }
    else
    {
        //błąd złegodopasownia wątku
    }
}

void c_SessionControllerThread::sessionRun()
{    
    emit sessionTimeSet(sessionExpireSeconds);

    dynamic_cast<c_SessionController *>(myParentConnector)->setState(myTypes::STARTED);
    dynamic_cast<c_SessionController *>(myParentConnector)->setOpened(true);

    emit processingFinished();
}

void c_SessionControllerThread::unlockSession()
{

}


void c_SessionControllerThread::sendSessionFileToServer(QUuid id_session, QByteArray fileData)
{
    c_Parser parser;

    QPair<QByteArray, QByteArray> pair = parser.prepareSendSessionFileToServerPacket(id_session,
                                                                                     fileData,
                                                                                     dynamic_cast<c_SessionController *>(myParentConnector)->getLoggedUser()->getId(),
                                                                                     dynamic_cast<c_SessionController *>(myParentConnector)->getLoggedUser()->getName(),
                                                                                     dynamic_cast<c_SessionController *>(myParentConnector)->getLoggedUser()->getEncryptedPassword(false, true),
                                                                                     getNameThreadDestination(),
                                                                                     getId());

    myStructures::packet packet;
    packet.md5_hash = pair.first;
    packet.packet_to_send = pair.second;
    packet.wait_for_reply = true;

    emit sendToServer(packet);
}

void c_SessionControllerThread::sendSessionStateToServer(QUuid id_session, qint32 state)
{
    c_Parser parser;

    QPair<QByteArray, QByteArray> pair = parser.prepareSendSessionStateToServerPacket(id_session,
                                                                                      state,
                                                                                      dynamic_cast<c_SessionController *>(myParentConnector)->getLoggedUser()->getId(),
                                                                                      dynamic_cast<c_SessionController *>(myParentConnector)->getLoggedUser()->getName(),
                                                                                      dynamic_cast<c_SessionController *>(myParentConnector)->getLoggedUser()->getEncryptedPassword(false, true),
                                                                                      getNameThreadDestination(),
                                                                                      getId());

    myStructures::packet packet;
    packet.md5_hash = pair.first;
    packet.packet_to_send = pair.second;
    packet.wait_for_reply = true;

    emit sendToServer(packet);
}



void c_SessionControllerThread::appIDLEdetected()
{

    dynamic_cast<c_SessionController *>(myParentConnector)->setState(myTypes::PAUSED_NOT_SAVED);
    //zapisac do pliku
    sendSessionFileToServer(dynamic_cast<c_SessionController *>(myParentConnector)->getIdentifier(), dynamic_cast<c_SessionController *>(myParentConnector)->readEntireSessionFile());
    dynamic_cast<c_SessionController *>(myParentConnector)->setState(myTypes::PAUSED_SAVED);

    emit idleDetected();
    // komunikat połącz ponownie
}

void c_SessionControllerThread::sessionClose()
{
    waitForSessionSavedBeforeClose = true;
    dynamic_cast<c_SessionController *>(myParentConnector)->setState(myTypes::CLOSED_NOT_SAVED);
    //zapisac do pliku
    sendSessionFileToServer(dynamic_cast<c_SessionController *>(myParentConnector)->getIdentifier(), dynamic_cast<c_SessionController *>(myParentConnector)->readEntireSessionFile());

    c_Parser parser;


    QPair<QByteArray, QByteArray> pair = parser.prepareUpdateSessionCloseTimeToServerPacket(dynamic_cast<c_SessionController *>(myParentConnector)->getIdentifier(),
                                                                                      QDateTime::currentDateTime(),
                                                                                      dynamic_cast<c_SessionController *>(myParentConnector)->getLoggedUser()->getId(),
                                                                                      dynamic_cast<c_SessionController *>(myParentConnector)->getLoggedUser()->getName(),
                                                                                      dynamic_cast<c_SessionController *>(myParentConnector)->getLoggedUser()->getEncryptedPassword(false, true),
                                                                                      getNameThreadDestination(),
                                                                                      getId());

    myStructures::packet packet;
    packet.md5_hash = pair.first;
    packet.packet_to_send = pair.second;
    packet.wait_for_reply = true;

    emit sendToServer(packet);

    dynamic_cast<c_SessionController *>(myParentConnector)->setState(myTypes::CLOSED_SAVED);
    dynamic_cast<c_SessionController *>(myParentConnector)->setOpened(false);

    emit sessionClosedCorrectly();

}


void c_SessionControllerThread::getsessionSettingsTimerTimeOut()
{
    if(dynamic_cast<c_SessionController *>(myParentConnector)->getLoggedUser()->getIsLogged()) {
        if(!dynamic_cast<c_SessionController *>(myParentConnector)->isValid()) {
            emit processingError(QString("Dane nie pobrane. Ponawiam pobieranie."));
            getSessionSettingsFromServer();
            getsessionSettingsTimer->start(10000);
        } else {
            getsessionSettingsTimer->stop();
        }
    } else {
        emit processingError(QString("Użytkownik nie zalogowany. Oczekuje na zalogowanie."));
        getsessionSettingsTimer->start(5000);
    }
}

void c_SessionControllerThread::threadStarted()
{
}

void c_SessionControllerThread::sessionSettingsReceivedFromServer(QMap<QString, QVariant> settings)
{
    dynamic_cast<c_SessionController *>(myParentConnector)->setUpSession(settings);
}

void c_SessionControllerThread::sessionStateUpdateConfirmationReceivedFromServer()
{
}

void c_SessionControllerThread::sessionFileUpdateConfirmationReceivedFromServer()
{
}

void c_SessionControllerThread::sessionUnlockConfirmationReceived()
{
    dynamic_cast<c_SessionController *>(myParentConnector)->setState(myTypes::RESTARTED);
    emit sessionTimeSet(sessionExpireSeconds);
}

void c_SessionControllerThread::configureSession(QString username, QString role)
{
    emit processingStarted(QString("Konfigurowanie sesji dla %1 [%2]...").arg(username, role));
    dynamic_cast<c_SessionController *>(myParentConnector)->setIdentifier();

    if(!dynamic_cast<c_SessionController *>(myParentConnector)->isValid()) getsessionSettingsTimer->start(10);
}

myTypes::ThreadDestination c_SessionControllerThread::getNameThreadDestination() const
{
    return nameThreadDestination;
}
