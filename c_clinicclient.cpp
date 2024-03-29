#include "c_clinicclient.h"
#include "m_loggeduser.h"

c_ClinicClient::c_ClinicClient(QObject *parent)
{
    setParent(parent);
    connect(qApp, SIGNAL(aboutToQuit()), SLOT(deleteLater()));    

    qRegisterMetaType<myStructures::packet>("packet");
    qRegisterMetaType<myStructures::threadData>("threadData");
    qRegisterMetaType<qintptr>("qintptr");

    //----------------------------------------//
    settCtrlr = new c_SettingsController();
    modCtrlr = new c_modulesController();

    connectionCtrlr = new c_connectionToServerController();
    user = new c_loggedUser();
    sessionCtrlr = new c_SessionController();
    processCtrlr = new c_processesController();

    sessionCtrlr->setLoggedUser(user);

    //----------------------------------------//



    //----------------------------------------//

    connect(this, SIGNAL(replyReceived(QByteArray)), connectionCtrlr, SLOT(replyReceivedRemoveFromList(QByteArray)), Qt::QueuedConnection );
    connect(processCtrlr->thread(), SIGNAL(passDataToClinicClient(quint64, QByteArray, qintptr)), this, SLOT(dataReceived(quint64, QByteArray, qintptr)), Qt::QueuedConnection);
    connect(this, SIGNAL(sendToServer(packet)), connectionCtrlr, SLOT(passDataToBuffer(packet)), Qt::QueuedConnection );
    connect(this, SIGNAL(setUpConnection(QMap<QString, QVariant>)), connectionCtrlr, SLOT(setConnection(QMap<QString, QVariant>)), Qt::DirectConnection);

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
    QMetaObject::invokeMethod(this, "cleanup");
}

