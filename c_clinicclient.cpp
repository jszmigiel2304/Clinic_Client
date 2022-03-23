#include "c_clinicclient.h"

c_ClinicClient::c_ClinicClient(QObject *parent)
{
    setParent(parent);
    connect(qApp, SIGNAL(aboutToQuit()), SLOT(deleteLater()));

    //----------------------------------------//
    settCtrlr = new c_SettingsController();

    modCtrlr = new c_modulesController();

    trayIcon = c_AppTrayIcon::Instance(this);
    mainWindow = w_MainWindow::Instance();
    connectionCtrlr = c_connectionToServerController::Instance();
    user = c_loggedUser::Instance();

    logsWindow = w_logsWindow::Instance();
    logsWindow->setWindowModality(Qt::NonModal);

    sessionCtrlr = c_SessionController::Instance();


    //----------------------------------------//

    //------ wczytywanie ustawień ---------------//

    settCtrlr->LoadSettings();

    QMap<QString, QVariant> map;
    if( !(map = settCtrlr->getSettings("user")).empty() ) {
        this->user->setName( map["name"].toString() );
        this->user->setPassword( map["password"].toString() );
    }

    modCtrlr->LoadSettings( settCtrlr->getSettings("application") );
    foreach ( QString str, modCtrlr->getModulesNames() ) {
        modCtrlr->LoadModuleSettings(str, settCtrlr->getSettings(str) );
    }


    //----------------------------------------//


}

c_ClinicClient::~c_ClinicClient()
{

}

void c_ClinicClient::run()
{
    this->connectionCtrlr->setConnection( settCtrlr->getSettings("server") );
    connect( this->connectionCtrlr, SIGNAL(dataReceived(quint32, QByteArray)), this, SLOT(dataReceived(quint32, QByteArray)) );
    this->connectionCtrlr->runSocket();

    QStringList modulesNames;

    QString modulesString = this->settCtrlr->getSettings("application")["modules"].toString();
    QStringList modules = modulesString.split(','/*, QString::SkipEmptyParts*/);
    foreach (QString str1, modules) {
        modulesNames << str1;
    }

    this->modCtrlr->LoadModules(modulesNames);

    mainWindow->refresh();
    mainWindow->show();
    trayIcon->show();

    if (logsWindow != nullptr)
        logsWindow->show();

//    if (!user->getIsLogged())
//    {
//        w_LoggingDialog * loggingWindow = w_LoggingDialog::Instance();
//        loggingWindow->setServerConnectionSettings( settCtrlr->getSettings( "server" ), true );

//        connect(loggingWindow, SIGNAL(accepted()), this, SLOT(loggingDialogAccepted()) );
//        connect(loggingWindow, SIGNAL(finished(int)), this, SLOT(loggingDialogFinished(int)) );
//        connect(loggingWindow, SIGNAL(rejected()), this, SLOT(loggingDialogRejected()) );
//        connect(loggingWindow, SIGNAL(logIn(QString,QString)), this, SLOT(logIn(QString,QString)));

//        //loggingWindow->open();
//    }


    // utworzenie sesji

    if (!sessionCtrlr->isOpened())
    {
        sessionCtrlr->resetSession();

        if(sessionCtrlr->getSessionSettingsFromServer(user->getId(), user->getName(), user->getPassword())) {
            //jesli blad
            switch (sessionCtrlr->getLastErrorCode()) {

            case 0x10:
            {
                //brak lub błędne dane użytkownika
                //logowanie
                w_LoggingDialog::Instance()->open();
                break;
            }

            default:
            {
                //inny błąd
                break;
            }

            }
        } else {
            //jesli poprawnie
        }


    }
}

void c_ClinicClient::pushTestDataToServer()
{
        w_logsWindow::Instance()->addSeparator();

        QString pwd = "kuba8888";
        QCryptographicHash hasher(QCryptographicHash::Md5);
        QString seed("klucz" + pwd);
        hasher.addData(seed.toLocal8Bit(), seed.length());

        QJsonDocument json;
        QJsonObject jobj;
        jobj["type"] = 0;
        jobj["name"] = "jszmigiel2304";
        jobj["password"] = QString(hasher.result());
        json.setObject(jobj);

        w_logsWindow::Instance()->addLog(QString::number(jobj["type"].toInt()));
        w_logsWindow::Instance()->addLog(jobj["name"].toString());
        w_logsWindow::Instance()->addLog(jobj["password"].toString());
        w_logsWindow::Instance()->addLog(hasher.result().toHex(' '));


        QByteArray data = json.toJson();

        for(int i = 0; i < 10; i++)
            data.push_back(json.toJson());

        QByteArray data2;

        QDataStream rs(&data2, QIODevice::ReadWrite);
        rs.setVersion(QDataStream::Qt_6_0);


        QString msg("Test Msg");
        quint32 threadID = 121212;
        quint32 reqType = 2;

        rs << threadID << reqType << data;

        quint64 size = data2.size();

        w_logsWindow::Instance()->addSeparator();

        w_logsWindow::Instance()->addLog(data.toHex(' '));
        w_logsWindow::Instance()->addLog(QString("/n bytes: %1").arg(data.toHex(' ').length()));

        w_logsWindow::Instance()->addSeparator();

        getConnectionCtrlr()->sendData(data2);

        c_LogsController::Instance()->saveLogToFile(QString("c_ClinicClient::pushTestDataToServer()"),
                                                    QString("%1").arg(getConnectionCtrlr()->getSocket()->socketDescriptor()),
                                                    data);
}

