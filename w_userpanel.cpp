#include "w_userpanel.h"
#include "c_apptrayicon.h"
#include "c_clinicclient.h"

w_userPanel::w_userPanel(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    createLayout();

}

w_userPanel::~w_userPanel()
{
    loggedUserName->deleteLater();
    logStateButton->deleteLater();
    connectionStateButton->deleteLater();
    controlButton->deleteLater();
    closeButton->deleteLater();

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
    QMessageBox msgBox;
     QString as = moduleAction;
     as += "/n w_userpanel";
     msgBox.setText(as);
     msgBox.exec();
    emit ButtonPressed(moduleAction);
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

    createCloseButton();
    createControlButton();
    createLogStateButton();
    createLoggedUserName();

    closeButton->setMinimumSize(40,40);
    closeButton->setMaximumSize(40,40);

    controlButton->setMinimumSize(40,40);
    controlButton->setMaximumSize(40,40);

    logStateButton->setMinimumSize(40,40);
    logStateButton->setMaximumSize(40,40);

    loggedUserName->setMinimumSize(280,40);
    loggedUserName->setMaximumSize(280,40);

    connect(closeButton, SIGNAL(moduleClicked(QString) ), this, SLOT(moduleButtonPress(QString) ) );
    connect(controlButton, SIGNAL(moduleClicked(QString) ), this, SLOT(moduleButtonPress(QString) ) );
    connect(logStateButton, SIGNAL(moduleClicked(QString) ), this, SLOT(moduleButtonPress(QString) ) );
    //connect(loggedUserName, SIGNAL(moduleClicked(QString) ), this, SLOT(moduleButtonPress(QString) ) );

    contentLayout->addWidget(closeButton);
    contentLayout->addWidget(controlButton);
    contentLayout->addWidget(logStateButton);
    contentLayout->addWidget(loggedUserName);

}

void w_userPanel::createControlButton()
{
    controlButton->setName("name=settings");
    controlButton->setIcon(":/icons/settings_128_128.png");
    controlButton->setDescription("Ustawienia systemu.");
    controlButton->setPressedAction("type=show,target=configure_window,params=");

    controlButton->loadButton();
}

void w_userPanel::createLogStateButton()
{


    if( !( dynamic_cast<c_ClinicClient *>(c_AppTrayIcon::Instance()->parent()) )->getUser()->getIsLogged() ) {
        logStateButton->setName("log_in");
        logStateButton->setIcon(":/icons/log_in_128_128.png");
        logStateButton->setDescription("Nacisnij aby zalogować się do systemu.");
        logStateButton->setPressedAction("type=show,target=login_dialog,params=");
    } else {
        logStateButton->setName("log_out");
        logStateButton->setIcon(":/icons/log_out_128_128.png");
        logStateButton->setDescription("Nacisnij aby wylogować się z systemu.");
        logStateButton->setPressedAction("type=doFunction,target=logout,params=");
    }

    logStateButton->loadButton();
}

void w_userPanel::createConnectionStateButton()
{

}

void w_userPanel::createLoggedUserName()
{
    if( ( dynamic_cast<c_ClinicClient *>(c_AppTrayIcon::Instance()->parent()) )->getUser()->getIsLogged()) {
        loggedUserName->setName("UserName");
        loggedUserName->setPressedAction("type=process,target=EMPLOYEE_APP_NAME,params=THIS_USER");
        loggedUserName->loadButton();
    }
}

void w_userPanel::createCloseButton()
{
    closeButton->setName("name=exit");
    closeButton->setIcon(":/icons/exit_256_256.png");
    closeButton->setDescription("Nacisnij aby zakończyć pracę i zamknąć system. Nastąpi automatyczne wylogowanie.");
    closeButton->setPressedAction("type=doFunction,target=close_application,params=");

    closeButton->loadButton();
}
