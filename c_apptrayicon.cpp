#include "c_apptrayicon.h"
#include "c_clinicclient.h"

c_AppTrayIcon *c_AppTrayIcon::Instance(QObject * parent)
{
    static c_AppTrayIcon * instance = nullptr;
    if ( instance == nullptr ) {
        instance = new c_AppTrayIcon(parent);
    }
    return instance;
}

c_AppTrayIcon::~c_AppTrayIcon()
{
    this->mainWnd->deleteLater();
    this->actions->deleteLater();
    this->menu->deleteLater();

    if(this->mainWnd != nullptr)
        delete this->mainWnd;
}

void c_AppTrayIcon::createActions()
{

    closeAction = new QAction(this);
    closeAction->setEnabled(true);
    closeAction->setText(tr("Zakończ"));
    closeAction->setWhatsThis("actExit");
    closeAction->setVisible(true);
    connect(closeAction, SIGNAL(triggered(bool)), this, SLOT(closeActionTriggered(bool)));

    logOutAction = new QAction(this);
    logOutAction->setEnabled(false);
    logOutAction->setText(tr("Wyloguj"));
    logOutAction->setVisible(false);
    logOutAction->setWhatsThis("actLogOut");
    connect(logOutAction, SIGNAL(triggered(bool)), this, SLOT(logOutActionTriggered(bool)));

    logInAction = new QAction(this);
    logInAction->setEnabled(true);
    logInAction->setText(tr("Zaloguj"));
    logInAction->setVisible(true);
    logInAction->setWhatsThis("actLogIn");
    connect(logInAction, SIGNAL(triggered(bool)), this, SLOT(logInActionTriggered(bool)));

     userPanelAction = new QAction(this);
    userPanelAction->setEnabled(false);
    userPanelAction->setText(tr("[User_Name]"));
    userPanelAction->setVisible(false);
    userPanelAction->setWhatsThis("actUserPanel");
    connect(userPanelAction, SIGNAL(triggered(bool)), this, SLOT(userPanelActionTriggered(bool)));

    unlockSessionAction = new QAction(this);
   unlockSessionAction->setEnabled(false);
   unlockSessionAction->setText(tr("[Odblokuj]"));
   unlockSessionAction->setVisible(false);
   unlockSessionAction->setWhatsThis("actUnlockSession");
   connect(unlockSessionAction, SIGNAL(triggered(bool)), this, SLOT(unlockSessionActionTriggered(bool)));

    this->actions->addAction(closeAction);
    this->actions->addAction(logOutAction);
    this->actions->addAction(logInAction);
   this->actions->addAction(unlockSessionAction);
    this->actions->addAction(userPanelAction);
}

void c_AppTrayIcon::createMenu()
{
    this->menu->addAction(actions->actions()[4]);
    this->menu->addSeparator();
    this->menu->addAction(actions->actions()[3]);
    this->menu->addAction(actions->actions()[2]);
    this->menu->addAction(actions->actions()[1]);
    this->menu->addSeparator();
    this->menu->addAction(actions->actions()[0]);


    this->setContextMenu(this->menu);
}

void c_AppTrayIcon::createConnections()
{
    connect(this->mainWnd, SIGNAL(myStateChanged(Qt::WindowStates,bool)), this, SLOT(mainWindowStateChanged(Qt::WindowStates,bool)));
}


c_AppTrayIcon::c_AppTrayIcon(QObject *parent)
{
    this->setParent(parent);

    this->mainWnd = w_MainWindow::Instance();
    this->mainWnd->loadSettings( ( (dynamic_cast<c_ClinicClient *>(parent) )->getSettCtrlr())->getSettings("window") );
    this->mainWnd->AddWatchedObject("modulesController", dynamic_cast<i_Watched *>( (dynamic_cast<c_ClinicClient *>(parent) )->getModCtrlr() ) );

    this->actions = new QActionGroup(this);
    this->menu = new QMenu();


    QIcon icon(":/icons/appIcon.png");
    this->setIcon(icon);

    createActions();
    createMenu();
    createConnections();
}



w_MainWindow *c_AppTrayIcon::getMainWnd() const
{
    return mainWnd;
}

void c_AppTrayIcon::setMainWnd(w_MainWindow *value)
{
    mainWnd = value;
}

void c_AppTrayIcon::actionTriggered(QAction *)
{
}

void c_AppTrayIcon::closeActionTriggered(bool)
{
    emit closeActionSignal();
}


void c_AppTrayIcon::logInActionTriggered(bool)
{
    emit logInActionSignal();
}

void c_AppTrayIcon::logOutActionTriggered(bool)
{
    emit logOutActionSignal();
}

void c_AppTrayIcon::userPanelActionTriggered(bool)
{
    emit userPanelActionSignal();
}

void c_AppTrayIcon::unlockSessionActionTriggered(bool)
{
    emit unlockSessionActionSignal(this->username);
}


void c_AppTrayIcon::mainWindowStateChanged(Qt::WindowStates states)
{
    if(states.testFlag(Qt::WindowNoState))
    {
        QIcon icon(":/icons/appIcon.png");
        //hideAllWindows();
        this->showMessage(qApp->applicationName(), tr("Zminializowano do zasobnika systemowego"), icon, 2000);
    }
}

void c_AppTrayIcon::mainWindowStateChanged(Qt::WindowStates states, bool appIsClosing)
{
    if(states.testFlag(Qt::WindowNoState) && !appIsClosing)
    {
        QIcon icon(":/icons/appIcon.png");
        this->showMessage(qApp->applicationName(), tr("Zminializowano do zasobnika systemowego"), icon, 2000);
    } else if(states.testFlag(Qt::WindowNoState) && appIsClosing)
    {
        QIcon icon(":/icons/appIcon.png");
        this->showMessage(qApp->applicationName(), tr("Zamykam aplikację"), icon, 2000);
    }

}

void c_AppTrayIcon::userLogged(QString name, QString role)
{
    logInAction->setEnabled(false);
    logInAction->setVisible(false);

    logOutAction->setEnabled(true);
    logOutAction->setVisible(true);

    userPanelAction->setEnabled(true);
    userPanelAction->setVisible(true);
    userPanelAction->setText(QString("%1 [%2]").arg(name, role));

    unlockSessionAction->setEnabled(false);
    unlockSessionAction->setVisible(false);
}

void c_AppTrayIcon::userNotLogged()
{
    logInAction->setEnabled(true);
    logInAction->setVisible(true);

    logOutAction->setEnabled(false);
    logOutAction->setVisible(false);

    userPanelAction->setEnabled(false);
    userPanelAction->setVisible(false);
    userPanelAction->setText(QString("[User_name]"));

    unlockSessionAction->setEnabled(false);
    unlockSessionAction->setVisible(false);

}

void c_AppTrayIcon::appLocked(QString name)
{
    this->username = name;

    logInAction->setEnabled(false);
    logInAction->setVisible(false);

    logOutAction->setEnabled(false);
    logOutAction->setVisible(false);

    userPanelAction->setEnabled(false);
    userPanelAction->setVisible(true);
    unlockSessionAction->setEnabled(true);
    unlockSessionAction->setVisible(true);
}

void c_AppTrayIcon::appUnlocked()
{
    logInAction->setEnabled(false);
    logInAction->setVisible(false);

    logOutAction->setEnabled(true);
    logOutAction->setVisible(true);

    userPanelAction->setEnabled(true);
    userPanelAction->setVisible(true);

    unlockSessionAction->setEnabled(false);
    unlockSessionAction->setVisible(false);
}
