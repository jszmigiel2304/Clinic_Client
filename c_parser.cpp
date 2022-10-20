#include "c_parser.h"

c_Parser::c_Parser(QObject *parent) : QObject(parent)
{

}

QPair<QByteArray, QByteArray> c_Parser::ParseReceivedPacket(quint64 size, QByteArray data)
{
    QDataStream rs(&data, QIODevice::ReadOnly);
    rs.setVersion(QDataStream::Qt_6_0);

    QByteArray md5Hash;
    QByteArray jsonPacket;

    rs >> md5Hash >> jsonPacket;


    QPair<QByteArray, QByteArray> pair;
    pair.first = QByteArray::fromHex(md5Hash);
    pair.second = jsonPacket;
    return pair;
}

myStructures::threadData c_Parser::ParseJsonPacket(QByteArray json, qintptr socketDescriptor)
{
    myStructures::threadData packetData;

    packetData.socketDescriptor = socketDescriptor;

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json, &error);

    if (error.error != QJsonParseError::NoError) {
        packetData.content = myTypes::ERRORS;
        QList<QMap<QString, QVariant>> attData;
        QMap<QString, QVariant> error;
        error["error_0"] = QString("JSON reading error.");
        attData.append(error);
        return packetData;
    }

    QJsonObject mainObject = jsonDoc.object();


    packetData.thread_dest = static_cast<myTypes::ThreadDestination>( mainObject["thread_dest"].toInteger() );
    packetData.thread_id = mainObject["thread_id"].toInteger();
    packetData.req_type = static_cast<myTypes::RequestType>( mainObject["req_type"].toInteger() );
    packetData.type_flag = mainObject["type_flag"].toInteger();
    packetData.content = static_cast<myTypes::JsonContent>( mainObject["content"].toInteger() );


    QJsonArray dataArray = mainObject["data"].toArray();

    for(int i = 0; i < dataArray.size(); i++)
    {
        QJsonObject object = dataArray[i].toObject();

        QMap<QString, QVariant> map;
        foreach(QString key, object.keys()) {
            map[key] = object[key].toVariant();
        }
        packetData.data.append(map);
    }

    return packetData;
}

QPair<QByteArray, QByteArray> c_Parser::prepareLogInPacket(QString name, QString encryptedPassword, quint32 threadID)
{
    QByteArray packet;

    QList<QMap<QString, QVariant>> packetData;

    QMap<QString, QVariant> packetInfo;
    packetInfo["thread_dest"] = static_cast<qint8>(myTypes::SERVER);
    packetInfo["thread_id"] = threadID;
    packetInfo["req_type"] = static_cast<qint8>(myTypes::REQUEST);
    packetInfo["type_flag"] = 0x00000000;
    packetInfo["content"] = static_cast<qint32>(myTypes::LOGGING_DATA);

    QMap<QString, QVariant> userInfo;
    userInfo["name"] = name;
    userInfo["encryptedPassword"] = encryptedPassword;
    packetData.append(userInfo);

    QJsonDocument jsonPacket = prepareJson(packetInfo, packetData);

    QByteArray JsonMD5 = getJsonMD5Hash(jsonPacket);

    QDataStream ds2(&packet, QIODevice::ReadWrite);
    ds2.setVersion(QDataStream::Qt_6_0);

    ds2 << JsonMD5.toHex() << jsonPacket.toJson();

    QPair<QByteArray, QByteArray> pair(JsonMD5.toHex(), packet);
    return pair;
}

