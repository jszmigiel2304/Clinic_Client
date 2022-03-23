#include "c_sessioncontroller.h"

c_SessionController::c_SessionController(QObject *parent) : QObject(parent)
{
    user = nullptr;
    identifier = QUuid("00000000-0000-0000-0000-000000000000");
    currentOpenTime = QDateTime::currentDateTime();
    filePath = "sessions//";

}

void c_SessionController::setOpened(bool newOpened)
{
    opened = newOpened;
}

quint8 c_SessionController::getLastErrorCode() const
{
    return lastErrorCode;
}

void c_SessionController::setLastErrorCode(quint8 newLastErrorCode)
{
    lastErrorCode = newLastErrorCode;
}

c_SessionController *c_SessionController::Instance()
{
    static c_SessionController * instance = nullptr;
    if ( instance == nullptr ) {
        instance = new c_SessionController();
    }

    return instance;
}

c_SessionController::~c_SessionController()
{
    if(sessionFile.isOpen())
        sessionFile.close();
}

void c_SessionController::setUpSession(QByteArray settings)
{
    QDataStream ds(settings);

    quint32 uuidSize;
    char * uuidBytes;
    quint8 sessionState;

    ds >> fileControlNumber >> fileVersionNumber;

    if (fileVersionNumber >= 1 || fileVersionNumber < 2)
        ds.setVersion(QDataStream::Qt_6_2);

    ds >> uuidSize;
    ds.readBytes(uuidBytes, uuidSize);
    identifier = QUuid::fromRfc4122(uuidBytes);

     ds >> fileName >> startTime >> sessionState;

    state = (SessionState)sessionState;


    sessionFile.setFileName( filePath.append(fileName) );

    QDataStream fs(&sessionFile);
}

void c_SessionController::resetSession()
{
    user = nullptr;
    identifier = QUuid("00000000-0000-0000-0000-000000000000");
    currentOpenTime = QDateTime::currentDateTime();
    filePath = "sessions//";
}

quint8 c_SessionController::getSessionSettingsFromServer(quint8 userID, QString name, QString password)
{
    lastErrorCode = 0x00;
    if (userID == 0 || name.isEmpty() || password.isEmpty())    { lastErrorCode = 0x10; return lastErrorCode; }

    return lastErrorCode;
}

bool c_SessionController::isOpened()
{
    if (opened && !identifier.isNull()) return true;
    return false;
}
