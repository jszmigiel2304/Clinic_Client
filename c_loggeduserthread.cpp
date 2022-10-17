#include "c_loggeduserthread.h"
#include "c_loggeduser.h"
#include "c_actionexecutive.h"

c_loggedUserThread::c_loggedUserThread(qint32 id, QString parentIdent, QString note, QObject *parent)
    : MyThread{id, parentIdent, note, parent}
{
    connect(this, SIGNAL(logInToServer(qint32, QString, QString)), this, SLOT(logIn(qint32, QString, QString)), Qt::DirectConnection);
    connect(this, SIGNAL(started()), this, SLOT(threadStarted()));

    loggingOutState = false;
    loggingState = false;
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

void c_loggedUserThread::processData(myStructures::threadData data)
{
    if( this->getId() == data.thread_id && (data.thread_dest == myTypes::CLINIC_LOGGED_USER_CONTROLLER  || data.thread_dest == myTypes::CLINIC_ERROR_CONTROLLER) )
    {
        c_actionExecutive *executive = new c_actionExecutive();
        //executive->moveToThread(mThread.get());
        connect( executive, SIGNAL(newLog(QString)), dynamic_cast<c_loggedUser *>(myParentConnector)->getLogs(), SLOT(addLog(QString)) );

        connect( executive, SIGNAL(getUserIdResultReady(qint32)), this, SLOT(userIdReceivedFromServer(qint32)) );
        connect( executive, SIGNAL(logInConfirmationReady(myStructures::logInConfirmation)),this, SLOT(logInConfirmationReceivedFromServer(myStructures::logInConfirmation)) );
        connect( executive, SIGNAL(logOutConfirmationReady(myStructures::logOutConfirmation)), this, SLOT(logOutConfirmationReceivedFromServer(myStructures::logOutConfirmation)) );
        connect( executive, SIGNAL(unlockConfirmationReceived(bool)), this, SLOT(unlockConfirmationReceived(bool)) );
        connect( executive, SIGNAL(userLogsReceivedFromServerResultReady(QList<QMap<QString, QVariant>>)), this, SLOT(userLogsReceivedFromServer(QList<QMap<QString, QVariant>>)) );
        connect( executive, SIGNAL(userEmployeeLogsReceivedFromServerResultReady(QList<QMap<QString, QVariant>>)), this, SLOT(userEmployeeLogsReceivedFromServer(QList<QMap<QString, QVariant>>)) );
        connect( executive, SIGNAL(employeeLogsReceivedFromServerResultReady(QList<QMap<QString, QVariant>>)), this, SLOT(emlpoyeeLogsReceivedFromServer(QList<QMap<QString, QVariant>>)) );
        connect( executive, SIGNAL(employeePropertiesReceivedFromServerResultReady(QMap<QString, QVariant>)), this, SLOT(employeePropertiesReceivedFromServer(QMap<QString, QVariant>)) );
        connect( executive, SIGNAL(userPropertiesReceivedFromServerResultReady(QMap<QString, QVariant>)), this, SLOT(userPropertiesReceivedFromServer(QMap<QString, QVariant>)) );

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
    QString pwd = dynamic_cast<c_loggedUser *>(myParentConnector)->getEncryptedPassword(false, true);
    c_Parser parser;
    QPair<QByteArray, QByteArray> pair = parser.prepareGetUserIdPacket(dynamic_cast<c_loggedUser *>(myParentConnector)->getName(),
                                                                       dynamic_cast<c_loggedUser *>(myParentConnector)->getEncryptedPassword(false, true),
                                                                       getId());

    myStructures::packet packet;
    packet.md5_hash = pair.first;
    packet.packet_to_send = pair.second;
    packet.wait_for_reply = true;


    emit logInProcessing(QString("Pobieranie danych użytkownika z serwera..."));
    emit sendToServer(packet);

    loggingTimer->start(5000);
}

void c_loggedUserThread::unlockOnIdle(QString userName, QString userPassword)
{
    c_Parser parser;

    QByteArray encryptedPassword;

    QByteArray encryptionSeed;
    QDataStream encryptionStream(&encryptionSeed, QIODevice::ReadWrite);
    encryptionStream.setVersion(QDataStream::Qt_6_0);
    encryptionStream << userPassword;

    QCryptographicHash hasher(QCryptographicHash::Md5);
    hasher.addData(encryptionSeed);
    encryptedPassword = hasher.result();

    QPair<QByteArray, QByteArray> pair = parser.unlockOnIdle(userName, encryptedPassword.toHex(), getId());

    myStructures::packet packet;
    packet.md5_hash = pair.first;
    packet.packet_to_send = pair.second;
    packet.wait_for_reply = true;

    emit logInProcessing(QString("Pobieranie danych użytkownika z serwera..."));
    emit sendToServer(packet);
}

void c_loggedUserThread::getProperties(QMap<QString, QVariant> *userProperties, QMap<QString, QVariant> *employeeProperties, QList<myStructures::myLog> * Logs)
{
    if(dynamic_cast<c_loggedUser *>(myParentConnector)->getId() == 0 || dynamic_cast<c_loggedUser *>(myParentConnector)->getName().isEmpty() || dynamic_cast<c_loggedUser *>(myParentConnector)->getPassword().isEmpty()) {
        this->getUserPropertiesFromServer(dynamic_cast<c_loggedUser *>(myParentConnector)->getName(), dynamic_cast<c_loggedUser *>(myParentConnector)->getPassword());
    } else {
        (*userProperties) = dynamic_cast<c_loggedUser *>(myParentConnector)->getUserProperties();
    }

    if(dynamic_cast<c_loggedUser *>(myParentConnector)->getEmployee()->getId() == 0 || dynamic_cast<c_loggedUser *>(myParentConnector)->getEmployee()->getName().isEmpty()) {
        this->getEmployeePropertiesFromServer(dynamic_cast<c_loggedUser *>(myParentConnector)->getName(), dynamic_cast<c_loggedUser *>(myParentConnector)->getPassword());
    } else {
        (*employeeProperties) = dynamic_cast<c_loggedUser *>(myParentConnector)->getEmployee()->getProperties();
    }

    if(dynamic_cast<c_loggedUser *>(myParentConnector)->getDbLogs()->isEmpty()) {
        this->getLogsFromServer(dynamic_cast<c_loggedUser *>(myParentConnector)->getId(),
                                dynamic_cast<c_loggedUser *>(myParentConnector)->getName(),
                                dynamic_cast<c_loggedUser *>(myParentConnector)->getPassword());
    } else {
        (*Logs) = QList<myStructures::myLog>(*dynamic_cast<c_loggedUser *>(myParentConnector)->getDbLogs());
    }
}

void c_loggedUserThread::getUserPropertiesFromServer(QString name, QString password)
{
    c_Parser parser;
    QPair<QByteArray, QByteArray> pair = parser.prepareGetUserPropertiesPacket(name, password, getId());

    myStructures::packet packet;
    packet.md5_hash = pair.first;
    packet.packet_to_send = pair.second;
    packet.wait_for_reply = true;

    emit sendToServer(packet);
}

void c_loggedUserThread::getEmployeePropertiesFromServer(QString name, QString password)
{
    c_Parser parser;
    QPair<QByteArray, QByteArray> pair = parser.prepareGetEmployeePropertiesPacket(name, password, getId());

    myStructures::packet packet;
    packet.md5_hash = pair.first;
    packet.packet_to_send = pair.second;
    packet.wait_for_reply = true;

    emit sendToServer(packet);
}

void c_loggedUserThread::getLogsFromServer(qint32 id, QString name, QString password)
{
    c_Parser parser;
    QPair<QByteArray, QByteArray> pair = parser.prepareGetLogsPacket(id, name, password, getId());

    myStructures::packet packet;
    packet.md5_hash = pair.first;
    packet.packet_to_send = pair.second;
    packet.wait_for_reply = true;

    emit sendToServer(packet);
}


bool c_loggedUserThread::getLoggingOutState() const
{
    return loggingOutState;
}

void c_loggedUserThread::setLoggingOutState(bool newLoggingOutState)
{
    loggingOutState = newLoggingOutState;
}

void c_loggedUserThread::logIn(qint32 id, QString name, QString password)
{
    loggingState = true;
    loggingTimer->stop();

    c_Parser parser;
    QPair<QByteArray, QByteArray> pair = parser.prepareLogInPacket(id, name, password, getId());

    myStructures::packet packet;
    packet.md5_hash = pair.first;
    packet.packet_to_send = pair.second;
    packet.wait_for_reply = true;

    emit logInProcessing(QString("Logowanie..."));
    emit sendToServer(packet);

    loggingTimer->start(5000);
}

void c_loggedUserThread::logOut(qint32 id, QString name, QString password)
{    
    loggingOutState = true;
    emit aboutToLogOut();

    c_Parser parser;
    qint32 idc;
    QString namec, passwordc;
    if(id <= 0) idc = dynamic_cast<c_loggedUser *>(myParentConnector)->getId(); else idc = id;
    if(namec.isEmpty()) namec = dynamic_cast<c_loggedUser *>(myParentConnector)->getName(); else namec = name;
    if(password.isEmpty()) passwordc = dynamic_cast<c_loggedUser *>(myParentConnector)->getEncryptedPassword(false, true); else passwordc = password;


    QPair<QByteArray, QByteArray> pair = parser.prepareLogOutPacket(idc, namec, passwordc, getId());

    myStructures::packet packet;
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
        loggingState = false;
        emit logInError(QString("Błędne dane logowania."));
    } else {
        dynamic_cast<c_loggedUser *>(myParentConnector)->setId(userID);
        if(!dynamic_cast<c_loggedUser *>(myParentConnector)->getIsLogged()) {
            emit logInToServer(dynamic_cast<c_loggedUser *>(myParentConnector)->getId(),
                               dynamic_cast<c_loggedUser *>(myParentConnector)->getName(),
                               dynamic_cast<c_loggedUser *>(myParentConnector)->getEncryptedPassword(false, true));
        } else {
            loggingTimer->stop();
            emit logInFinished();
            loggingState = false;
        }
    }
}

void c_loggedUserThread::logInConfirmationReceivedFromServer(myStructures::logInConfirmation confirmation)
{
    loggingTimer->stop();

    dynamic_cast<c_loggedUser *>(myParentConnector)->setId(confirmation.id);
    dynamic_cast<c_loggedUser *>(myParentConnector)->setName(confirmation.name);
    dynamic_cast<c_loggedUser *>(myParentConnector)->setLoginTimeStamp(confirmation.logInTime);
    dynamic_cast<c_loggedUser *>(myParentConnector)->setEmail(confirmation.email);
    dynamic_cast<c_loggedUser *>(myParentConnector)->setVerified(confirmation.verified);
    dynamic_cast<c_loggedUser *>(myParentConnector)->setBlocked(confirmation.blocked);
    dynamic_cast<c_loggedUser *>(myParentConnector)->setCreate_date(confirmation.create_date);
    dynamic_cast<c_loggedUser *>(myParentConnector)->setVerify_date(confirmation.verify_date);
    dynamic_cast<c_loggedUser *>(myParentConnector)->setBlock_date(confirmation.block_date);
    dynamic_cast<c_loggedUser *>(myParentConnector)->setPhoto(confirmation.photo);

    QMetaEnum metaEnum = QMetaEnum::fromType<m_loggedUser::UserRole>();
    dynamic_cast<c_loggedUser *>(myParentConnector)->setRole( static_cast<m_loggedUser::UserRole>( metaEnum.keyToValue(confirmation.role.toStdString().c_str() ) ) );

    dynamic_cast<c_loggedUser *>(myParentConnector)->setIsLogged(true);

    emit userLogged(dynamic_cast<c_loggedUser *>(myParentConnector)->getName(), dynamic_cast<c_loggedUser *>(myParentConnector)->getRoleString());
    emit logInFinished();


    loggingState = false;
    emit dynamic_cast<c_loggedUser *>(myParentConnector)->newLog(QString("Udane logowanie. \n"));
}

void c_loggedUserThread::logOutConfirmationReceivedFromServer(myStructures::logOutConfirmation confirmation)
{
    emit dynamic_cast<c_loggedUser *>(myParentConnector)->newLog(QString("Udane wylogowanie. \n"));

    dynamic_cast<c_loggedUser *>(myParentConnector)->clearProperties();
    emit userNotLogged();
    loggingOutState = false;
}

void c_loggedUserThread::userPropertiesReceivedFromServer(QMap<QString, QVariant> properties)
{
    dynamic_cast<c_loggedUser *>(myParentConnector)->setProperties(properties);
}

void c_loggedUserThread::employeePropertiesReceivedFromServer(QMap<QString, QVariant> properties)
{
    dynamic_cast<c_loggedUser *>(myParentConnector)->getEmployee()->setProperties(properties);
}

void c_loggedUserThread::userLogsReceivedFromServer(QList<QMap<QString, QVariant>> logs)
{
    dynamic_cast<c_loggedUser *>(myParentConnector)->setDbLogs(logs);
//    for(int i = 0; i< logs.size(); i++) {
//        myStructures::myLog log;
//        log.ip_address = QHostAddress(logs[i]["ip_address"].toString());
//        log.time = QDateTime::fromString(logs[i]["log_time"].toString());
//        log.log_text = logs[i]["log"].toString();
//        dynamic_cast<c_loggedUser *>(myParentConnector)->getDbLogs()->append(log);
//    }

//    emit logsDbSaved();
}

void c_loggedUserThread::userEmployeeLogsReceivedFromServer(QList<QMap<QString, QVariant>> logs)
{
    dynamic_cast<c_loggedUser *>(myParentConnector)->setDbLogs(logs);

//    for(int i = 0; i< logs.size(); i++) {
//        myStructures::myLog log;
//        log.ip_address = QHostAddress(logs[i]["ip_address"].toString());
//        log.time = QDateTime::fromString(logs[i]["log_time"].toString());
//        log.log_text = logs[i]["log"].toString();
//        dynamic_cast<c_loggedUser *>(myParentConnector)->getDbLogs()->append(log);
//    }

//    emit logsDbSaved();
}

void c_loggedUserThread::emlpoyeeLogsReceivedFromServer(QList<QMap<QString, QVariant>> logs)
{
    dynamic_cast<c_loggedUser *>(myParentConnector)->setDbLogs(logs);
//    for(int i = 0; i< logs.size(); i++) {
//        myStructures::myLog log;
//        log.ip_address = QHostAddress(logs[i]["ip_address"].toString());
//        log.time = QDateTime::fromString(logs[i]["log_time"].toString());
//        log.log_text = logs[i]["log"].toString();
//        dynamic_cast<c_loggedUser *>(myParentConnector)->getDbLogs()->append(log);
//    }

//    emit logsDbSaved();
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
    if(!dynamic_cast<c_loggedUser *>(myParentConnector)->getIsLogged()) {
        emit logInError("Upłynął czas oczekiwania na odpiewdź serwera.");
        loggingState = false;
    }
}

void c_loggedUserThread::loggingOutTimerTimeout()
{
    if(dynamic_cast<c_loggedUser *>(myParentConnector)->getIsLogged())
    {
        emit logInError("Upłynął czas oczekiwania na odpiewdź serwera. Nastąpi wylogowanie bez komunikatu do serwera.");
        loggingOutState = false;
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