QPair<QByteArray, QByteArray> c_Parser::prepareLogInPacket(qint32 id, QString name, QString encryptedPassword, quint32 threadID)
{
    QByteArray packet;

    QList<QMap<QString, QVariant>> packetData;

    QMap<QString, QVariant> packetInfo;
    packetInfo["thread_dest"] = static_cast<qint8>(myTypes::SERVER);
    packetInfo["thread_id"] = threadID;
    packetInfo["req_type"] = static_cast<qint8>(myTypes::REQUEST);
    packetInfo["type_flag"] = 0x00000000;
    packetInfo["content"] = static_cast<qint32>(myTypes::LOGGING_DATA);

    QMap<QString, QVariant> userInfo;
    userInfo["id"] = id;
    userInfo["name"] = name;
    userInfo["encryptedPassword"] = encryptedPassword;
    packetData.append(userInfo);

    QJsonDocument jsonPacket = prepareJson(packetInfo, packetData);
    QByteArray JsonMD5 = getJsonMD5Hash(jsonPacket);


    QDataStream ds2(&packet, QIODevice::ReadWrite);
    ds2.setVersion(QDataStream::Qt_6_0);

    ds2 << JsonMD5.toHex() << jsonPacket.toJson();

    QPair<QByteArray, QByteArray> pair(JsonMD5.toHex(), packet);
    return pair;
}

QPair<QByteArray, QByteArray> c_Parser::prepareLogOutPacket(qint32 id, QString name, QString encryptedPassword, quint32 threadID)
{
    QByteArray packet;

    QList<QMap<QString, QVariant>> packetData;

    QMap<QString, QVariant> packetInfo;
    packetInfo["thread_dest"] = static_cast<qint8>(myTypes::SERVER);
    packetInfo["thread_id"] = threadID;
    packetInfo["req_type"] = static_cast<qint8>(myTypes::REQUEST);
    packetInfo["type_flag"] = 0x00000001;
    packetInfo["content"] = static_cast<qint32>(myTypes::LOGGING_DATA);

    QMap<QString, QVariant> userInfo;
    userInfo["id"] = id;
    userInfo["name"] = name;
    userInfo["encryptedPassword"] = encryptedPassword;
    packetData.append(userInfo);

    QJsonDocument jsonPacket = prepareJson(packetInfo, packetData);
    QByteArray JsonMD5 = getJsonMD5Hash(jsonPacket);

    QDataStream ds2(&packet, QIODevice::ReadWrite);
    ds2.setVersion(QDataStream::Qt_6_0);
    ds2 << JsonMD5.toHex() << jsonPacket.toJson();

    QPair<QByteArray, QByteArray> pair(JsonMD5.toHex(), packet);
    return pair;
}

QPair<QByteArray, QByteArray> c_Parser::prepareGetUserIdPacket(QString name, QString encryptedPassword, quint32 threadID)
{
    QByteArray packet;

    QList<QMap<QString, QVariant>> packetData;

    QMap<QString, QVariant> packetInfo;
    packetInfo["thread_dest"] = static_cast<qint8>(myTypes::SERVER);
    packetInfo["thread_id"] = threadID;
    packetInfo["req_type"] = static_cast<qint8>(myTypes::GET);
    packetInfo["type_flag"] = 0x00000000;
    packetInfo["content"] = static_cast<qint32>(myTypes::USER_ID_REQUEST);

    QMap<QString, QVariant> userInfo;
    userInfo["name"] = name;
    userInfo["encryptedPassword"] = encryptedPassword;
    packetData.append(userInfo);

    QJsonDocument jsonPacket = prepareJson(packetInfo, packetData);
    QByteArray JsonMD5 = getJsonMD5Hash(jsonPacket);

    QDataStream ds2(&packet, QIODevice::ReadWrite);
    ds2.setVersion(QDataStream::Qt_6_0);

    ds2 << JsonMD5.toHex() << jsonPacket.toJson();

    QPair<QByteArray, QByteArray> pair(JsonMD5.toHex(), packet);
    return pair;
}

