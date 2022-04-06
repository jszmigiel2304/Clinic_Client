#ifndef C_MYSTRUCTURES_H
#define C_MYSTRUCTURES_H

#include <QString>

enum buttonType {BIG_BUTTON = 0, SMALL_BUTTON = 1, LABEL_BUTTON = 2};
enum moduleAction {DO_FUNCTION = 0, SHOW = 1, PROCESS = 2};
enum SessionState {NOT_DEFINED = 0x00, DEFINED_NOT_STARTED = 0x01, STARTED = 0x10, RESTORED = 0x11, PAUSED_NOT_SAVED = 0x20, PAUSED_SAVED = 0x21, CLOSED_NOT_SAVED = 0x30, CLOSED_SAVED = 0x31};
enum ThreadDestination {SERVER = 0x00, CLINIC = 0x10, CLINIC_CONNECTION_CONTROLLER = 0x11};
enum RequestType {PING = 0x00, REPLY = 0x01, MESSAGE = 0x02, GET = 0x03, REQUEST = 0x04, UPDATE = 0x05, DELETE = 0x06, SEND = 0x07};
enum ConnectionToServerState {NOT_CONNECTED = 0x00, CONNECTED_NOT_AUTHORIZED = 0x10, CONNECTED_AND_AUTHORIZED = 0x11, CONNECTION_INTERRUPTED = 0x20, DISCONNECTED_BY_SERVER = 0x21, DISCONNECTED_BY_CLIENT = 0x22};


struct moduleInfo {
    QString name;
    QString displayedName;
    QString icon;
    QString description;
    QString pressedAction;
};

struct threadData {
    quint32 thread_id;
    ThreadDestination thread_dest;
    RequestType req_type;
    quint8 type_flag;
    quint32 data_size;
    QByteArray data;
};


#endif // C_MYSTRUCTURES_H
