#include "c_loggeduserthread.h"
#include "c_loggeduser.h"
#include "c_actionexecutive.h"

c_loggedUserThread::c_loggedUserThread(qint32 id, QString parentIdent, QString note, QObject *parent)
    : MyThread{id, parentIdent, note, parent}
{
    connect(this, SIGNAL(logInToServer(qint32, QString, QString)), this, SLOT(logIn(qint32, QString, QString)), Qt::DirectConnection);
    connect(this, SIGNAL(started()), this, SLOT(threadStarted()));


}

void c_loggedUserThread::run()
{
    exec();
}

bool c_loggedUserThread::getLoggingState() const
{
    return loggingState;
}

void c_loggedUserThread::setLoggingState(bool newLoggingState)
{
    loggingState = newLoggingState;
}

void c_loggedUserThread::processData(threadData data)
{
    if( this->getId() == data.thread_id && (data.thread_dest == CLINIC_LOGGED_USER_CONTROLLER  || data.thread_dest == CLINIC_ERROR_CONTROLLER) )
    {
        c_actionExecutive *executive = new c_actionExecutive();
        //executive->moveToThread(mThread.get());
        connect( executive, SIGNAL(newLog(QString)), dynamic_cast<c_loggedUser *>(myParentConnector)->getLogs(), SLOT(addLog(QString)) );

        connect( executive, SIGNAL(getUserIdResultReady(qint32)), this, SLOT(userIdReceivedFromServer(qint32)) );
        connect( executive, SIGNAL(logInConfirmationReady(logInConfirmation)),this, SLOT(logInConfirmationReceivedFromServer(logInConfirmation)) );
        connect( executive, SIGNAL(logOutConfirmationReady(logOutConfirmation)), this, SLOT(logOutConfirmationReceivedFromServer(logOutConfirmation)) );
        connect( executive, SIGNAL(unlockConfirmationReceived(bool)), this, SLOT(unlockConfirmationReceived(bool)) );

        executive->processData(data);

        executive->deleteLater();
    }
    else
    {
        //błąd złegodopasownia wątku

        QString errMsg = QString("Thread ERROR. \n Wrong THREAD DESTINATION or THREAD ID");
        emit dynamic_cast<c_loggedUser *>(myParentConnector)->newLog(errMsg);
    }
}

void c_loggedUserThread::sessionLocked()
{
    emit userAuthorizationDataNeededToUnlock(dynamic_cast<c_loggedUser *>(myParentConnector)->getName());
}

void c_loggedUserThread::getUserId(QString userName, QString userPassword)
{
    loggingTimer->stop();

    dynamic_cast<c_loggedUser *>(myParentConnector)->setName(userName);
    dynamic_cast<c_loggedUser *>(myParentConnector)->setPassword(userPassword);

    c_Parser parser;
    QPair<QByteArray, QByteArray> pair = parser.prepareGetUserIdPacket(dynamic_cast<c_loggedUser *>(myParentConnector)->getName(),
                                                                       dynamic_cast<c_loggedUser *>(myParentConnector)->getEncryptedPassword(),
                                                                       getId());

    packet packet;
    packet.md5_hash = pair.first;
    packet.packet_to_send = pair.second;
    packet.wait_for_reply = true;

//    QByteArray packet = parser.prepareGetUserIdPacket(dynamic_cast<c_loggedUser *>(myParentConnector)->getName(),
//                                                      dynamic_cast<c_loggedUser *>(myParentConnector)->getPassword(),
//                                                      getId());

    emit logInProcessing(QString("Pobieranie danych użytkownika z serwera..."));
    emit sendToServer(packet);

//    loggingTimer = new QTimer();
//    connect(loggingTimer, SIGNAL(timeout()), this, SLOT(loggingTimerTimeout()));

    loggingTimer->start(5000);
}

void c_loggedUserThread::unlockOnIdle(QString userName, QString userPassword)
{
    c_Parser parser;

    QByteArray encryptedPassword;

    QByteArray encryptionSeed;
    QDataStream encryptionStream(&encryptionSeed, QIODevice::ReadWrite);
    encryptionStream.setVersion(QDataStream::Qt_6_0);
    encryptionStream << QString("test") << userPassword;

    QCryptographicHash hasher(QCryptographicHash::Md5);
    hasher.addData(encryptionSeed);
    encryptedPassword = hasher.result();

    QPair<QByteArray, QByteArray> pair = parser.unlockOnIdle(userName, encryptedPassword.toHex(), getId());

    packet packet;
    packet.md5_hash = pair.first;
    packet.packet_to_send = pair.second;
    packet.wait_for_reply = true;

    emit logInProcessing(QString("Pobieranie danych użytkownika z serwera..."));
    emit sendToServer(packet);
}

void c_loggedUserThread::logIn(qint32 id, QString name, QString password)
{
    loggingTimer->stop();
//    c_Parser parser;
//    QByteArray packet = parser.prepareLogInPacket(id, name, password, getId());

    c_Parser parser;
    QPair<QByteArray, QByteArray> pair = parser.prepareLogInPacket(id, name, password, getId());

    packet packet;
    packet.md5_hash = pair.first;
    packet.packet_to_send = pair.second;
    packet.wait_for_reply = true;

    emit logInProcessing(QString("Logowanie..."));
    emit sendToServer(packet);

    loggingTimer->start(5000);
}

