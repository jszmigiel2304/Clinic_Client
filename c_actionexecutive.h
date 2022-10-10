#ifndef C_ACTIONEXECUTIVE_H
#define C_ACTIONEXECUTIVE_H

#include "w_logswindow.h"
#include "c_mystructures.h"
#include "c_sessioncontroller.h"
#include "c_parser.h"
#include "c_logscontroller.h"

#include <QObject>
#include <QAuthenticator>
#include <QMap>
#include <QList>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>

class c_actionExecutive : public QObject
{
    Q_OBJECT
public:
    explicit c_actionExecutive(QObject *parent = nullptr);
    ~c_actionExecutive();


    w_logsWindow *getLogs() const;
    void setLogs(w_logsWindow *value);

    void processData(threadData data);

    c_LogsController *getLogsController() const;
    void setLogsController(c_LogsController *newLogsController);

public slots:

protected:

private:
   QMap<QString, QString> processedDataErrors;

    w_logsWindow *logs;

    void processMessage(threadData data, QMap<QString, QString> * processedDataErrors = nullptr);
    void processGet(threadData data, QMap<QString, QString> * processedDataErrors = nullptr);
    void processReply(threadData data, QMap<QString, QString> * processedDataErrors = nullptr);


private slots:
    void processResults(JsonContent jsonContent, QList<QMap<QString, QVariant>> results);
    void processErrors(QMap<QString, QString> * receivedErrors);

signals:
    void newLog(QString log);
    void newLogToFile(QString sender, QString notes, QByteArray data);
    void errors(QMap<QString, threadData> errors);
    void resultReady(JsonContent jsonContent, QList<QMap<QString, QVariant>> results);
    void getUserIdResultReady(qint32 userID);
    void logInConfirmationReady(logInConfirmation confirmation);
    void logOutConfirmationReady(logOutConfirmation confirmation);
    void unlockConfirmationReceived(bool canUnlock);
    void errors(QMap<QString, QString> * receivedErrors);

    void getSessionSettingsFromServerResultReady(QMap<QString, QVariant> settings);
    void updateSessionFileConfirmationReceived();
    void updateSessionStatusConfirmationReceived();
    void updateSessionCloseTimeConfirmationReceived();
    void sessionUnlockConfirmationReceived();
};

#endif // C_ACTIONEXECUTIVE_H
