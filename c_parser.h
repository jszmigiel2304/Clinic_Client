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

    QPair<QByteArray, QByteArray> ParseReceivedPacket(quint64 size, QByteArray data/*, qintptr socketDescriptor*/);
    myStructures::threadData ParseJsonPacket(QByteArray json, qintptr socketDescriptor);
    //------------------------------ client -> server ------------------------------------------------------------------------------------//
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
    //------------------------------------------------------------------------------------------------------------------------------------------------------------------//


    //------------------------------ client -> modules ------------------------------------------------------------------------------------//
    QPair<QByteArray, QByteArray> prepareRequestConnectionToProcessPacket(quint32 threadID = 0);
    QPair<QByteArray, QByteArray> prepareConnectionEstablishedConfirmationPacket(quint32 threadID = 0);
    //------------------------------------------------------------------------------------------------------------------------------------------------------------------//


    QJsonDocument prepareJson(QMap<QString, QVariant> packet_info, QList<QMap<QString, QVariant>> packet_data);
    QByteArray getJsonMD5Hash(QJsonDocument json);
    void insertAuthDataInJson(QJsonDocument *json, QString name, QString encryptedPassword);
    void insertAuthDataInJson(QByteArray *json, QString name, QString encryptedPassword);

public slots:
    QPair<QByteArray, QByteArray> parseData(quint64 data_size, QByteArray data);
    void parseJson(QByteArray * json, myStructures::threadData * data);
    void parseForMd5(myStructures::threadData * data, QByteArray * md5);
    void parseErrors(QList<QMap<QString, QVariant>> * jsonData, QMap<QString, QString> * processedDataErrors);

private:

signals:

};

#endif // C_PARSER_H