QPair<QByteArray, QByteArray> c_Parser::prepareGetUserPropertiesPacket(QString name, QString encryptedPassword, quint32 threadID)
{
    QByteArray packet;

    QList<QMap<QString, QVariant>> packetData;

    QMap<QString, QVariant> packetInfo;
    packetInfo["thread_dest"] = static_cast<qint8>(myTypes::SERVER);
    packetInfo["thread_id"] = threadID;
    packetInfo["req_type"] = static_cast<qint8>(myTypes::GET);
    packetInfo["type_flag"] = 0x00000001;
    packetInfo["content"] = static_cast<qint32>(myTypes::USER_PROPERTIES_REQUEST);

    QMap<QString, QVariant> userInfo;
    userInfo["name"] = name;
    userInfo["encryptedPassword"] = encryptedPassword;
    packetData.append(userInfo);

    QJsonDocument jsonPacket = prepareJson(packetInfo, packetData);
    QByteArray JsonMD5 = getJsonMD5Hash(jsonPacket);

    QDataStream ds2(&packet, QIODevice::ReadWrite);
    ds2.setVersion(QDataStream::Qt_6_0);

    ds2 << JsonMD5.toHex() << jsonPacket.toJson();

    QPair<QByteArray, QByteArray> pair(JsonMD5.toHex(), packet);
    return pair;
}

QPair<QByteArray, QByteArray> c_Parser::prepareGetEmployeePropertiesPacket(QString name, QString encryptedPassword, quint32 threadID)
{
    QByteArray packet;

    QList<QMap<QString, QVariant>> packetData;

    QMap<QString, QVariant> packetInfo;
    packetInfo["thread_dest"] = static_cast<qint8>(myTypes::SERVER);
    packetInfo["thread_id"] = threadID;
    packetInfo["req_type"] = static_cast<qint8>(myTypes::GET);
    packetInfo["type_flag"] = 0x00000002;
    packetInfo["content"] = static_cast<qint32>(myTypes::EMPLOYEE_PROPERTIES_REQUEST);

    QMap<QString, QVariant> userInfo;
    userInfo["name"] = name;
    userInfo["encryptedPassword"] = encryptedPassword;
    packetData.append(userInfo);

    QJsonDocument jsonPacket = prepareJson(packetInfo, packetData);
    QByteArray JsonMD5 = getJsonMD5Hash(jsonPacket);

    QDataStream ds2(&packet, QIODevice::ReadWrite);
    ds2.setVersion(QDataStream::Qt_6_0);

    ds2 << JsonMD5.toHex() << jsonPacket.toJson();

    QPair<QByteArray, QByteArray> pair(JsonMD5.toHex(), packet);
    return pair;
}

QPair<QByteArray, QByteArray> c_Parser::prepareGetLogsPacket(qint32 id, QString name, QString encryptedPassword, quint32 threadID)
{
    QByteArray packet;

    QList<QMap<QString, QVariant>> packetData;

    QMap<QString, QVariant> packetInfo;
    packetInfo["thread_dest"] = static_cast<qint8>(myTypes::SERVER);
    packetInfo["thread_id"] = threadID;
    packetInfo["req_type"] = static_cast<qint8>(myTypes::GET);
    packetInfo["type_flag"] = 0x00000005;
    packetInfo["content"] = static_cast<qint32>(myTypes::USER_EMPLOYEE_LOGS_REQUEST);

    QMap<QString, QVariant> userInfo;
    userInfo["id"] = id;
    userInfo["name"] = name;
    userInfo["encryptedPassword"] = encryptedPassword;
    packetData.append(userInfo);

    QJsonDocument jsonPacket = prepareJson(packetInfo, packetData);
    QByteArray JsonMD5 = getJsonMD5Hash(jsonPacket);

    QDataStream ds2(&packet, QIODevice::ReadWrite);
    ds2.setVersion(QDataStream::Qt_6_0);

    ds2 << JsonMD5.toHex() << jsonPacket.toJson();

    QPair<QByteArray, QByteArray> pair(JsonMD5.toHex(), packet);
    return pair;
}

