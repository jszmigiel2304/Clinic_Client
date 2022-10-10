#include "c_loggeduser.h"
#include "c_loggeduserthread.h"

c_loggedUser::c_loggedUser()
{


    mThread.reset(new c_loggedUserThread);
    moveToThread(mThread.get());


    setIsLogged(false);
    mThread->setLoggingState(false);

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

w_logsWindow *c_loggedUser::getLogs() const
{
    return logs;
}

void c_loggedUser::setLogs(w_logsWindow *newLogs)
{
    logs = newLogs;
}