void c_ClinicClient::run()
{    
    prepareModules();

    refreshWindows(true);
    createConnections();

    user->setUpThread();
    sessionCtrlr->setUpThread();
    processCtrlr->setUpThread();
    emit setUpConnection(settCtrlr->getSettings("server"));

    connectionCtrlr->start();
    user->thread()->start();
    sessionCtrlr->thread()->start();    
    processCtrlr->thread()->start();
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

c_SessionController *c_ClinicClient::getSessionCtrlr() const
{
    return sessionCtrlr;
}

void c_ClinicClient::setSessionCtrlr(c_SessionController *newSessionCtrlr)
{
    sessionCtrlr = newSessionCtrlr;
}

void c_ClinicClient::refreshWindows(bool show)
{
    mainWindow->refresh();

    if(show) {
        mainWindow->show();
        trayIcon->show();
    }
}

void c_ClinicClient::prepareModules()
{
    QStringList modulesNames;

    QString modulesString = this->settCtrlr->getSettings("application")["modules"].toString();
    QStringList modules = modulesString.split(','/*, QString::SkipEmptyParts*/);
    foreach (QString str1, modules) {
        modulesNames << str1;
    }

    this->modCtrlr->LoadModules(modulesNames);
}

void c_ClinicClient::createConnections()
{
    connect(this, SIGNAL(idleSignalReceived()), sessionCtrlr->thread(), SLOT(appIDLEdetected()), Qt::DirectConnection);

    connect( connectionCtrlr, SIGNAL(dataReceived(quint64, QByteArray, qintptr)), this, SLOT(dataReceived(quint64, QByteArray, qintptr)) );
    connect( connectionCtrlr, SIGNAL(threadAssigned(MyThread *)), threadCtrlr, SLOT(newThread(MyThread *)) );

    connect(user, SIGNAL( threadAssigned(MyThread *) ), threadCtrlr, SLOT( newThread(MyThread *) ) );
    connect(user->thread(), SIGNAL(sendToServer(packet)), connectionCtrlr, SLOT(passDataToBuffer(packet)), Qt::QueuedConnection);

    connect(sessionCtrlr->thread(), SIGNAL(sendToServer(packet)), connectionCtrlr, SLOT(passDataToBuffer(packet)), Qt::QueuedConnection);
    connect(sessionCtrlr, SIGNAL( threadAssigned(MyThread *) ), threadCtrlr, SLOT( newThread(MyThread *) ) );

    connect(user->thread(), SIGNAL(userLogged(QString, QString)), mainWindow->getUserPanel(), SLOT(userLogged(QString, QString)));
    connect(user->thread(), SIGNAL(userNotLogged()), mainWindow->getUserPanel(), SLOT(userNotLogged()));
    connect(user->thread(), SIGNAL(userAuthorizationDataNeeded()), this, SLOT(showLoggingDialog()), Qt::UniqueConnection);
    connect(user->thread(), SIGNAL(userAuthorizationDataNeededToUnlock(QString)), this->mainWindow, SLOT(IdleDetected(QString)), Qt::QueuedConnection);
    connect(user->thread(), SIGNAL(userAuthorizationDataNeededToUnlock(QString)), this, SLOT(showAuthorizationDialogOnIdle(QString)), Qt::UniqueConnection);

    connect(this->mainWindow, SIGNAL(logOutModuleClicked(qint32, QString, QString)), user->thread(), SLOT(logOut(qint32, QString, QString)));
    connect(this->mainWindow, SIGNAL(settingsWindowModuleClicked()), this, SLOT(showSettingsWindow()));
    connect(this->mainWindow, SIGNAL(loggingWindowModuleClicked()), this, SLOT(showLoggingDialog()));
    connect(this->mainWindow, SIGNAL(closeApplicationButtonClicked()), this, SLOT(closeApplication()));
    connect(this->mainWindow, SIGNAL(processAppButtonClicked(QString, QMap<QString, QString>)), this, SLOT(processApp(QString, QMap<QString, QString>)));

    connect(qApp, SIGNAL(sessionTimeExpireChanged(QTime)), this->mainWindow->getUserPanel(), SLOT(sessionTimeChanged(QTime)));
    connect(sessionCtrlr->thread(), SIGNAL(idleDetected()), user->thread(), SLOT(sessionLocked()));
    connect(connectionCtrlr, SIGNAL(connectionToServerStateChanged(QAbstractSocket::SocketState)), mainWindow->getUserPanel(), SLOT(connectionToServerStatusChanged(QAbstractSocket::SocketState)));
    connect( connectionCtrlr, SIGNAL(socketDisconnectedLogOutUser()), user->thread(), SLOT(logOutOffLine()), Qt::DirectConnection);

    connect(sessionCtrlr->thread(), SIGNAL(processingError(QString)), this->mainWindow, SLOT(ProcessingFault(QString)));
    connect(sessionCtrlr->thread(), SIGNAL(processingStarted(QString)), this->mainWindow, SLOT(Processing(QString)));
    connect(sessionCtrlr->thread(), SIGNAL(processingFinished()), this->mainWindow, SLOT(ProcessingFinished()));

    connect(user->thread(), SIGNAL(logInError(QString)), this->mainWindow, SLOT(ProcessingFault(QString)));
    connect(user->thread(), SIGNAL(logInProcessing(QString)), this->mainWindow, SLOT(Processing(QString)));
    connect(user->thread(), SIGNAL(logInFinished()), this->mainWindow, SLOT(ProcessingFinished()));

    connect(this, SIGNAL(stopAllThreads()), this->getThreadCtrlr(), SLOT(stopAllThreads()), Qt::DirectConnection);
    connect(this->getThreadCtrlr(), SIGNAL(forceLogOutUser()), user, SLOT(forceLogOut()), Qt::DirectConnection);

    connect(this, SIGNAL(packetReceiveConfirmationReceived(threadData)), connectionCtrlr, SLOT(receiveConfirmationReceived(threadData)), Qt::DirectConnection);

    connect( this->getTrayIcon(), SIGNAL(closeActionSignal()), this, SLOT(closeApplication()) );
    connect( this->getTrayIcon(), SIGNAL(logInActionSignal()), this, SLOT(showLoggingDialog()) );
    connect( this->getTrayIcon(), SIGNAL(logOutActionSignal(qint32, QString, QString)), user->thread(), SLOT(logOut(qint32, QString, QString)) );
    connect( this->getTrayIcon(), SIGNAL(userPanelActionSignal()), this, SLOT(showUserPanelWindow()) );
    connect( this->getTrayIcon(), SIGNAL(unlockSessionActionSignal(QString)), this, SLOT(showAuthorizationDialogOnIdle(QString)) );

    connect(user->thread(), SIGNAL(userLogged(QString, QString)), this->getTrayIcon(), SLOT(userLogged(QString, QString)));
    connect(user->thread(), SIGNAL(userLogged(QString, QString)), sessionCtrlr->thread(), SLOT(configureSession(QString, QString)));
    connect(user->thread(), SIGNAL(userNotLogged()), this->getTrayIcon(), SLOT(userNotLogged()));
    connect(user->thread(), SIGNAL(userAuthorizationDataNeededToUnlock(QString)), this->getTrayIcon(), SLOT(appLocked(QString)));
    connect(user->thread(), SIGNAL(userLogged(QString, QString)), this->mainWindow, SLOT(unLockModulesScrollArea(QString, QString)));
    connect(user->thread(), SIGNAL(userNotLogged()), this->mainWindow, SLOT(lockModulesScrollArea()));
    connect(user->thread(), SIGNAL(userNotLogged()), this->mainWindow, SLOT(closeAllAddWindows()));
    connect(user->thread(), SIGNAL(userNotLogged()), processCtrlr, SLOT(removeAllModuleProcesses()));

    connect(user->thread(), SIGNAL(aboutToLogOut()), sessionCtrlr->thread(), SLOT(sessionClose()), Qt::DirectConnection);

    connect(user->thread(), SIGNAL(unlockSession()), this, SLOT(sessionUnlocked()));
    connect(user->thread(), SIGNAL(unlockSession()), sessionCtrlr->thread(), SLOT(sessionUnlockConfirmationReceived()));
    connect( this, SIGNAL(unlockSessionSignal()), this->mainWindow, SLOT(unlockWindow()) );
    connect( this, SIGNAL(unlockSessionSignal()), this->getTrayIcon(), SLOT(appUnlocked()) );
    connect( this, SIGNAL(logOutUserBeforeCloseApp(qint32, QString, QString)), user->thread(), SLOT(logOut(qint32, QString, QString)), Qt::DirectConnection );

    connect( this->mainWindow, SIGNAL(userProfileButtonClicked()), this, SLOT(showUserPanelWindow()));

    connect(processCtrlr, SIGNAL(threadAssigned(MyThread *)), threadCtrlr, SLOT(newThread(MyThread *)) );
}

c_ThreadController *c_ClinicClient::getThreadCtrlr() const
{
    return threadCtrlr;
}

void c_ClinicClient::setThreadCtrlr(c_ThreadController *newThreadCtrlr)
{
    threadCtrlr = newThreadCtrlr;
}

void c_ClinicClient::cleanup()
{
    delete settCtrlr;
    sessionCtrlr->deleteLater();
    user->deleteLater();
    connectionCtrlr->deleteLater();
    processCtrlr->deleteLater();

    qApp->exit(0);
}

void c_ClinicClient::showSettingsWindow()
{
    w_SettingsWindow::Instance()->show();    
    mainWindow->newAddWindow(w_SettingsWindow::Instance());
    connect(w_SettingsWindow::Instance(), SIGNAL(aboutToClose(QWidget *)), mainWindow, SLOT(removeAddWindow(QWidget *)));
}

void c_ClinicClient::showLoggingDialog()
{
    w_LoggingDialog *dialog = w_LoggingDialog::Instance();

    dialog->setServerConnectionSettings( settCtrlr->getSettings( "server" ), true);

    disconnect(dialog, SIGNAL(getUserId(QString, QString)), nullptr, nullptr);
    disconnect(dialog, SIGNAL(getUserIdOnIdle(QString, QString)), nullptr, nullptr);
    disconnect(user->thread(), SIGNAL(logInProcessing(QString)), nullptr, nullptr);
    disconnect(user->thread(), SIGNAL(logInFinished()), nullptr, nullptr);
    disconnect(user->thread(), SIGNAL(logInError(QString)), nullptr, nullptr);

    connect(dialog, SIGNAL(getUserId(QString, QString)), user->thread(), SLOT(getUserId(QString, QString)));
    connect(user->thread(), SIGNAL(logInProcessing(QString)), dialog, SLOT(logInProcessing(QString)));
    connect(user->thread(), SIGNAL(logInFinished()), dialog, SLOT(logInFinished()));
    connect(user->thread(), SIGNAL(logInError(QString)), dialog, SLOT(logInFault(QString)));

    dialog->open();
}

void c_ClinicClient::showAuthorizationDialogOnIdle(QString username)
{
    w_AuthorizationDialog *dialog = new w_AuthorizationDialog();
    dialog->setUsernameLabel(username);

    connect(dialog, SIGNAL(processRequest(QString, QString)), user->thread(), SLOT(unlockOnIdle(QString, QString)));
    connect(user->thread(), SIGNAL(logInProcessing(QString)), dialog, SLOT(authorizationProcessing(QString)));
    connect(user->thread(), SIGNAL(logInFinished()), dialog, SLOT(authorizationFinished()));
    connect(user->thread(), SIGNAL(logInError(QString)), dialog, SLOT(authorizationFault(QString)));

    dialog->open();
}

void c_ClinicClient::showUserPanelWindow()
{
    w_UserProfileWindow * userProfileWindow = new w_UserProfileWindow();
    mainWindow->newAddWindow(userProfileWindow);
    connect(userProfileWindow, SIGNAL(aboutToClose(QWidget *)), mainWindow, SLOT(removeAddWindow(QWidget *)));
    userProfileWindow->show();

    connect( userProfileWindow, SIGNAL(getUserPanelProperties(QMap<QString, QVariant> *, QMap<QString, QVariant> *, QList<myStructures::myLog> * )),
             user->thread(), SLOT(getProperties(QMap<QString, QVariant> *, QMap<QString, QVariant> *, QList<myStructures::myLog> * )), Qt::DirectConnection );


    userProfileWindow->setUpLoop();

    connect(userProfileWindow->getWaitingLoop(), SIGNAL(loopStarted(QString)), userProfileWindow, SLOT(processing(QString)));
    connect(userProfileWindow->getWaitingLoop(), SIGNAL(exitLoop(int)), userProfileWindow, SLOT(processingFinished(int)));

    if(this->user->getId() == 0 || this->user->getName().isEmpty() || this->user->getPassword().isEmpty()) {
        connect( this->user, SIGNAL(propertiesSaved()), userProfileWindow->getWaitingLoop()->newCondition(), SLOT(conditionFulfilled()) );
        connect(this->user, SIGNAL(passProperties(QMap<QString, QVariant>)), userProfileWindow, SLOT(setUserProperties(QMap<QString, QVariant>)));
    }
    if(this->user->getEmployee()->getId() == 0 || this->user->getEmployee()->getName().isEmpty()) {
        connect( this->user->getEmployee(), SIGNAL(propertiesSaved()), userProfileWindow->getWaitingLoop()->newCondition(), SLOT(conditionFulfilled()) );
        connect(this->user->getEmployee(), SIGNAL(passProperties(QMap<QString, QVariant>)), userProfileWindow, SLOT(setEmployeeProperties(QMap<QString, QVariant>)));
    }
    if(this->user->getDbLogs()->isEmpty()) {
        connect( this->user, SIGNAL(logsSaved()), userProfileWindow->getWaitingLoop()->newCondition(), SLOT(conditionFulfilled()) );
        connect(this->user, SIGNAL(passLogs(QList<myStructures::myLog>)), userProfileWindow, SLOT(setLogs(QList<myStructures::myLog>)));
    }

    userProfileWindow->refreshProperties();

    userProfileWindow->refresh();
}


void c_ClinicClient::closeApplication()
{
    if(user->getIsLogged())
    {
        QTimer timer;
        timer.setSingleShot(true);
        QEventLoop loop;
        connect( this->user->thread(), SIGNAL(userNotLogged()), &loop, SLOT(quit()) );
        connect( &timer, &QTimer::timeout, &loop, &QEventLoop::quit );
        timer.start(3000);
        emit this->logOutUserBeforeCloseApp(this->user->getId(),
                                            this->user->getName(),
                                            this->user->getEncryptedPassword(false, true));
        loop.exec();


        if(timer.isActive()) {
            // komunikat, wylogowanie przed zamknięciem
        }
        else{
            emit user->forceLogOut();
        }
    }

    emit stopAllThreads();
    qApp->closeAllWindows();
}

void c_ClinicClient::applicatioIdleDetected()
{
    emit idleSignalReceived();
}

void c_ClinicClient::sessionUnlocked()
{
    emit unlockSessionSignal();
}

void c_ClinicClient::processApp(QString target, QMap<QString, QString> parameters)
{
    if(target.isEmpty() || parameters["app_path"].isEmpty()) return;

    c_moduleProcess * process = new c_moduleProcess(this);
    process->setModuleProcessName( QString("%1%2").arg(target, QString("%1").arg(QRandomGenerator::global()->bounded(1000,9999))) );
    process->setThreadId( threadCtrlr->generateThreadId() );

    connect(process, SIGNAL(passModuleProcessToController(c_moduleProcess *)), processCtrlr, SLOT(newModuleProcess(c_moduleProcess *)));
    connect(process, SIGNAL(moduleClosed(c_moduleProcess *, int, QProcess::ExitStatus)), processCtrlr, SLOT(moduleClosed(c_moduleProcess *, int, QProcess::ExitStatus)));

    QStringList arguments;
    arguments.append( QString("MyArgServerName=%1").arg( processCtrlr->getHashServerName()) );
    arguments.append( QString("MyArgModuleName=%1").arg( process->getModuleProcessNameHash() ) );
    arguments.append( QString("MyArgThreadId=%1").arg( process->getThreadId() ) );

    foreach (const QString parameter, parameters.keys()) {
        if(parameter == "app_path") { process->setProgram( parameters["app_path"] ); }
        if(parameter == "userId") {
            if(parameters["userId"] == "CURRENT_LOGGED_USER_ID")
                arguments.append(QString("MyArgUserId=%1").arg(this->user->getId()));
            else
                arguments.append(QString("MyArgUserId=%1").arg(parameters["userId"]));
        }

        if(parameter == "userName") {
            if(parameters["userName"] == "CURRENT_LOGGED_USER_NAME")
                arguments.append(QString("MyArgUserName=%1").arg(this->user->getName()));
            else
                arguments.append(QString("MyArgUserName=%1").arg(parameters["userName"]));
        }


    }

    process->insertMapInArgsList(user->getUserProperties(), &arguments);

    process->setArguments(arguments);
    process->start();

    if( process->waitForStarted(-1) ) {
        emit process->passModuleProcessToController(process);
    } else {
        return;
    }
}


void c_ClinicClient::dataReceived(quint64 data_size, QByteArray data, qintptr socketDescriptor)
{
    c_Parser parser;
    QPair<QByteArray, QByteArray> receivedDataFromServer = parser.parseData(data_size, data);
    myStructures::threadData attchedData;
    parser.parseJson( &receivedDataFromServer.second, &attchedData );

    switch( static_cast<qint8>(attchedData.thread_dest) ) {
    case myTypes::SERVER: {
        parser.insertAuthDataInJson(&receivedDataFromServer.second, this->user->getId(), this->user->getName(), this->user->getPassword());

        QByteArray packet;

        QJsonDocument jsonPacket = QJsonDocument::fromJson(receivedDataFromServer.second);
        QByteArray JsonMD5 = parser.getJsonMD5Hash(jsonPacket);

        QDataStream ds2(&packet, QIODevice::ReadWrite);
        ds2.setVersion(QDataStream::Qt_6_0);

        ds2 << JsonMD5.toHex() << jsonPacket.toJson();

        QPair<QByteArray, QByteArray> pair(JsonMD5.toHex(), packet);

        myStructures::packet packetToSend;
        packetToSend.md5_hash = pair.first;
        packetToSend.packet_to_send = pair.second;
        packetToSend.wait_for_reply = true;

        emit sendToServer(packetToSend);
        break;
    }
    case myTypes::CLINIC_MODULE: {
        emit replyReceived( attchedData.ref_md5 );
        processCtrlr->thread()->processData(attchedData, socketDescriptor);
        break;
    }
    default: {
        if(attchedData.content == myTypes::PACKET_RECEIVE_CONFIRMATION)
            emit packetReceiveConfirmationReceived(attchedData);
        else {
            emit replyReceived( attchedData.ref_md5 );
            emit passDataToThread(attchedData, socketDescriptor);
        }
        break;
    }
    }
}



