#include "c_sessioncontroller.h"

c_SessionController::c_SessionController(QObject *parent) : QObject(parent)
{
    loggedUser = nullptr;
    identifier = QUuid("00000000-0000-0000-0000-000000000000");
    opened = false;
    state = myTypes::NOT_DEFINED;
    currentOpenTime = QDateTime::currentDateTime();
    filePath = QString("");
    fileName = QString("");


    mThread.reset(new c_SessionControllerThread);
    moveToThread(mThread.get());

    connect(this, SIGNAL(sessionFileCreated(QUuid, QByteArray)), thread(), SLOT(sendSessionFileToServer(QUuid, QByteArray)), Qt::QueuedConnection);
    connect(this, SIGNAL(sessionReady()), thread(), SLOT(sessionRun()), Qt::DirectConnection);
    connect(this, SIGNAL(sessionStateChanged(QUuid, qint32)), thread(), SLOT(sendSessionStateToServer(QUuid, qint32)), Qt::QueuedConnection);
    connect(thread(), SIGNAL(sessionClosedCorrectly()), this, SLOT(resetSession()), Qt::DirectConnection);
}

void c_SessionController::setOpened(bool newOpened)
{
    opened = newOpened;
}

myTypes::SessionState c_SessionController::getState() const
{
    return state;
}

void c_SessionController::setState(myTypes::SessionState newState)
{
    state = newState;

    emit sessionStateChanged(getIdentifier(), static_cast<qint32>(getState()));
}

c_SessionControllerThread*c_SessionController::thread() const
{
    return mThread.get();
}

void c_SessionController::setThread(std::unique_ptr<c_SessionControllerThread> newThread)
{
    mThread = std::move(newThread);
}

c_loggedUser *c_SessionController::getLoggedUser() const
{
    return loggedUser;
}

void c_SessionController::setLoggedUser(c_loggedUser *newLoggedUser)
{
    loggedUser = newLoggedUser;
}



void c_SessionController::cleanUpThread()
{
    mThread->quit();
}



c_SessionController::~c_SessionController()
{
    if(sessionFile.isOpen())
        sessionFile.close();


    QMetaObject::invokeMethod(this, "cleanUpThread");
    mThread->wait();
}



void c_SessionController::setUpSession(QMap<QString, QVariant> settings)
{
    fileVersionNumber = settings["file_version_number"].toDouble();
    fileControlNumber = quint32( settings["file_control_number"].toUInt() );
    filePath = settings["file_path"].toString();
    fileName = settings["file_name"].toString();
    hostAddress = QHostAddress( settings["ip_address"].toString() );
    startTime = QDateTime::fromString(settings["begin_time"].toString());
    endTime = QDateTime::fromString(settings["end_time"].toString());
    sessionExpireTime = settings["session_expire_time"].toUInt();

    thread()->setSessionExpireSeconds(sessionExpireTime);

    switch(settings["state"].toInt()) {
    case 0x00: { state = myTypes::NOT_DEFINED; break;}
    case 0x01: { state = myTypes::DEFINED_NOT_STARTED; break;}
    case 0x10: { state = myTypes::STARTED; break;}
    case 0x11: { state = myTypes::RESTORED; break;}
    case 0x12: { state = myTypes::RESTARTED; break;}
    case 0x20: { state = myTypes::PAUSED_NOT_SAVED; break;}
    case 0x21: { state = myTypes::PAUSED_SAVED; break;}
    case 0x30: { state = myTypes::CLOSED_NOT_SAVED; break;}
    case 0x31: { state = myTypes::CLOSED_SAVED; break;}
    default: { state = myTypes::ERROR; break;}
    }

    if(createSessionStateFile()) {

    }
    else {
        emit sessionFileCreated(identifier, readEntireSessionFile());
        emit sessionReady();
    }
}

void c_SessionController::setIdentifier(QUuid uuid)
{
    if(uuid.isNull()) identifier = QUuid::createUuid();
    else identifier = uuid;
}

QUuid c_SessionController::getIdentifier() const
{
    return identifier;
}

void c_SessionController::setUpThread()
{
    mThread->setMyParentConnector(this);
    mThread->setId( c_ThreadController::generateThreadId() );
    mThread->setParentIdentifier( QString("c_SessionController") );
    mThread->setNote("Proces kontrolowania sesji.");

    emit threadAssigned(mThread.get());
}

void c_SessionController::resetSession()
{
    identifier = QUuid("00000000-0000-0000-0000-000000000000");
    opened = false;
    state = myTypes::NOT_DEFINED;
    currentOpenTime = QDateTime::currentDateTime();
    filePath = QString("");
    fileName = QString("");


    fileControlNumber = 0;
    fileVersionNumber = 0;

    hostAddress.clear();

    startTime = QDateTime();
    endTime = QDateTime();
    currentOpenTime = QDateTime();
    sessionExpireTime = 0;
    thread()->setSessionExpireSeconds(sessionExpireTime);

    sessionFile.close();
    sessionFile.setFileName(QString(""));
}

void c_SessionController::closeSession()
{
    emit sessionClosed();
}

int c_SessionController::createSessionStateFile(quint32 fileCN, double fileVN, int dsVersion, QString filePathName)
{
    sessionFile.setFileName( filePathName.isEmpty() ? QString(fileName).prepend(filePath) : QString(filePathName) );
    if(sessionFile.open(QIODevice::WriteOnly)) {
        QDataStream ds(&sessionFile);
        ds.setVersion(dsVersion);
        ds << sessionFile.fileName();
        ds << (fileCN == 0 ? fileControlNumber : fileCN);
        ds << (fileVN == 0 ? fileVersionNumber : fileVN);
        ds << identifier;
        ds << loggedUser->getId();
        ds << loggedUser->getName();
        ds << fileData;
        sessionFile.close();
        return 0;
    } else {
        return sessionFile.error();
    }
}

QByteArray c_SessionController::readEntireSessionFile(QString filePathName)
{
    sessionFile.setFileName( filePathName.isEmpty() ? QString(fileName).prepend(filePath) : QString(filePathName) );
    if(sessionFile.open(QIODevice::ReadOnly)) {
        QByteArray data = sessionFile.readAll();
        sessionFile.close();
        return data;
    } else {
        return QByteArray();
    }

}



bool c_SessionController::isOpened()
{
    if (opened && !identifier.isNull()) return true;
    return false;
}

bool c_SessionController::isValid()
{
    if(identifier.isNull()) return false;
    if(fileControlNumber == 0) return false;
    if(fileVersionNumber == 0) return false;
    if(fileName.isEmpty()) return false;
    if(filePath.isEmpty()) return false;
    if(startTime.isNull()) return false;
    if(static_cast<quint8>(state) == 0xff) return false;
    if(loggedUser == nullptr) return false;

    return true;
}

int c_SessionController::addNotSendedNoteToFile(QString filePathName)
{
    sessionFile.setFileName( filePathName.isEmpty() ? QString(fileName).prepend(filePath) : QString(filePathName) );
    if(sessionFile.open(QIODevice::WriteOnly)) {
        sessionFile.close();
        return 0;
    } else {
        return sessionFile.error();
    }
}