w_MainWindow *c_ClinicClient::getMainWindow() const
{
    return mainWindow;
}

void c_ClinicClient::setMainWindow(w_MainWindow *value)
{
    mainWindow = value;
}

c_SettingsController *c_ClinicClient::getSettCtrlr() const
{
    return settCtrlr;
}

void c_ClinicClient::setSettCtrlr(c_SettingsController *value)
{
    settCtrlr = value;
}

c_modulesController *c_ClinicClient::getModCtrlr() const
{
    return modCtrlr;
}

void c_ClinicClient::setModCtrlr(c_modulesController *value)
{
    modCtrlr = value;
}

c_connectionToServerController *c_ClinicClient::getConnectionCtrlr() const
{
    return connectionCtrlr;
}

void c_ClinicClient::setConnectionCtrlr(c_connectionToServerController *value)
{
    connectionCtrlr = value;
}

c_loggedUser *c_ClinicClient::getUser() const
{
    return user;
}

void c_ClinicClient::setUser(c_loggedUser *value)
{
    user = value;
}

c_AppTrayIcon *c_ClinicClient::getTrayIcon() const
{
    return trayIcon;
}

void c_ClinicClient::setTrayIcon(c_AppTrayIcon *value)
{
    trayIcon = value;
}

c_ThreadController *c_ClinicClient::getThreadCtrlr() const
{
    return threadCtrlr;
}

void c_ClinicClient::setThreadCtrlr(c_ThreadController *newThreadCtrlr)
{
    threadCtrlr = newThreadCtrlr;
}

void c_ClinicClient::dataReceived(quint32 data_size, QByteArray data)
{
    threadData receivedDataFromServer = parser.parseData(data_size, data);

    threadCtrlr->AddReceivedData(receivedDataFromServer);
}

void c_ClinicClient::loggingDialogAccepted()
{
}

void c_ClinicClient::loggingDialogFinished(int r)
{

}

void c_ClinicClient::loggingDialogRejected()
{

}

void c_ClinicClient::logIn(QString userName, QString userPassword)
{
    //logowanie + zaszyfrowac haslo
    QString name = userName;
    QString password = userPassword;
    QByteArray encryptedPassword;

    QByteArray encryptionSeed;
    QDataStream encryptionStream(&encryptionSeed, QIODevice::ReadWrite);
    encryptionStream.setVersion(QDataStream::Qt_6_0);
    encryptionStream << QString("test") << password;

    QCryptographicHash hasher(QCryptographicHash::Md5);
    hasher.addData(encryptionSeed);
    encryptedPassword = hasher.result();

    //tworzenie danych logowania do wysłania
    //nowy blok do wysłania
    //GET [0x03(Get Code); quint32 data_size ;0x00(Get Session UUID); quint32 - login_length;	QString login;	quint32 - password_length;	QString encrypted_password;]
    QByteArray packet;
    QByteArray attached_data;

    ThreadDestination thread_dest = SERVER;
    quint32 thread_id = QRandomGenerator::global()->generate();
    RequestType req_type = GET;
    quint32 type_flag = 0x00;

    QDataStream ds1(&attached_data, QIODevice::ReadWrite);
    ds1.setVersion(QDataStream::Qt_6_0);

    QDataStream ds2(&packet, QIODevice::ReadWrite);
    ds2.setVersion(QDataStream::Qt_6_0);

    ds1 <<  name << encryptedPassword;

    ds2 << static_cast<quint8>(thread_dest) << thread_id << static_cast<quint8>(req_type) << type_flag << attached_data;

    w_logsWindow::Instance()->addSeparator();

    w_logsWindow::Instance()->addLog(packet.toHex(' '));
    w_logsWindow::Instance()->addLog(QString("/n bytes: %1").arg(packet.toHex(' ').length()));

    w_logsWindow::Instance()->addSeparator();


    getConnectionCtrlr()->sendData(packet);

}
