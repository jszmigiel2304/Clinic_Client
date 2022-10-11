#include "c_clinicclient.h"

c_ClinicClient::c_ClinicClient(QObject *parent)
{
    setParent(parent);
    connect(qApp, SIGNAL(aboutToQuit()), SLOT(deleteLater()));    

    qRegisterMetaType<packet>("packet");
    qRegisterMetaType<threadData>("threadData");

    //----------------------------------------//
    settCtrlr = new c_SettingsController();
    modCtrlr = new c_modulesController();

    connectionCtrlr = new c_connectionToServerController();
    user = new c_loggedUser();
    sessionCtrlr = new c_SessionController();

    sessionCtrlr->setLoggedUser(user);

    //----------------------------------------//



    //----------------------------------------//

    connect(this, SIGNAL(replyReceived(QByteArray)), connectionCtrlr, SLOT(replyReceivedRemoveFromList(QByteArray)), Qt::QueuedConnection );
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

    user->setLogs(this->logsWindow);
    refreshWindows(true);
    createConnections();

    user->setUpThread();
    sessionCtrlr->setUpThread();
    emit setUpConnection(settCtrlr->getSettings("server"));



    connectionCtrlr->start();
    user->thread()->start();
    sessionCtrlr->thread()->start();

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

w_ThreadsListWindow *c_ClinicClient::getThreadsListWindow() const
{
    return threadsListWindow;
}

void c_ClinicClient::setThreadsListWindow(w_ThreadsListWindow *newThreadsListWindow)
{
    threadsListWindow = newThreadsListWindow;
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
    threadsListWindow->refresh();

    if(show) {
        mainWindow->show();
        trayIcon->show();
        logsWindow->show();
        threadsListWindow->show();
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
    connect(this, SIGNAL(newLog(QString)), logsWindow, SLOT(addLog(QString)) );
    connect(this, SIGNAL(idleSignalReceived()), sessionCtrlr->thread(), SLOT(appIDLEdetected()), Qt::DirectConnection);

    connect( connectionCtrlr, SIGNAL(dataReceived(quint64, QByteArray)), this, SLOT(dataReceived(quint64, QByteArray)) );
    connect( connectionCtrlr, SIGNAL(newLog(QString)), logsWindow, SLOT(addLog(QString)) );
    connect( connectionCtrlr, SIGNAL(threadAssigned(MyThread *)), threadCtrlr, SLOT(newThread(MyThread *)) );

    connect( user, SIGNAL(newLog(QString)), logsWindow, SLOT(addLog(QString)) );
    connect( sessionCtrlr, SIGNAL(newLog(QString)), logsWindow, SLOT(addLog(QString)) );
    connect(user, SIGNAL( threadAssigned(MyThread *) ), threadCtrlr, SLOT( newThread(MyThread *) ) );
    connect(user->thread(), SIGNAL(sendToServer(packet)), connectionCtrlr, SLOT(passDataToBuffer(packet)), Qt::QueuedConnection);

    connect(sessionCtrlr->thread(), SIGNAL(sendToServer(packet)), connectionCtrlr, SLOT(passDataToBuffer(packet)), Qt::QueuedConnection);
    connect(sessionCtrlr, SIGNAL( threadAssigned(MyThread *) ), threadCtrlr, SLOT( newThread(MyThread *) ) );

    connect(user->thread(), SIGNAL(userLogged(QString, UserRole)), mainWindow->getUserPanel(), SLOT(userLogged(QString, UserRole)));
    connect(user->thread(), SIGNAL(userNotLogged()), mainWindow->getUserPanel(), SLOT(userNotLogged()));
    connect(user->thread(), SIGNAL(userAuthorizationDataNeeded()), this, SLOT(showLoggingDialog()), Qt::UniqueConnection);
    connect(user->thread(), SIGNAL(userAuthorizationDataNeededToUnlock(QString)), this->mainWindow, SLOT(IdleDetected(QString)), Qt::QueuedConnection);
    connect(user->thread(), SIGNAL(userAuthorizationDataNeededToUnlock(QString)), this, SLOT(showAuthorizationDialogOnIdle(QString)), Qt::UniqueConnection);

    connect(this->mainWindow, SIGNAL(logOutModuleClicked(qint32, QString, QString)), user->thread(), SLOT(logOut(qint32, QString, QString)));
    connect(this->mainWindow, SIGNAL(settingsWindowModuleClicked()), this, SLOT(showSettingsWindow()));
    connect(this->mainWindow, SIGNAL(loggingWindowModuleClicked()), this, SLOT(showLoggingDialog()));
    connect(this->mainWindow, SIGNAL(closeApplicationButtonClicked()), this, SLOT(closeApplication()));

    connect(qApp, SIGNAL(sessionTimeExpireChanged(QTime)), this->mainWindow->getUserPanel(), SLOT(sessionTimeChanged(QTime)));
//    connect(sessionCtrlr->thread(), SIGNAL(sessionTimeExpireChanged(QTime)), this->mainWindow->getUserPanel(), SLOT(sessionTimeChanged(QTime)));
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

    connect(user->thread(), SIGNAL(userLogged(QString, UserRole)), this->getTrayIcon(), SLOT(userLogged(QString, UserRole)));
    connect(user->thread(), SIGNAL(userLogged(QString, UserRole)), sessionCtrlr->thread(), SLOT(configureSession(QString, UserRole)));
    connect(user->thread(), SIGNAL(userNotLogged()), this->getTrayIcon(), SLOT(userNotLogged()));
    connect(user->thread(), SIGNAL(userAuthorizationDataNeededToUnlock(QString)), this->getTrayIcon(), SLOT(appLocked(QString)));

    connect(user->thread(), SIGNAL(aboutToLogOut()), sessionCtrlr->thread(), SLOT(sessionClose()), Qt::DirectConnection);

    //connect(sessionCtrlr->thread(), SIGNAL(sessionUnlocked()), this, SLOT(sessionUnlocked()) );
    connect(user->thread(), SIGNAL(unlockSession()), this, SLOT(sessionUnlocked()));
    connect(user->thread(), SIGNAL(unlockSession()), sessionCtrlr->thread(), SLOT(sessionUnlockConfirmationReceived()));
    connect( this, SIGNAL(unlockSessionSignal()), this->mainWindow, SLOT(unlockWindow()) );
    connect( this, SIGNAL(unlockSessionSignal()), this->getTrayIcon(), SLOT(appUnlocked()) );
    connect( this, SIGNAL(logOutUserBeforeCloseApp(qint32, QString, QString)), user->thread(), SLOT(logOut(qint32, QString, QString)), Qt::DirectConnection );
}

w_logsWindow *c_ClinicClient::getLogsWindow() const
{
    return logsWindow;
}

void c_ClinicClient::setLogsWindow(w_logsWindow *newLogsWindow)
{
    logsWindow = newLogsWindow;
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

    qApp->exit(0);
}

void c_ClinicClient::showSettingsWindow()
{
    w_SettingsWindow::Instance()->show();
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
}

void c_ClinicClient::closeApplication()
{
    {
        QTimer timer;
        timer.setSingleShot(true);
        QEventLoop loop;
        connect( this->user->thread(), SIGNAL(userNotLogged()), &loop, SLOT(quit()) );
        connect( &timer, &QTimer::timeout, &loop, &QEventLoop::quit );
        timer.start(3000);
        emit this->logOutUserBeforeCloseApp(this->user->getId(),
                                            this->user->getName(),
                                            this->user->getEncryptedPassword());
        loop.exec();


        if(timer.isActive()) {
            emit newLog(QString("Wylogowano przed zamknięciem.\n"));
        }
        else{
            emit newLog(QString("Błąd wylogowania.\n"));
            emit user->forceLogOut();
        }
    }

    emit stopAllThreads();
    qApp->closeAllWindows();
}

void c_ClinicClient::applicatioIdleDetected()
{
    emit newLog(QString("IDLE from myApplication\n"));
    emit idleSignalReceived();
}

void c_ClinicClient::sessionUnlocked()
{
    emit newLog(QString("Session Unlocked\n"));
    emit unlockSessionSignal();
}


void c_ClinicClient::dataReceived(quint64 data_size, QByteArray data)
{
    c_Parser parser;
    QPair<QByteArray, QByteArray> receivedDataFromServer = parser.parseData(data_size, data);
    threadData attchedData;
    parser.parseJson( &receivedDataFromServer.second, &attchedData );

    if(attchedData.content == PACKET_RECEIVE_CONFIRMATION)
        emit packetReceiveConfirmationReceived(attchedData);
    else {
        emit replyReceived( attchedData.ref_md5 );
        emit passDataToThread(attchedData);
    }
}