QPair<QByteArray, QByteArray> c_Parser::unlockOnIdle(QString name, QString encryptedPassword, quint32 threadID)
{
    QByteArray packet;

    QList<QMap<QString, QVariant>> packetData;

    QMap<QString, QVariant> packetInfo;
    packetInfo["thread_dest"] = static_cast<qint8>(myTypes::SERVER);
    packetInfo["thread_id"] = threadID;
    packetInfo["req_type"] = static_cast<qint8>(myTypes::REQUEST);
    packetInfo["type_flag"] = 0x00000002;
    packetInfo["content"] = static_cast<qint32>(myTypes::UNLOCK_ON_IDLE_REQUEST);

    QMap<QString, QVariant> userInfo;
    userInfo["name"] = name;
    userInfo["encryptedPassword"] = encryptedPassword;
    packetData.append(userInfo);

    QJsonDocument jsonPacket = prepareJson(packetInfo, packetData);
    QByteArray JsonMD5 = getJsonMD5Hash(jsonPacket);

    QDataStream ds2(&packet, QIODevice::ReadWrite);
    ds2.setVersion(QDataStream::Qt_6_0);

    ds2 << JsonMD5.toHex() << jsonPacket.toJson();

    QPair<QByteArray, QByteArray> pair(JsonMD5.toHex(), packet);
    return pair;
}

QPair<QByteArray, QByteArray> c_Parser::prepareGetSessionSettingsPacket(qint32 id, QString name, QString encryptedPassword, QUuid identifier, quint32 threadID)
{
    QByteArray packet;

    QList<QMap<QString, QVariant>> packetData;

    QMap<QString, QVariant> packetInfo;
    packetInfo["thread_dest"] = static_cast<qint8>(myTypes::SERVER);
    packetInfo["thread_id"] = threadID;
    packetInfo["req_type"] = static_cast<qint8>(myTypes::GET);
    packetInfo["type_flag"] = 0x00000100;
    packetInfo["content"] = static_cast<qint32>(myTypes::NEW_SESSION_SETTINGS);

    QMap<QString, QVariant> data;
    data["id"] = id;
    data["name"] = name;
    data["encryptedPassword"] = encryptedPassword;
    data["session_identifier"] = identifier;
    packetData.append(data);

    QJsonDocument jsonPacket = prepareJson(packetInfo, packetData);
    QByteArray JsonMD5 = getJsonMD5Hash(jsonPacket);


    QDataStream ds2(&packet, QIODevice::ReadWrite);
    ds2.setVersion(QDataStream::Qt_6_0);

    ds2 << JsonMD5.toHex() << jsonPacket.toJson();

    QPair<QByteArray, QByteArray> pair(JsonMD5.toHex(), packet);
    return pair;
}

QPair<QByteArray, QByteArray> c_Parser::prepareSendSessionFileToServerPacket(QUuid identifier, QByteArray fileData, qint32 id, QString name, QString encryptedPassword, quint32 threadID)
{
    QByteArray packet;

    QList<QMap<QString, QVariant>> packetData;

    QMap<QString, QVariant> packetInfo;
    packetInfo["thread_dest"] = static_cast<qint8>(myTypes::SERVER);
    packetInfo["thread_id"] = threadID;
    packetInfo["req_type"] = static_cast<qint8>(myTypes::UPDATE);
    packetInfo["type_flag"] = 0x01000001;
    packetInfo["content"] = static_cast<qint32>(myTypes::SESSION_FILE);

    QMap<QString, QVariant> data;
    data["id"] = id;
    data["name"] = name;
    data["encryptedPassword"] = encryptedPassword;
    data["session_identifier"] = identifier;
    data["session_file_data"] = QString(fileData.toHex());
    packetData.append(data);

    QJsonDocument jsonPacket = prepareJson(packetInfo, packetData);
    QByteArray JsonMD5 = getJsonMD5Hash(jsonPacket);

    QDataStream ds2(&packet, QIODevice::ReadWrite);
    ds2.setVersion(QDataStream::Qt_6_0);

    ds2 << JsonMD5.toHex() << jsonPacket.toJson();

    QPair<QByteArray, QByteArray> pair(JsonMD5.toHex(), packet);
    return pair;
}

