#ifndef C_ACTIONEXECUTIVE_H
#define C_ACTIONEXECUTIVE_H

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


    c_LogsController *getLogsController() const;
    void setLogsController(c_LogsController *newLogsController);

public slots:
    void processData(myStructures::threadData data, qintptr socketDescriptor);

protected:

private:
   QMap<QString, QString> processedDataErrors;

    void processMessage(myStructures::threadData data, qintptr socketDescriptor, QMap<QString, QString> * processedDataErrors = nullptr);
    void processGet(myStructures::threadData data, qintptr socketDescriptor, QMap<QString, QString> * processedDataErrors = nullptr);
    void processReply(myStructures::threadData data, qintptr socketDescriptor, QMap<QString, QString> * processedDataErrors = nullptr);

private slots:
    void processResults(myTypes::JsonContent jsonContent, QList<QMap<QString, QVariant>> results, qintptr socketDescriptor);
    void processErrors(QMap<QString, QString> * receivedErrors);

signals:
    void newLogToFile(QString sender, QString notes, QByteArray data);
    void errors(QMap<QString, myStructures::threadData> errors);
    void resultReady(myTypes::JsonContent jsonContent, QList<QMap<QString, QVariant>> results, qintptr socketDescriptor);

    //from external server
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

    //from local modules
    void connectionSettingsReceived(QMap<QString, QVariant> settings, qintptr socketDescriptor);
};

#endif // C_ACTIONEXECUTIVE_H
