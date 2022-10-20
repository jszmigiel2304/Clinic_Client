#ifndef C_MYSTRUCTURES_H
#define C_MYSTRUCTURES_H

#include <QString>
#include <QUuid>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QByteArray>
#include <QHostAddress>

namespace myTypes {
    enum buttonType {BIG_BUTTON = 0, SMALL_BUTTON = 1, LABEL_BUTTON = 2};
    enum moduleAction {DO_FUNCTION = 0, SHOW = 1, PROCESS = 2};
    enum SessionState {NOT_DEFINED = 0x00, DEFINED_NOT_STARTED = 0x01, STARTED = 0x10, RESTORED = 0x11, RESTARTED = 0x12,
                       PAUSED_NOT_SAVED = 0x20, PAUSED_SAVED = 0x21, CLOSED_NOT_SAVED = 0x30, CLOSED_SAVED = 0x31, ERROR = 0xff
                      };
    enum ThreadDestination {SERVER = 0x00, CLINIC = 0x10, CLINIC_CONNECTION_CONTROLLER = 0x11, CLINIC_SESSION_CONTROLLER = 0x12, CLINIC_LOGGED_USER_CONTROLLER = 0x13, CLINIC_MODULE_PROCESS_CONTROLLER = 0x14,
                            CLINIC_MODULE = 0x15, CLINIC_MODULE_ERROR = 0x16, CLINIC_MODULE_CONNECTION_CONTROLLER = 0x17,
                            SERVER_ERROR_CONTROLLER = 0x20, CLINIC_ERROR_CONTROLLER = 0x21};
    enum RequestType {PING = 0x00, REPLY = 0x01, MESSAGE = 0x02, GET = 0x03, REQUEST = 0x04, UPDATE = 0x05, DELETE = 0x06, SEND = 0x07, CONFIRM = 0x08, RECEIVE_CONFIRMATION = 0x09};
    enum ConnectionToServerState {NOT_CONNECTED = 0x00, CONNECTED_NOT_AUTHORIZED = 0x10, CONNECTED_AND_AUTHORIZED = 0x11, CONNECTION_INTERRUPTED = 0x20, DISCONNECTED_BY_SERVER = 0x21, DISCONNECTED_BY_CLIENT = 0x22};
    enum JsonContent { EMPTY = 0x00000000, ERRORS = 0x00000001, TEXT_MESSAGE = 0x00000002, BINARY_DATA = 0x00000003, PACKET_RECEIVE_CONFIRMATION = 0x00000004,
                       LOGGING_DATA = 0x00000100, LOGGING_DATA_ADDITIONAL_DATA = 0xff000100, LOGGING_ANSWER = 0x00000101, LOGGING_OUT_ANSWER = 0x00000102, UNLOCK_ON_IDLE_REQUEST = 0x00000103,
                       USER_ID_REQUEST = 0x00000200, USER_ID_ANSWER = 0x00000201, USER_PROPERTIES_REQUEST = 0x00000202, USER_PROPERTIES_ANSWER = 0x00000203,
                       USER_LOGS_REQUEST = 0x00000204, USER_LOGS_ANSWER = 0x00000205, USER_EMPLOYEE_LOGS_REQUEST = 0x00000206, USER_EMPLOYEE_LOGS_ANSWER = 0x00000207,
                     NEW_SESSION_SETTINGS = 0x00000301, USER_NOT_FINISHED_SESSIONS = 0x00000302, SESSION_FILE = 0x00000303, SESSION_STATE = 0x00000304, SESSION_CLOSE_TIME = 0x00000305,
                      SESSION_STATE_UPDATE_CONFIRMATION = 0x000003F0, SESSION_FILE_UPDATE_CONFIRMATION = 0x000003F1, SESSION_CLOSE_TIME_UPDATE_CONFIRMATION = 0x000003F2, SESSION_UNLOCK_CONFIRMATION = 0x000003F3,
                       EMPLOYEE_PROPERTIES_REQUEST = 0x00000400, EMPLOYEE_PROPERTIES_ANSWER = 0x00000401, EMPLOYEE_LOGS_REQUEST = 0x00000402, EMPLOYEE_LOGS_ANSWER = 0x00000403,

                       CONNECTION_TO_PROCESS_REQUEST = 0xFFFF0000
                     };


    //enum UserRole { NO_ROLE = 0x00, GUEST = 0x01, USER = 0x02, MODERATOR = 0x03, ADMIN = 0x04, SUPER = 0x05 };
}

namespace myStructures {

    struct authenticator {
        qint32 id;
        QString name;
        QString password;
    };

    struct logInConfirmation {
        qint32 id;
        QString name;
        QString email;
        bool verified;
        bool blocked;
        QDateTime create_date;
        QDateTime verify_date;
        QDateTime block_date;
        QByteArray photo;
        QString role;
        QDateTime logInTime;
    };

    struct logOutConfirmation {
        qint32 id;
        QString name;
        QDateTime logOutTime;
    };


    struct moduleInfo {
        QString name;
        QString displayedName;
        QString icon;
        QString description;
        QString pressedAction;
    };

    struct threadData {
        qintptr socketDescriptor;
        myTypes::JsonContent content;
        quint32 thread_id;
        myTypes::ThreadDestination thread_dest;
        myTypes::RequestType req_type;
        quint32 type_flag;
        QByteArray ref_md5;
        //quint64 data_size;
        QList<QMap<QString, QVariant>> data;
    };

    struct processedThreadData {
        QByteArray md5Hash;
        qintptr socketDescriptor;
        myTypes::JsonContent content;
        quint32 thread_id;
        myTypes::ThreadDestination thread_dest;
        myTypes::RequestType req_type;
        quint32 type_flag;
        //quint64 data_size;
        QList<QMap<QString, QVariant>> data;
    };


    struct packet {
        QByteArray md5_hash;
        QByteArray packet_to_send;
        bool wait_for_reply;
    };

    struct processedPacket {
        QByteArray md5_hash;
        threadData data;
        bool wait_for_reply = false;
    };

    struct myLog {
        QDateTime time;
        QHostAddress ip_address;
        QString log_text;

        QMap<QString, QVariant> toMap() const {
            QMap<QString, QVariant> map;
            map["activity"] = this->log_text;
            map["ip_address"] = this->ip_address.toString();
            map["log_time"] = this->time.toString(Qt::ISODateWithMs);
            return map;
        }

        static myLog fromMap(const QMap<QString, QVariant>& map) {
            myLog log;
            log.log_text = map["activity"].toString();
            log.ip_address = QHostAddress( QHostAddress::parseSubnet(map["ip_address"].toString()).first );
            log.time = QDateTime::fromString(map["log_time"].toString(), Qt::ISODateWithMs);
            return log;
        }

        bool operator<(const myLog& other) const { return time < other.time; }
    };
}




#endif // C_MYSTRUCTURES_H