QPair<QByteArray, QByteArray> c_Parser::prepareSendSessionStateToServerPacket(QUuid id_session, qint32 state, qint32 id, QString name, QString encryptedPassword, quint32 threadID)
{
    QByteArray packet;

    QList<QMap<QString, QVariant>> packetData;

    QMap<QString, QVariant> packetInfo;
    packetInfo["thread_dest"] = static_cast<qint8>(myTypes::SERVER);
    packetInfo["thread_id"] = threadID;
    packetInfo["req_type"] = static_cast<qint8>(myTypes::UPDATE);
    packetInfo["type_flag"] = 0x01000002;
    packetInfo["content"] = static_cast<qint32>(myTypes::SESSION_STATE);

    QMap<QString, QVariant> data;
    data["id"] = id;
    data["name"] = name;
    data["encryptedPassword"] = encryptedPassword;
    data["session_identifier"] = id_session;
    data["session_state"] = state;
    packetData.append(data);

    QJsonDocument jsonPacket = prepareJson(packetInfo, packetData);
    QByteArray JsonMD5 = getJsonMD5Hash(jsonPacket);

    QDataStream ds2(&packet, QIODevice::ReadWrite);
    ds2.setVersion(QDataStream::Qt_6_0);

    ds2 << JsonMD5.toHex() << jsonPacket.toJson();

    QPair<QByteArray, QByteArray> pair(JsonMD5.toHex(), packet);
    return pair;
}

QPair<QByteArray, QByteArray> c_Parser::prepareUpdateSessionCloseTimeToServerPacket(QUuid id_session, QDateTime time, qint32 id, QString name, QString encryptedPassword, quint32 threadID)
{
    QByteArray packet;

    QList<QMap<QString, QVariant>> packetData;

    QMap<QString, QVariant> packetInfo;
    packetInfo["thread_dest"] = static_cast<qint8>(myTypes::SERVER);
    packetInfo["thread_id"] = threadID;
    packetInfo["req_type"] = static_cast<qint8>(myTypes::UPDATE);
    packetInfo["type_flag"] = 0x01000003;
    packetInfo["content"] = static_cast<qint32>(myTypes::SESSION_CLOSE_TIME);

    QMap<QString, QVariant> data;
    data["id"] = id;
    data["name"] = name;
    data["encryptedPassword"] = encryptedPassword;
    data["session_identifier"] = id_session;
    data["close_time"] = time.toString();
    packetData.append(data);

    QJsonDocument jsonPacket = prepareJson(packetInfo, packetData);
    QByteArray JsonMD5 = getJsonMD5Hash(jsonPacket);

    QDataStream ds2(&packet, QIODevice::ReadWrite);
    ds2.setVersion(QDataStream::Qt_6_0);

    ds2 << JsonMD5.toHex() << jsonPacket.toJson();

    QPair<QByteArray, QByteArray> pair(JsonMD5.toHex(), packet);
    return pair;
}

QPair<QByteArray, QByteArray> c_Parser::prepareRequestConnectionToProcessPacket(quint32 threadID)
{
    QByteArray packet;

    QList<QMap<QString, QVariant>> packetData;

    QMap<QString, QVariant> packetInfo;
    packetInfo["thread_dest"] = static_cast<qint8>(myTypes::CLINIC_MODULE);
    packetInfo["thread_id"] = threadID;
    packetInfo["req_type"] = static_cast<qint8>(myTypes::REQUEST);
    packetInfo["type_flag"] = 0x01000003;
    packetInfo["content"] = static_cast<qint32>(myTypes::CONNECTION_TO_PROCESS_REQUEST);


    QJsonDocument jsonPacket = prepareJson(packetInfo, packetData);
    QByteArray JsonMD5 = getJsonMD5Hash(jsonPacket);

    QDataStream ds2(&packet, QIODevice::ReadWrite);
    ds2.setVersion(QDataStream::Qt_6_0);

    ds2 << JsonMD5.toHex() << jsonPacket.toJson();

    QPair<QByteArray, QByteArray> pair(JsonMD5.toHex(), packet);
    return pair;
}


