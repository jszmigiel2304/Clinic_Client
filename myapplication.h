#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <QApplication>
#include <QObject>
#include <QTimer>
#include <QEvent>
#include <QTime>

class MyApplication : public QApplication
{
    Q_OBJECT
public:
    explicit MyApplication(int &argc, char **argv);

    QTimer idleTimer;
    QTimer secsTimer;

    QTime currentSessionExpireTime;
    QTime sessionExpireTime;
    quint32 sessionExpireSeconds;

    bool notify(QObject * receiver, QEvent * event) override;

    quint32 getSessionExpireSeconds() const;
    void setSessionExpireSeconds(quint32 newSessionExpireSeconds);

    const QTime &getCurrentSessionExpireTime() const;
    void setCurrentSessionExpireTime(const QTime &newCurrentSessionExpireTime);

    const QTime &getSessionExpireTime() const;
    void setSessionExpireTime(const QTime &newSessionExpireTime);

public slots:
    void idleTimeReceived(quint32 time);
    void resetIDLEtimer();
    void appIDLEdetected();

private slots:
    void secsTimerTimeOut();
    void idleTimerTimeout();

signals:
    void idleDetected();

    void sessionTimeExpireChanged(QTime time);
};

#endif // MYAPPLICATION_H
