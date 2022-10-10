#ifndef C_SENDINGPACKETSCONTROLLER_H
#define C_SENDINGPACKETSCONTROLLER_H

#include <QObject>
#include <QTimer>

class c_SendingPacketsController : public QObject
{
    Q_OBJECT
public:
    explicit c_SendingPacketsController(QObject *parent = nullptr);
    void blockSendingPackets(qint32 time);
    bool getSendingEnabled() const;

private:
    bool sendingEnabled;
    QTimer * timer;

    void Open();
    void Close();
    void setSendingEnabled(bool newSendingEnabled);

private slots:
    void timerTimeout();


signals:

};

#endif // C_SENDINGPACKETSCONTROLLER_H
