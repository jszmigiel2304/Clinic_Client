#ifndef C_SESSIONCONTROLLER_H
#define C_SESSIONCONTROLLER_H

#include "c_mystructures.h"
#include "c_loggeduser.h"
#include "c_connectiontoservercontroller.h"

#include <QObject>
#include <QWidget>
#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <QUuid>
#include <QDateTime>
#include <QFile>
#include <QList>

class c_SessionController : public QObject
{
    Q_OBJECT
public:
    static c_SessionController *Instance();
    ~ c_SessionController();

    void setUpSession(QByteArray settings);
    void resetSession();
    quint8 getSessionSettingsFromServer(quint8 userID, QString name, QString password);
    bool isOpened();

    c_loggedUser * user;    


    quint8 getLastErrorCode() const;
    void setLastErrorCode(quint8 newLastErrorCode);

private:
    explicit c_SessionController(QObject *parent = nullptr);
    void setOpened(bool newOpened);
    quint8 lastErrorCode;

    bool opened;
    QUuid identifier;
    quint32 fileControlNumber;
    quint8 fileVersionNumber;
    QString fileName;
    QString filePath;

    QDateTime startTime;
    QDateTime currentOpenTime;

    SessionState state;

    QFile sessionFile;



};

#endif // C_SESSIONCONTROLLER_H
