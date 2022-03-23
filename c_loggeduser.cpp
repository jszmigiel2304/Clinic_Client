#include "c_loggeduser.h"

c_loggedUser::c_loggedUser()
{
    setIsLogged(false);
}

c_loggedUser *c_loggedUser::Instance()
{
    static c_loggedUser * instance = nullptr;
    if ( instance == nullptr ) {
        instance = new c_loggedUser();
    }
    return instance;
}

c_loggedUser::~c_loggedUser()
{

}

QAuthenticator c_loggedUser::getAuthenticator()
{
    QAuthenticator authData;

    authData.setUser(getName());
    authData.setPassword(getPassword());

    return authData;
}

void c_loggedUser::execute(QObject *sender, quint32 request_type, QByteArray request)
{

}
