#include "c_threadcontroller.h"

c_ThreadController *c_ThreadController::Instance()
{
    static c_ThreadController * instance = nullptr;
    if ( instance == nullptr ) {
        instance = new c_ThreadController();
    }

    return instance;
}

c_ThreadController::~c_ThreadController()
{

}

void c_ThreadController::AddReceivedData(threadData data)
{
    dataReceivedFromServer.push_back(data);
}

c_ThreadController::c_ThreadController(QObject *parent) : QObject(parent)
{

}
