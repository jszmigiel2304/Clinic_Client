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
    QAction * pushDataToServerAction = new QAction(this);
    pushDataToServerAction->setEnabled(true);
    pushDataToServerAction->setText(tr("Push Data to Serever"));
    pushDataToServerAction->setWhatsThis("actPushData");
    pushDataToServerAction->setVisible(true);
    connect(pushDataToServerAction, SIGNAL(triggered(bool)), this, SLOT(pushDataToServerActionTriggered(bool)));

    QAction * closeAction = new QAction(this);
    closeAction->setEnabled(true);
    closeAction->setText(tr("Zakończ"));
    closeAction->setWhatsThis("actExit");
    closeAction->setVisible(true);
    connect(closeAction, SIGNAL(triggered(bool)), this, SLOT(closeActionTriggered(bool)));

    QAction * raiseAction = new QAction(this);
    raiseAction->setEnabled(true);
    raiseAction->setText(tr("Pokaż okna"));
    raiseAction->setVisible(false);
    raiseAction->setWhatsThis("actRaise");
    connect(raiseAction, SIGNAL(triggered(bool)), this, SLOT(raiseActionTriggered(bool)));

    QAction * hideAction = new QAction(this);
    hideAction->setEnabled(true);
    hideAction->setText(tr("Ukryj okna"));
    hideAction->setVisible(true);
    hideAction->setWhatsThis("actHide");
    connect(hideAction, SIGNAL(triggered(bool)), this, SLOT(hideActionTriggered(bool)));

    QAction * logOutAction = new QAction(this);
    logOutAction->setEnabled(true);
    logOutAction->setText(tr("Wyloguj"));
    logOutAction->setVisible(false);
    logOutAction->setWhatsThis("actLogOut");
    connect(logOutAction, SIGNAL(triggered(bool)), this, SLOT(logOutActionTriggered(bool)));

    QAction * logInAction = new QAction(this);
    logInAction->setEnabled(true);
    logInAction->setText(tr("Zaloguj się"));
    logInAction->setVisible(true);
    logInAction->setWhatsThis("actLogIn");
    connect(logInAction, SIGNAL(triggered(bool)), this, SLOT(logInActionTriggered(bool)));


    this->actions->addAction(closeAction);
    this->actions->addAction(pushDataToServerAction);
    this->actions->addAction(raiseAction);
    this->actions->addAction(hideAction);
    this->actions->addAction(logOutAction);
    this->actions->addAction(logInAction);
}

void c_AppTrayIcon::createMenu()
{
    this->menu->addSeparator();
    this->menu->addAction(actions->actions()[2]);
    this->menu->addAction(actions->actions()[3]);
    this->menu->addSeparator();
    this->menu->addAction(actions->actions()[4]);
    this->menu->addAction(actions->actions()[5]);
    this->menu->addSeparator();
    this->menu->addAction(actions->actions()[1]);
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

void c_AppTrayIcon::hideAllWindows()
{
    if(mainWnd != nullptr )
        mainWnd->hide();

    if(w_logsWindow::Instance() != nullptr)
        w_logsWindow::Instance()->hide();

    if(w_LoggingDialog::Instance() != nullptr)
        w_LoggingDialog::Instance()->hide();

    w_logsWindow::Instance()->addLog("All windows minimized to tray. \n");
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
    this->mainWnd->close();

    qApp->exit(0);
}

void c_AppTrayIcon::raiseActionTriggered(bool)
{

}

void c_AppTrayIcon::hideActionTriggered(bool)
{

}

void c_AppTrayIcon::logInActionTriggered(bool)
{

}

void c_AppTrayIcon::logOutActionTriggered(bool)
{

}

void c_AppTrayIcon::pushDataToServerActionTriggered(bool)
{
    ( dynamic_cast<c_ClinicClient *>(parent())->pushTestDataToServer() );
}


void c_AppTrayIcon::mainWindowStateChanged(Qt::WindowStates states)
{
    if(states.testFlag(Qt::WindowNoState))
    {
        QIcon icon(":/icons/appIcon.png");
        hideAllWindows();
        this->showMessage(qApp->applicationName(), tr("Zminializowano do zasobnika systemowego"), icon, 2000);
    }
}

void c_AppTrayIcon::mainWindowStateChanged(Qt::WindowStates states, bool appIsClosing)
{
    if(states.testFlag(Qt::WindowNoState) && !appIsClosing)
    {
        QIcon icon(":/icons/appIcon.png");
        hideAllWindows();
        this->showMessage(qApp->applicationName(), tr("Zminializowano do zasobnika systemowego"), icon, 2000);
    } else if(states.testFlag(Qt::WindowNoState) && appIsClosing)
    {
        QIcon icon(":/icons/appIcon.png");
        this->showMessage(qApp->applicationName(), tr("Zamykam aplikację"), icon, 2000);
    }

}
