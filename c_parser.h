#ifndef C_PARSER_H
#define C_PARSER_H

#include "c_mystructures.h"

#include <QObject>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QAuthenticator>
#include <QCryptographicHash>

class c_Parser : public QObject
{
    Q_OBJECT
public:
    explicit c_Parser(QObject *parent = nullptr);

    QPair<QByteArray, QByteArray> prepareLogInPacket(QString name, QString encryptedPassword, quint32 threadID = 0);
    QPair<QByteArray, QByteArray> prepareLogInPacket(qint32 id, QString name, QString encryptedPassword, quint32 threadID = 0);
    QPair<QByteArray, QByteArray> prepareLogOutPacket(qint32 id, QString name, QString encryptedPassword, quint32 threadID = 0);
    QPair<QByteArray, QByteArray> prepareGetUserIdPacket(QString name, QString encryptedPassword, quint32 threadID = 0);
    QPair<QByteArray, QByteArray> prepareGetUserPropertiesPacket(QString name, QString encryptedPassword, quint32 threadID = 0);
    QPair<QByteArray, QByteArray> prepareGetEmployeePropertiesPacket(QString name, QString encryptedPassword, quint32 threadID = 0);
    QPair<QByteArray, QByteArray> prepareGetLogsPacket(qint32 id, QString name, QString encryptedPassword, quint32 threadID = 0);

    QPair<QByteArray, QByteArray> unlockOnIdle(QString name, QString encryptedPassword, quint32 threadID = 0);
    QPair<QByteArray, QByteArray> prepareGetSessionSettingsPacket(qint32 id, QString name, QString encryptedPassword, QUuid identifier, quint32 threadID = 0);
    QPair<QByteArray, QByteArray> prepareSendSessionFileToServerPacket(QUuid identifier, QByteArray fileData, qint32 id, QString name, QString encryptedPassword, quint32 threadID = 0);
    QPair<QByteArray, QByteArray> prepareSendSessionStateToServerPacket(QUuid id_session, qint32 state, qint32 id, QString name, QString encryptedPassword, quint32 threadID = 0);
    QPair<QByteArray, QByteArray> prepareUpdateSessionCloseTimeToServerPacket(QUuid id_session, QDateTime time, qint32 id, QString name, QString encryptedPassword, quint32 threadID = 0);


//    QJsonDocument prepareAuthenticateJson(QMap<QString, QVariant> packet_info, qint32 id, QString name, QByteArray password, QMap<QString, QVariant> additionalData = QMap<QString, QVariant>());
//    QJsonDocument prepareAuthenticateJson(QMap<QString, QVariant> packet_info, QString name, QByteArray password, QMap<QString, QVariant> additionalData = QMap<QString, QVariant>());
//    QJsonDocument prepareAuthenticateJson(QMap<QString, QVariant> packet_info, authenticator authData, QMap<QString, QVariant> additionalData = QMap<QString, QVariant>());
//    QJsonDocument prepareTextMessageJson(QMap<QString, QVariant> packet_info, QString message);
//    QJsonDocument prepareBinaryDataJson(QMap<QString, QVariant> packet_info, QByteArray data, qint32 id, QString name, QByteArray password);

    QJsonDocument prepareJson(QMap<QString, QVariant> packet_info, QList<QMap<QString, QVariant>> packet_data);

public slots:
    QPair<QByteArray, QByteArray> parseData(quint64 data_size, QByteArray data);
//    void parseJson(QJsonDocument * json, JsonContent * content, QList<QMap<QString, QVariant> > * data);
    void parseJson(QByteArray * json, myStructures::threadData * data);
    void parseForMd5(myStructures::threadData * data, QByteArray * md5);
    void parseErrors(QList<QMap<QString, QVariant>> * jsonData, QMap<QString, QString> * processedDataErrors);
//    void parseErrorJson(QJsonDocument * json, JsonContent * content, QMap<QString, threadData> *errors);

private:
    QByteArray getJsonMD5Hash(QJsonDocument json);

signals:

};

#endif // C_PARSER_H
