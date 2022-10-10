#include "c_actionexecutive.h"

c_actionExecutive::c_actionExecutive(QObject *parent)
    : QObject{parent}
{
    connect( this, SIGNAL(resultReady(JsonContent, QList<QMap<QString, QVariant>>)), this, SLOT(processResults(JsonContent, QList<QMap<QString, QVariant>>)), Qt::DirectConnection );
    connect( this, SIGNAL(errors(QMap<QString, QString> *)), this, SLOT(processErrors(QMap<QString, QString> *)), Qt::DirectConnection );
}

c_actionExecutive::~c_actionExecutive()
{
}

void c_actionExecutive::processData(threadData data)
{
        // obsługa otrzymanych danych

        switch (data.req_type) {
        case PING:
        {
        }
        case REPLY:
        {
            processReply(data, &processedDataErrors);            
            break;
        }
        case MESSAGE:
        {
            processMessage(data, &processedDataErrors);
            break;
        }
        case GET:
        {
            processGet(data, &processedDataErrors);
            break;
        }
        case REQUEST:
        {
        }
        case UPDATE:
        {
        }
        case DELETE:
        {
        }
        case SEND:
        {
        }
        default:
        {
        }
        }
}

void c_actionExecutive::processMessage(threadData data, QMap<QString, QString> * processedDataErrors)
{

}

void c_actionExecutive::processGet(threadData data, QMap<QString, QString> * processedDataErrors)
{

}

void c_actionExecutive::processReply(threadData data, QMap<QString, QString> * processedDataErrors)
{
        c_Parser parser;
        JsonContent jsonContent = data.content;
        QList<QMap<QString, QVariant>> jsonData = data.data;

        if (data.thread_dest == CLINIC_ERROR_CONTROLLER)
            parser.parseErrors(&jsonData, processedDataErrors);

        if(jsonContent == EMPTY) { (*processedDataErrors)[QString("JSON_DOC_ERROR")] = QString("Pusty dokument JSON.\n"); emit errors(processedDataErrors);  return; }
        else { emit resultReady(jsonContent, jsonData); }
}

void c_actionExecutive::processResults(JsonContent jsonContent, QList<QMap<QString, QVariant> > results)
{
    switch (jsonContent) {
    case LOGGING_ANSWER:
    {
        logInConfirmation confirmation;
        confirmation.blocked = results[0]["blocked"].toBool();
        confirmation.email = results[0]["email"].toString();
        confirmation.id = results[0]["id"].toInt();
        confirmation.logInTime = QDateTime::fromString(results[0]["logInTime"].toString());
        confirmation.name = results[0]["name"].toString();
        confirmation.verified = results[0]["verified"].toBool();
        if(results[0]["role"].toString() == "SUPER") confirmation.role = SUPER;
        else if(results[0]["role"].toString() == "ADMIN") confirmation.role = ADMIN;
        else if(results[0]["role"].toString() == "MODERATOR") confirmation.role = MODERATOR;
        else if(results[0]["role"].toString() == "USER") confirmation.role = USER;
        else if(results[0]["role"].toString() == "GUEST") confirmation.role = GUEST;
        else if(results[0]["role"].toString() == "") confirmation.role = NO_ROLE;

        emit logInConfirmationReady(confirmation);
        break;
    }
    case LOGGING_OUT_ANSWER:
    {
        logOutConfirmation confirmation;
        confirmation.id = results[0]["id"].toInt();
        confirmation.logOutTime = QDateTime::fromString(results[0]["logOutTime"].toString());
        confirmation.name = results[0]["name"].toString();
        emit logOutConfirmationReady(confirmation);
        break;
    }
    case UNLOCK_ON_IDLE_REQUEST: {
        emit unlockConfirmationReceived( results[0]["unlock"].toBool() );
        break;
    }
    case USER_ID_ANSWER:
    {
        qint32 id = results[0]["id"].toInt();
        emit getUserIdResultReady(id);
        break;
    }
    case NEW_SESSION_SETTINGS: {
        emit getSessionSettingsFromServerResultReady(results[0]);
        break;
    }
    case SESSION_FILE_UPDATE_CONFIRMATION: {
        emit updateSessionFileConfirmationReceived();
        break;
    }
    case SESSION_STATE_UPDATE_CONFIRMATION: {
        emit updateSessionStatusConfirmationReceived();
        break;
    }
    case SESSION_CLOSE_TIME_UPDATE_CONFIRMATION: {
        emit updateSessionCloseTimeConfirmationReceived();
        break;
    }
    case SESSION_UNLOCK_CONFIRMATION: {
        emit unlockConfirmationReceived(true);
        break;
    }
    case ERRORS: {
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
