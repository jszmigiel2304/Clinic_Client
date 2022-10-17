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

    void processData(myStructures::threadData data);

    c_LogsController *getLogsController() const;
    void setLogsController(c_LogsController *newLogsController);

public slots:

protected:

private:
   QMap<QString, QString> processedDataErrors;

    w_logsWindow *logs;

    void processMessage(myStructures::threadData data, QMap<QString, QString> * processedDataErrors = nullptr);
    void processGet(myStructures::threadData data, QMap<QString, QString> * processedDataErrors = nullptr);
    void processReply(myStructures::threadData data, QMap<QString, QString> * processedDataErrors = nullptr);


private slots:
    void processResults(myTypes::JsonContent jsonContent, QList<QMap<QString, QVariant>> results);
    void processErrors(QMap<QString, QString> * receivedErrors);

signals:
    void newLog(QString log);
    void newLogToFile(QString sender, QString notes, QByteArray data);
    void errors(QMap<QString, myStructures::threadData> errors);
    void resultReady(myTypes::JsonContent jsonContent, QList<QMap<QString, QVariant>> results);
    void getUserIdResultReady(qint32 userID);
    void logInConfirmationReady(myStructures::logInConfirmation confirmation);
    void logOutConfirmationReady(myStructures::logOutConfirmation confirmation);
    void unlockConfirmationReceived(bool canUnlock);
    void errors(QMap<QString, QString> * receivedErrors);

    void getSessionSettingsFromServerResultReady(QMap<QString, QVariant> settings);
    void updateSessionFileConfirmationReceived();
    void updateSessionStatusConfirmationReceived();
    void updateSessionCloseTimeConfirmationReceived();
    void sessionUnlockConfirmationReceived();

    void userLogsReceivedFromServerResultReady(QList<QMap<QString, QVariant>> logs);
    void userEmployeeLogsReceivedFromServerResultReady(QList<QMap<QString, QVariant>> logs);
    void employeeLogsReceivedFromServerResultReady(QList<QMap<QString, QVariant>> logs);
    void employeePropertiesReceivedFromServerResultReady(QMap<QString, QVariant> properties);
    void userPropertiesReceivedFromServerResultReady(QMap<QString, QVariant> properties);
};

#endif // C_ACTIONEXECUTIVE_H
