#include "w_userpanel.h"
#include "c_apptrayicon.h"
#include "c_clinicclient.h"

w_userPanel::w_userPanel(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    setFont(QFont("Segoe", 8, QFont::Bold));
    createLayout();

}

w_userPanel::~w_userPanel()
{
    loggedUserName->deleteLater();
    logStateButton->deleteLater();
    connectionStateButton->deleteLater();
    controlButton->deleteLater();
    closeButton->deleteLater();
    sessionTimeOutLabel->deleteLater();

    contentLayout->deleteLater();
}

void w_userPanel::refresh()
{
    createLayout();
}

bool w_userPanel::eventFilter(QObject *, QEvent *evt)
{
    if (evt->type() == QEvent::Wheel)
    {
        evt->accept();
    } else if (evt->type() == QEvent::Enter)
    {
        evt->accept();
    } else if (evt->type() == QEvent::Leave)
    {
        evt->accept();
    }



    return false;
}

void w_userPanel::moduleButtonPress(QString moduleAction)
{
     emit ButtonPressed(moduleAction);
}

void w_userPanel::sessionTimeChanged(QTime time)
{
//    QMessageBox msgBox;
//     QString as = "sessionTimeChanged";
//     as += "/n w_userpanel";
//     msgBox.setText(as);
//     msgBox.exec();

    //dynamic_cast<w_moduleButtonLabel *>(sessionTimeOutLabel)->getLabel()->setText("time.toString()");
    dynamic_cast<w_moduleButtonLabel *>(sessionTimeOutLabel)->getLabel()->setText(time.toString());
}

void w_userPanel::connectionToServerStatusChanged(QAbstractSocket::SocketState state)
{
    switch(state)
    {
        case QAbstractSocket::UnconnectedState:
        {
            dynamic_cast<w_moduleButtonLabel *>(connectionStateButton)->getLabel()->setText("DOWN");
            break;
        }
        case QAbstractSocket::ConnectingState:
        {
            dynamic_cast<w_moduleButtonLabel *>(connectionStateButton)->getLabel()->setText("Connecting");
            break;
        }
        case QAbstractSocket::ConnectedState:
        {
            dynamic_cast<w_moduleButtonLabel *>(connectionStateButton)->getLabel()->setText("UP");
            break;
        }
        default:
        {
            dynamic_cast<w_moduleButtonLabel *>(connectionStateButton)->getLabel()->setText("ERROR");
            break;
        }
    }
}

void w_userPanel::userLogged(QString name, UserRole role)
{
    QString text = name;
    if(role == SUPER) text += QString(" [SUPER]");
    else if(role == ADMIN) text += QString(" [ADMIN]");
    else if(role == MODERATOR) text += QString(" [MODERATOR]");
    else if(role == USER) text += QString(" [UŻYTKOWNIK]");
    else if(role == GUEST) text += QString(" [GOŚĆ]");
    else if(role == NO_ROLE) text += QString(" []");


    dynamic_cast<w_moduleButtonLabel *>(loggedUserName)->getLabel()->setText(text);

    logStateButton->setName("log_out");
    logStateButton->setIcon(":/icons/log_out_128_128.png");
    logStateButton->setDescription("Nacisnij aby wylogować się z systemu.");
    logStateButton->setPressedAction("type=doFunction,target=logout,params=CURRENT_USER");
    logStateButton->loadButton();
}

void w_userPanel::userNotLogged()
{
    dynamic_cast<w_moduleButtonLabel *>(loggedUserName)->getLabel()->setText(QString(""));

    logStateButton->setName("log_in");
    logStateButton->setIcon(":/icons/log_in_128_128.png");
    logStateButton->setDescription("Nacisnij aby zalogować się do systemu.");
    logStateButton->setPressedAction("type=show,target=login_dialog,params=NULL");
    logStateButton->loadButton();
}


