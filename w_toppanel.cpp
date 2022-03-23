#include "w_toppanel.h"
#include "c_apptrayicon.h"
#include "c_clinicclient.h"

w_topPanel *w_topPanel::Instance()
{
    static w_topPanel * instance = nullptr;
    if ( instance == nullptr ) {
        instance = new w_topPanel();
    }
    return instance;
}

w_topPanel::~w_topPanel()
{
    userPanel->deleteLater();
    buttonListPanel->deleteLater();
    lay->deleteLater();
}

void w_topPanel::moduleButtonPress(QString moduleAction)
{
    QMessageBox msgBox;
     QString as = moduleAction;
     as += "/n w_toppanel";
     msgBox.setText(as);
     msgBox.exec();
    emit ButtonPressed(moduleAction);
}


w_topPanel::w_topPanel(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("background-color: #000000;");
    setGeometry(0,5,1000,60);
    setMinimumSize(1000,60);
    setMaximumSize(1000,60);
    setEnabled(true);


    lay = new QHBoxLayout();

    createButtonListPanel();
    createUserPanel();


    setLayout(lay);
    lay->addWidget(buttonListPanel);
    lay->addWidget(userPanel);

    setLayoutDirection(Qt::LeftToRight);
    lay->setAlignment(Qt::AlignCenter);

    userPanel->setMinimumSize(400,50);
    userPanel->setMaximumSize(400,50);

    buttonListPanel->setMinimumSize(600,50);
    buttonListPanel->setMaximumSize(600,50);

}

void w_topPanel::createUserPanel()
{
    userPanel = new w_userPanel();
    //userPanel->setGeometry(0,0,400,60);
    userPanel->setStyleSheet("background-color: #FFFFFF;");

    connect(userPanel, SIGNAL(ButtonPressed(QString)), this, SLOT(moduleButtonPress(QString)));
}

void w_topPanel::createButtonListPanel()
{
    buttonListPanel = new QWidget();
    //buttonListPanel->setGeometry(0,0,600,60);
    buttonListPanel->setStyleSheet("background-color: #FFFFFF;");

    //connect(buttonListPanel, SIGNAL(ButtonPressed(QString)), this, SLOT(moduleButtonPress(QString)));
}
