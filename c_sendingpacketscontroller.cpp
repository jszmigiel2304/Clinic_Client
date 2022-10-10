#include "c_sendingpacketscontroller.h"

c_SendingPacketsController::c_SendingPacketsController(QObject *parent)
    : QObject{parent}
{
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTimeout));

    Open();
}

bool c_SendingPacketsController::getSendingEnabled() const
{
    return sendingEnabled;
}

void c_SendingPacketsController::setSendingEnabled(bool newSendingEnabled)
{
    sendingEnabled = newSendingEnabled;
}

void c_SendingPacketsController::blockSendingPackets(qint32 time)
{
    Close();
    timer->start(time);
}

void c_SendingPacketsController::Open()
{
    setSendingEnabled(true);
}

void c_SendingPacketsController::Close()
{
    setSendingEnabled(false);
}

void c_SendingPacketsController::timerTimeout()
{
    timer->stop();
    Open();
}
