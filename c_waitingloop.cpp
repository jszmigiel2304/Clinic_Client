#include "c_waitingloop.h"



c_waitingLoop::waitingCondition::waitingCondition(QObject *parent)
    : QObject(parent)
{
    id = QRandomGenerator::global()->bounded(quint32(100000));
}

void c_waitingLoop::waitingCondition::conditionFulfilled()
{
    emit this->removeCondition(this->id);
}

c_waitingLoop::c_waitingLoop::c_waitingLoop(QObject *parent)
    : QEventLoop(parent)
{
    connect(this, SIGNAL(exitLoop(int)), this, SLOT(exit(int)));

    connect(&exitLoopTimer, SIGNAL(timeout()), this, SLOT(stopLoop()));

    connect(&checkConditionsTimer, SIGNAL(timeout()), this, SLOT(checkConditions()));
}

c_waitingLoop::c_waitingLoop::~c_waitingLoop()
{
    while(waitingConditions.size() > 0) {
        waitingConditions.takeFirst()->deleteLater();
    }
}

void c_waitingLoop::c_waitingLoop::setExpireTime(quint32 time)
{
    this->exitLoopTime = time;
}


c_waitingLoop::waitingCondition *c_waitingLoop::c_waitingLoop::newCondition()
{
    waitingCondition * condition = new waitingCondition();
    connect(condition, SIGNAL(removeCondition(int)), this, SLOT(removeCondition(int)));
    this->waitingConditions.append(condition);

    return condition;
}

void c_waitingLoop::c_waitingLoop::removeCondition(int id)
{
    int i;
    for(i=0; i<waitingConditions.size(); i++) {
        if(waitingConditions[i]->getId() == id) {
            break;
        }
    }
    waitingConditions.removeAt(i);

}

void c_waitingLoop::c_waitingLoop::startExec()
{
    emit loopStarted( QString("Pobieram dane. Liczba potrzebnych danych: %1").arg(waitingConditions.size()) );
    checkConditionsTimer.start(1000);
    exitLoopTimer.start(exitLoopTime);
    this->exec();
}

void c_waitingLoop::c_waitingLoop::stopLoop()
{
    emit exitLoop( waitingConditions.size() );
}

void c_waitingLoop::c_waitingLoop::checkConditions()
{
    checkConditionsTimer.stop();

    if(waitingConditions.size() == 0)
        emit exitLoop(0);


    checkConditionsTimer.start();
}


namespace c_waitingLoop {
int waitingCondition::getId() const
{
    return id;
}

void waitingCondition::setId(int newId)
{
    id = newId;
}

}