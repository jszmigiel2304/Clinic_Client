#include "c_actionexecutive.h"

c_actionExecutive::c_actionExecutive(QObject *parent)
    : QObject{parent}
{
    connect( this, SIGNAL(resultReady(myTypes::JsonContent, QList<QMap<QString, QVariant>>)), this, SLOT(processResults(myTypes::JsonContent, QList<QMap<QString, QVariant>>)), Qt::DirectConnection );
    connect( this, SIGNAL(errors(QMap<QString, QString> *)), this, SLOT(processErrors(QMap<QString, QString> *)), Qt::DirectConnection );
}

c_actionExecutive::~c_actionExecutive()
{
}

void c_actionExecutive::processData(myStructures::threadData data)
{
        // obsługa otrzymanych danych

        switch (data.req_type) {
        case myTypes::PING:
        {
        }
        case myTypes::REPLY:
        {
            processReply(data, &processedDataErrors);            
            break;
        }
        case myTypes::MESSAGE:
        {
            processMessage(data, &processedDataErrors);
            break;
        }
        case myTypes::GET:
        {
            processGet(data, &processedDataErrors);
            break;
        }
        case myTypes::REQUEST:
        {
        }
        case myTypes::UPDATE:
        {
        }
        case myTypes::DELETE:
        {
        }
        case myTypes::SEND:
        {
        }
        default:
        {
        }
        }
}

void c_actionExecutive::processMessage(myStructures::threadData data, QMap<QString, QString> * processedDataErrors)
{

}

void c_actionExecutive::processGet(myStructures::threadData data, QMap<QString, QString> * processedDataErrors)
{

}

void c_actionExecutive::processReply(myStructures::threadData data, QMap<QString, QString> * processedDataErrors)
{
        c_Parser parser;
        myTypes::JsonContent jsonContent = data.content;
        QList<QMap<QString, QVariant>> jsonData = data.data;

        if (data.thread_dest == myTypes::CLINIC_ERROR_CONTROLLER)
            parser.parseErrors(&jsonData, processedDataErrors);

        if(jsonContent == myTypes::EMPTY) { (*processedDataErrors)[QString("JSON_DOC_ERROR")] = QString("Pusty dokument JSON.\n"); emit errors(processedDataErrors);  return; }
        else { emit resultReady(jsonContent, jsonData); }
}

void c_actionExecutive::processResults(myTypes::JsonContent jsonContent, QList<QMap<QString, QVariant> > results)
{
    switch (jsonContent) {
    case myTypes::LOGGING_ANSWER:
    {
        myStructures::logInConfirmation confirmation;

        confirmation.id = results[0]["id"].toInt();
        confirmation.name = results[0]["name"].toString();
        confirmation.email = results[0]["email"].toString();
        confirmation.verified = results[0]["verified"].toBool();
        confirmation.blocked = results[0]["blocked"].toBool();
        confirmation.create_date = QDateTime::fromString( results[0]["create_date"].toString(), Qt::ISODateWithMs );
        confirmation.verify_date = QDateTime::fromString( results[0]["verify_date"].toString(), Qt::ISODateWithMs );
        confirmation.block_date = QDateTime::fromString( results[0]["block_date"].toString(), Qt::ISODateWithMs );
        confirmation.photo = results[0]["photo"].toByteArray();
        confirmation.logInTime = QDateTime::fromString(results[0]["logInTime"].toString());
        confirmation.role = results[0]["role"].toString();

        emit logInConfirmationReady(confirmation);
        break;
    }
    case myTypes::LOGGING_OUT_ANSWER:
    {
        myStructures::logOutConfirmation confirmation;
        confirmation.id = results[0]["id"].toInt();
        confirmation.logOutTime = QDateTime::fromString(results[0]["logOutTime"].toString());
        confirmation.name = results[0]["name"].toString();
        emit logOutConfirmationReady(confirmation);
        break;
    }
    case myTypes::UNLOCK_ON_IDLE_REQUEST: {
        emit unlockConfirmationReceived( results[0]["unlock"].toBool() );
        break;
    }
    case myTypes::USER_ID_ANSWER:
    {
        qint32 id = results[0]["id"].toInt();
        emit getUserIdResultReady(id);
        break;
    }
    case myTypes::NEW_SESSION_SETTINGS: {
        emit getSessionSettingsFromServerResultReady(results[0]);
        break;
    }
    case myTypes::SESSION_FILE_UPDATE_CONFIRMATION: {
        emit updateSessionFileConfirmationReceived();
        break;
    }
    case myTypes::SESSION_STATE_UPDATE_CONFIRMATION: {
        emit updateSessionStatusConfirmationReceived();
        break;
    }
    case myTypes::SESSION_CLOSE_TIME_UPDATE_CONFIRMATION: {
        emit updateSessionCloseTimeConfirmationReceived();
        break;
    }
    case myTypes::SESSION_UNLOCK_CONFIRMATION: {
        emit unlockConfirmationReceived(true);
        break;
    }
    case myTypes::USER_PROPERTIES_ANSWER: {
        if(results.isEmpty())
            emit userPropertiesReceivedFromServerResultReady( QMap<QString, QVariant>() );
        else
            emit userPropertiesReceivedFromServerResultReady( results[0] );
        break;
    }
    case myTypes::EMPLOYEE_PROPERTIES_ANSWER: {
        if(results.isEmpty())
            emit employeePropertiesReceivedFromServerResultReady( QMap<QString, QVariant>() );
        else
            emit employeePropertiesReceivedFromServerResultReady( results[0] );
        break;
    }
    case myTypes::USER_LOGS_ANSWER: {
        if(results.isEmpty())
            emit userLogsReceivedFromServerResultReady( QList<QMap<QString, QVariant>>() );
        else
            emit userLogsReceivedFromServerResultReady(results);
        break;
    }
    case myTypes::USER_EMPLOYEE_LOGS_ANSWER: {
        if(results.isEmpty())
            emit userEmployeeLogsReceivedFromServerResultReady( QList<QMap<QString, QVariant>>() );
        else
            emit userEmployeeLogsReceivedFromServerResultReady(results);
        break;
    }
    case myTypes::EMPLOYEE_LOGS_ANSWER: {
        if(results.isEmpty())
            emit employeeLogsReceivedFromServerResultReady( QList<QMap<QString, QVariant>>() );
        else
            emit employeeLogsReceivedFromServerResultReady(results);
        break;
    }
    case myTypes::ERRORS: {
        emit errors(&processedDataErrors);
        break;
    }
    default:
    {
        break;
    }
    }
}

void c_actionExecutive::processErrors(QMap<QString, QString> *receivedErrors)
{
    foreach(const QString key, receivedErrors->keys()) {
        emit newLog(QString("%1: %2 \n").arg(key, (*receivedErrors)[key]));
    }
    receivedErrors->clear();
}