QJsonDocument c_Parser::prepareJson(QMap<QString, QVariant> packet_info, QList<QMap<QString, QVariant>> packet_data)
{
    QJsonDocument jsonDoc;
    QJsonObject mainobject;

    QJsonArray packetDataArray;

    foreach(QString key, packet_info.keys()) {
        mainobject[key] = QJsonValue::fromVariant(packet_info[key]);
    }


    for(int i = 0; i < packet_data.size(); i++) {
        QJsonObject data_obj;

        foreach(QString key, packet_data[i].keys()) {
            data_obj[key] = QJsonValue::fromVariant(packet_data[i][key]);
        }

        packetDataArray.append(data_obj);
    }

    mainobject["data"] = packetDataArray;
    jsonDoc.setObject(mainobject);
    return jsonDoc;
}

QPair<QByteArray, QByteArray>  c_Parser::parseData(quint64 data_size, QByteArray data)
{
    QDataStream rs(&data, QIODevice::ReadOnly);
    rs.setVersion(QDataStream::Qt_6_0);

    QByteArray md5Hash;
    QByteArray json;

    rs >> md5Hash >> json;

    QPair<QByteArray, QByteArray> pair(QByteArray::fromHex(md5Hash), json);
    return pair;
}

void c_Parser::parseJson(QByteArray * json, myStructures::threadData * data)
{
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(*json, &error);

    if (error.error != QJsonParseError::NoError) {
        (*data).content = myTypes::ERRORS;
        QList<QMap<QString, QVariant>> attData;
        QMap<QString, QVariant> error;
        error["error_0"] = QString("JSON reading error.");
        attData.append(error);
        (*data).data = attData;
        return;
    }

    QJsonObject mainObject = jsonDoc.object();

    (*data).thread_dest = static_cast<myTypes::ThreadDestination>( mainObject["thread_dest"].toInteger() );
    (*data).thread_id = mainObject["thread_id"].toInteger();
    (*data).req_type = static_cast<myTypes::RequestType>( mainObject["req_type"].toInteger() );
    (*data).type_flag = mainObject["type_flag"].toInteger();
    (*data).content = static_cast<myTypes::JsonContent>( mainObject["content"].toInteger() );
    (*data).ref_md5 = QByteArray::fromHex( (mainObject["ref_md5"].toVariant()).toByteArray() ).toHex();


    QJsonArray dataArray = mainObject["data"].toArray();

    for(int i = 0; i < dataArray.size(); i++)
    {
        QJsonObject object = dataArray[i].toObject();

        QMap<QString, QVariant> map;
        foreach(QString key, object.keys()) {
            map[key] = object[key].toVariant();
        }
        (*data).data.append(map);
    }
}

void c_Parser::parseForMd5(myStructures::threadData *data, QByteArray *md5)
{
    *md5 = data->data[0]["md5"].toByteArray();
}

void c_Parser::parseErrors(QList<QMap<QString, QVariant> > *jsonData, QMap<QString, QString> *processedDataErrors)
{
    foreach (const QString key, (*jsonData).data()->keys()) {
        (*processedDataErrors)[key] = (*jsonData).data()[0][key].toString();
    }

}


QByteArray c_Parser::getJsonMD5Hash(QJsonDocument json)
{
    QByteArray md5Hash;

    QCryptographicHash hasher(QCryptographicHash::Md5);
    hasher.addData(json.toJson());
    md5Hash = hasher.result();

    return md5Hash;
}
