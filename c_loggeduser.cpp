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
    connect(this, SIGNAL(getLogsSignal(qint32, QString, QString)), this->thread(), SLOT(getLogs(qint32, QString, QString)) );
    connect(this->thread(), SIGNAL(logsReceivedFromServerSignal(QList<myStructures::myLog>, QList<myStructures::myLog>)), this, SLOT(logsReceivedFromServer(QList<myStructures::myLog>, QList<myStructures::myLog>)));
}

void c_loggedUser::cleanUpThread()
{
    mThread->quit();
}

void c_loggedUser::logsReceivedFromServer(QList<myStructures::myLog> dbLogs)
{

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

void c_loggedUser::setDbLogs(const QList<myStructures::myLog> &newDbLogs)
{
    dbLogs = newDbLogs;
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

QStringList c_loggedUser::getDbLogs()
{
    QStringList list;
    return list;
}


w_logsWindow *c_loggedUser::getLogs() const
{
    return logs;
}

void c_loggedUser::setLogs(w_logsWindow *newLogs)
{
    logs = newLogs;
}