void c_loggedUserThread::logOut(qint32 id, QString name, QString password)
{    
    emit aboutToLogOut();

    c_Parser parser;
    qint32 idc;
    QString namec, passwordc;
    if(id <= 0) idc = dynamic_cast<c_loggedUser *>(myParentConnector)->getId(); else idc = id;
    if(namec.isEmpty()) namec = dynamic_cast<c_loggedUser *>(myParentConnector)->getName(); else namec = name;
    if(password.isEmpty()) passwordc = dynamic_cast<c_loggedUser *>(myParentConnector)->getEncryptedPassword(); else passwordc = password;


    QPair<QByteArray, QByteArray> pair = parser.prepareLogOutPacket(idc, namec, passwordc, getId());

    packet packet;
    packet.md5_hash = pair.first;
    packet.packet_to_send = pair.second;
    packet.wait_for_reply = true;

    emit sendToServer(packet);
}

void c_loggedUserThread::logOutOffLine()
{
    emit dynamic_cast<c_loggedUser *>(myParentConnector)->newLog(QString("Połączenie przerwane. Wylogowano użytkownika. \n"));

    dynamic_cast<c_loggedUser *>(myParentConnector)->clearProperties();
    emit userNotLogged();
}

void c_loggedUserThread::userIdReceivedFromServer(qint32 userID)
{
    if(userID == -1) {
        loggingTimer->stop();
        emit logInError(QString("Błędne dane logowania."));
    } else {
        dynamic_cast<c_loggedUser *>(myParentConnector)->setId(userID);
        if(!dynamic_cast<c_loggedUser *>(myParentConnector)->getIsLogged()) {
            emit logInToServer(dynamic_cast<c_loggedUser *>(myParentConnector)->getId(),
                               dynamic_cast<c_loggedUser *>(myParentConnector)->getName(),
                               dynamic_cast<c_loggedUser *>(myParentConnector)->getEncryptedPassword());
        } else {
            loggingTimer->stop();
            emit logInFinished();
        }
    }
}

void c_loggedUserThread::logInConfirmationReceivedFromServer(logInConfirmation confirmation)
{
    loggingTimer->stop();

    dynamic_cast<c_loggedUser *>(myParentConnector)->setId(confirmation.id);
    dynamic_cast<c_loggedUser *>(myParentConnector)->setName(confirmation.name);
    dynamic_cast<c_loggedUser *>(myParentConnector)->setLoginTimeStamp(confirmation.logInTime);
    dynamic_cast<c_loggedUser *>(myParentConnector)->setEmail(confirmation.email);
    dynamic_cast<c_loggedUser *>(myParentConnector)->setVerified(confirmation.verified);
    dynamic_cast<c_loggedUser *>(myParentConnector)->setBlocked(confirmation.blocked);
    dynamic_cast<c_loggedUser *>(myParentConnector)->setRole(confirmation.role);
    dynamic_cast<c_loggedUser *>(myParentConnector)->setIsLogged(true);

    emit userLogged(dynamic_cast<c_loggedUser *>(myParentConnector)->getName(), dynamic_cast<c_loggedUser *>(myParentConnector)->getRole());
    emit logInFinished();

    emit dynamic_cast<c_loggedUser *>(myParentConnector)->newLog(QString("Udane logowanie. \n"));
}

void c_loggedUserThread::logOutConfirmationReceivedFromServer(logOutConfirmation confirmation)
{
    emit dynamic_cast<c_loggedUser *>(myParentConnector)->newLog(QString("Udane wylogowanie. \n"));

    dynamic_cast<c_loggedUser *>(myParentConnector)->clearProperties();
    emit userNotLogged();
}

void c_loggedUserThread::unlockConfirmationReceived(bool canUnlock)
{
    if(canUnlock) {
        emit unlockSession();
        emit logInFinished();
    } else {
        emit logInError(QString("Błędne dane logowania."));
    }
}

void c_loggedUserThread::loggingTimerTimeout()
{
//    loggingTimer->deleteLater();
    if(!dynamic_cast<c_loggedUser *>(myParentConnector)->getIsLogged())
        emit logInError("Upłynął czas oczekiwania na odpiewdź serwera.");
}

void c_loggedUserThread::loggingOutTimerTimeout()
{
    if(dynamic_cast<c_loggedUser *>(myParentConnector)->getIsLogged())
    {
        emit logInError("Upłynął czas oczekiwania na odpiewdź serwera. Nastąpi wylogowanie bez komunikatu do serwera.");
    }

    loggingOutTimer->deleteLater();
}

void c_loggedUserThread::threadStarted()
{
    if(!dynamic_cast<c_loggedUser *>(myParentConnector)->getIsLogged()) emit userAuthorizationDataNeeded();

    loggingTimer = new QTimer();
    connect(loggingTimer, SIGNAL(timeout()), this, SLOT(loggingTimerTimeout()));
    connect(this, SIGNAL(finished()), loggingTimer, SLOT(deleteLater()));
}
