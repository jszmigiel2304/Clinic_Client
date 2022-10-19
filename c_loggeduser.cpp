#include "c_loggeduser.h"
#include "c_loggeduserthread.h"

c_loggedUser::c_loggedUser()
{    
    qRegisterMetaType<myStructures::myLog>("myLog");

    mThread.reset(new c_loggedUserThread);
    moveToThread(mThread.get());


    setIsLogged(false);
    mThread->setLoggingState(false);

    this->setEmployee(new c_employee(true));

    connect(this, SIGNAL(logOutUser()), this->thread(), SLOT(logOut()), Qt::DirectConnection);
}

void c_loggedUser::cleanUpThread()
{
    mThread->quit();
}




c_loggedUser::~c_loggedUser()
{

        QMetaObject::invokeMethod(this, "cleanUpThread");
        mThread->wait();

}

QAuthenticator c_loggedUser::getAuthenticator()
{
    QAuthenticator authData;

    authData.setUser(getName());
    authData.setPassword(getPassword());

    return authData;
}

void c_loggedUser::setUpThread()
{
    mThread->setMyParentConnector(this);
    mThread->setId( c_ThreadController::generateThreadId() );
    mThread->setParentIdentifier( QString("c_loggedUser") );
    mThread->setNote("Proces zarządzania zalogowanym użytkownikiem.");

    emit threadAssigned(mThread.get());
}


c_loggedUserThread*c_loggedUser::thread() const
{
    return mThread.get();
}

void c_loggedUser::setThread(std::unique_ptr<c_loggedUserThread> newThread)
{
    mThread = std::move(newThread);
}

void c_loggedUser::forceLogOut()
{
    if(getIsLogged())
        emit logOutUser();
}

void c_loggedUser::setProperties(QMap<QString, QVariant> userInfo)
{
       setId( userInfo["id"].toUInt() );
       setName( userInfo["name"].toString() );
       setEmail( userInfo["email"].toString() );
       setVerified( userInfo["verified"].toBool() );
       setBlocked( userInfo["blocked"].toBool() );
       setCreate_date( QDateTime::fromString( userInfo["create_date"].toString() ) );
       setVerify_date( QDateTime::fromString( userInfo["verify_date"].toString() ) );
       setBlock_date( QDateTime::fromString( userInfo["blocked_date"].toString() ) );
       setPhoto( QByteArray( userInfo["photo"].toByteArray() ) );
        QMetaEnum metaEnum = QMetaEnum::fromType<m_loggedUser::UserRole>();
       setRole( static_cast<m_loggedUser::UserRole>( metaEnum.keyToValue(userInfo["role"].toString().toStdString().c_str() ) ) );
       setIsLogged( userInfo["logged"].toBool() );

       emit propertiesSaved();
       emit passProperties(userInfo);
}

void c_loggedUser::setDbLogs(QList<myStructures::myLog> &newDbLogs)
{
    dbLogs = newDbLogs;
}

void c_loggedUser::setDbLogs(QList<QMap<QString, QVariant> > logs)
{
    for(int i = 0; i< logs.size(); i++) {
        myStructures::myLog log = myStructures::myLog::fromMap(logs[i]);
//        log.ip_address = QHostAddress(logs[i]["ip_address"].toString());
//        log.time = QDateTime::fromString(logs[i]["log_time"].toString(), Qt::ISODateWithMs);
//        log.log_text = logs[i]["activity"].toString();
        this->dbLogs.append(log);
    }

    emit logsSaved();
    emit passLogs(QList<myStructures::myLog>(this->dbLogs));
}

QMap<QString, QVariant> c_loggedUser::getUserProperties()
{
    QMap<QString, QVariant> map;

    //QMetaEnum metaEnum = QMetaEnum::fromType<m_loggedUser::UserRole>();

    map["id"] = this->getId();
    map["name"] = this->getName();    
    map["role"] = this->getRoleString();
    //map["role"] = metaEnum.valueToKey( this->getRole() );
    map["create_date"] = this->getCreate_date().toString();
    map["email"] = this->getEmail();
    map["is_blocked"] = this->getBlocked();
    map["is_verified"] = this->getVerified();
    map["blocked_date"] = this->getBlock_date().toString();
    map["verified_date"] = this->getVerify_date().toString();
    map["is_logged"] = this->getIsLogged();
    map["photo"] = this->getPhoto();


    return map;
}

QMap<QString, QVariant> c_loggedUser::getEmployeeProperties()
{
    return this->getEmployee()->getProperties(true, true);
}

QList<myStructures::myLog> * c_loggedUser::getDbLogs()
{
    return &dbLogs;
}


w_logsWindow *c_loggedUser::getLogs() const
{
    return logs;
}

void c_loggedUser::setLogs(w_logsWindow *newLogs)
{
    logs = newLogs;
}
