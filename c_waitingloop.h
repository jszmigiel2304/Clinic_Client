#ifndef C_WAITINGLOOP_H
#define C_WAITINGLOOP_H

#include "c_mystructures.h"

#include <QEventLoop>
#include <QObject>
#include <QTimer>
#include <QRandomGenerator>
#include <QList>

namespace c_waitingLoop {

    class waitingCondition : public QObject
    {
        Q_OBJECT

    public:
        explicit waitingCondition(QObject *parent = nullptr);

        int getId() const;
        void setId(int newId);

    public slots:
        void conditionFulfilled();

    private:
        int id;

    signals:
        void removeCondition(int id);
    };



    class c_waitingLoop : public QEventLoop
    {
        Q_OBJECT
    public:
        explicit c_waitingLoop(QObject *parent = nullptr);
        ~c_waitingLoop();
        void setExpireTime(quint32 time);

        waitingCondition * newCondition();

        int getConditionsNumber();

    public slots:
        void removeCondition(int id);
        void startExec();

    private:
        QList<waitingCondition *> waitingConditions;
        QTimer exitLoopTimer;
        quint32 exitLoopTime;

    private slots:
        void stopLoop();

    signals:
        void exitLoop(int exitCode);
        void loopStarted(QString text);
    };
}

#endif // C_WAITINGLOOP_H