void w_userPanel::createLayout()
{
    contentLayout = new QHBoxLayout();
//    contentLayout->setMargin(0);

    setLayout(contentLayout);
    setLayoutDirection(Qt::RightToLeft);

    contentLayout->setAlignment(Qt::AlignCenter);

    c_moduleButtonsFactory * factory = new c_moduleButtonsFactory();


    closeButton = factory->getButton(SMALL_BUTTON);
    controlButton = factory->getButton(SMALL_BUTTON);
    logStateButton = factory->getButton(SMALL_BUTTON);
    loggedUserName = factory->getButton(LABEL_BUTTON);
    sessionTimeOutLabel = factory->getButton(LABEL_BUTTON);
    connectionStateButton = factory->getButton(LABEL_BUTTON);

    createCloseButton();
    createControlButton();
    createLogStateButton();
    createLoggedUserName();
    createSessionTimeOutLabel();
    createConnectionStateButton();

    closeButton->setMinimumSize(40,40);
    closeButton->setMaximumSize(40,40);

    controlButton->setMinimumSize(40,40);
    controlButton->setMaximumSize(40,40);

    logStateButton->setMinimumSize(40,40);
    logStateButton->setMaximumSize(40,40);

    loggedUserName->setMinimumSize(140,40);
    loggedUserName->setMaximumSize(140,40);
    dynamic_cast<w_moduleButtonLabel *>(loggedUserName)->getLabel()->setMinimumSize(140,40);
    dynamic_cast<w_moduleButtonLabel *>(loggedUserName)->getLabel()->setMaximumSize(140,40);

    sessionTimeOutLabel->setMinimumSize(60,40);
    sessionTimeOutLabel->setMaximumSize(60,40);    

    connectionStateButton->setMinimumSize(60,40);
    connectionStateButton->setMaximumSize(60,40);




    connect(closeButton, SIGNAL(moduleClicked(QString) ), this, SLOT(moduleButtonPress(QString) ) );
    connect(controlButton, SIGNAL(moduleClicked(QString) ), this, SLOT(moduleButtonPress(QString) ) );
    connect(logStateButton, SIGNAL(moduleClicked(QString) ), this, SLOT(moduleButtonPress(QString) ) );
    connect(loggedUserName, SIGNAL(moduleClicked(QString) ), this, SLOT(moduleButtonPress(QString) ) );
    connect(sessionTimeOutLabel, SIGNAL(moduleClicked(QString) ), this, SLOT(moduleButtonPress(QString) ) );

    contentLayout->addWidget(closeButton);
    contentLayout->addWidget(controlButton);
    contentLayout->addWidget(logStateButton);
    contentLayout->addWidget(loggedUserName);
    contentLayout->addWidget(sessionTimeOutLabel);
    contentLayout->addWidget(connectionStateButton);
}

void w_userPanel::createControlButton()
{
    controlButton->setName("name=settings");
    controlButton->setIcon(":/icons/settings_128_128.png");
    controlButton->setDescription("Ustawienia systemu.");
    controlButton->setPressedAction("type=show,target=configure_window,params=NULL");

    controlButton->loadButton();
}

void w_userPanel::createLogStateButton()
{
    if( !( dynamic_cast<c_ClinicClient *>(c_AppTrayIcon::Instance()->parent()) )->getUser()->getIsLogged() ) {
        logStateButton->setName("log_in");
        logStateButton->setIcon(":/icons/log_in_128_128.png");
        logStateButton->setDescription("Nacisnij aby zalogować się do systemu.");
        logStateButton->setPressedAction("type=show,target=login_dialog,params=NULL");
    } else {
        logStateButton->setName("log_out");
        logStateButton->setIcon(":/icons/log_out_128_128.png");
        logStateButton->setDescription("Nacisnij aby wylogować się z systemu.");
        logStateButton->setPressedAction("type=doFunction,target=logout,params=CURRENT_USER");
    }

    logStateButton->loadButton();
}

void w_userPanel::createConnectionStateButton()
{
    connectionStateButton->setName("ConnectionToServerStatus");
    connectionStateButton->setDescription("Status połączenia z serwerem.");
    connectionStateButton->loadButton();
}

void w_userPanel::createLoggedUserName()
{
    if( ( dynamic_cast<c_ClinicClient *>(c_AppTrayIcon::Instance()->parent()) )->getUser()->getIsLogged()) {
        loggedUserName->setName("UserName");
        loggedUserName->setPressedAction("type=process,target=USER_PROFILE_CARD,params=CURRENT_USER");
        loggedUserName->loadButton();
        dynamic_cast<w_moduleButtonLabel *>(loggedUserName)->getLabel()->setText(QString("Zalogowany USER"));
    } else {
        loggedUserName->setName("UserName");
        loggedUserName->setPressedAction("type=NULL,target=NULL,params=NULL");
        loggedUserName->loadButton();
        dynamic_cast<w_moduleButtonLabel *>(loggedUserName)->getLabel()->setText(QString("Nie zalogowany USER"));
    }
}

void w_userPanel::createCloseButton()
{
    closeButton->setName("name=exit");
    closeButton->setIcon(":/icons/exit_256_256.png");
    closeButton->setDescription("Nacisnij aby zakończyć pracę i zamknąć system. Nastąpi automatyczne wylogowanie.");
    closeButton->setPressedAction("type=doFunction,target=close_application,params=NULL");

    closeButton->loadButton();
}

void w_userPanel::createSessionTimeOutLabel()
{
    sessionTimeOutLabel->setName("SessionExpireTime");
    sessionTimeOutLabel->setDescription("Czas pozostały do zakończenia sesji.");
    sessionTimeOutLabel->setPressedAction("type=NULL,target=NULL,params=NULL");

    sessionTimeOutLabel->loadButton();
}
