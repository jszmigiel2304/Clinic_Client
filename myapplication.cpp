#include "myapplication.h"

MyApplication::MyApplication(int &argc, char **argv) : QApplication(argc, argv)
{
//    idleTimer.setInterval(100000);
    sessionExpireSeconds = -1;

    connect(&idleTimer, SIGNAL(timeout()), this, SLOT(idleTimerTimeout()));


    sessionExpireTime = QTime(0,0,0);
    currentSessionExpireTime = QTime(0,0,0);

    connect(&secsTimer, SIGNAL(timeout()), this, SLOT(secsTimerTimeOut()));
}

bool MyApplication::notify(QObject *receiver, QEvent *event)
{
    if (event->type() == QEvent::MouseMove || event->type() == QEvent::KeyPress) {
//         idleTimer.stop(); // reset timer
//         idleTimer.start();
        if(currentSessionExpireTime.second() != 0 || currentSessionExpireTime.minute() != 0 || currentSessionExpireTime.hour() != 0)
            resetIDLEtimer();
    }
    return QApplication::notify(receiver, event);
}

void MyApplication::idleTimeReceived(quint32 time)
{
    if(time > 0) {
        this->setSessionExpireSeconds(time);
        idleTimer.start(sessionExpireSeconds*1000);
        secsTimer.start(1000);
        emit sessionTimeExpireChanged(currentSessionExpireTime);
    } else {
        this->setSessionExpireSeconds(time);
        idleTimer.stop();
        secsTimer.stop();
        emit sessionTimeExpireChanged(currentSessionExpireTime);
    }
}



quint32 MyApplication::getSessionExpireSeconds() const
{
    return sessionExpireSeconds;
}

void MyApplication::setSessionExpireSeconds(quint32 newSessionExpireSeconds)
{
    sessionExpireSeconds = newSessionExpireSeconds;


        quint32 h,m,s, r;
        h = sessionExpireSeconds / 3600;
        r = sessionExpireSeconds % 3600;
        m = r / 60;
        s = r % 60;

        setSessionExpireTime( QTime(h,m,s) );
        setCurrentSessionExpireTime( QTime(h,m,s) );
}


void MyApplication::resetIDLEtimer()
{
    currentSessionExpireTime = sessionExpireTime;
    idleTimer.stop(); // reset timer
    secsTimer.stop();
    idleTimer.start(sessionExpireSeconds*1000);
    secsTimer.start(1000);

    emit sessionTimeExpireChanged(currentSessionExpireTime);
}

void MyApplication::appIDLEdetected()
{
    emit idleDetected();
    //resetIDLEtimer();
}




const QTime &MyApplication::getSessionExpireTime() const
{
    return sessionExpireTime;
}

void MyApplication::setSessionExpireTime(const QTime &newSessionExpireTime)
{
    sessionExpireTime = newSessionExpireTime;
}

const QTime &MyApplication::getCurrentSessionExpireTime() const
{
    return currentSessionExpireTime;
}

void MyApplication::setCurrentSessionExpireTime(const QTime &newCurrentSessionExpireTime)
{
    currentSessionExpireTime = newCurrentSessionExpireTime;
}



void MyApplication::secsTimerTimeOut()
{
    if(currentSessionExpireTime.hour() == 0 && currentSessionExpireTime.minute() == 0 && currentSessionExpireTime.second() == 0 ) {
        setCurrentSessionExpireTime(currentSessionExpireTime);
        secsTimer.stop();
        appIDLEdetected();
    } else {
        currentSessionExpireTime = currentSessionExpireTime.addSecs(-1);
        setCurrentSessionExpireTime(currentSessionExpireTime);
        secsTimer.stop();
        secsTimer.start(1000);
    }
    emit sessionTimeExpireChanged(currentSessionExpireTime);
}

void MyApplication::idleTimerTimeout()
{

}



