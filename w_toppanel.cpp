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

    userPanel->setMinimumSize(460,50);
    userPanel->setMaximumSize(460,50);

    buttonListPanel->setMinimumSize(540,50);
    buttonListPanel->setMaximumSize(540,50);

}

w_userPanel *w_topPanel::getUserPanel() const
{
    return userPanel;
}

void w_topPanel::setUserPanel(w_userPanel *newUserPanel)
{
    userPanel = newUserPanel;
}

void w_topPanel::createUserPanel()
{
    userPanel = new w_userPanel();
    userPanel->setStyleSheet("background-color: #FFFFFF;");

    userPanel->setFont(QFont("Segoe", 8, QFont::Bold));

    connect(userPanel, SIGNAL(ButtonPressed(QString)), this, SLOT(moduleButtonPress(QString)));
}

void w_topPanel::createButtonListPanel()
{
    buttonListPanel = new QWidget();
    buttonListPanel->setStyleSheet("background-color: #FFFFFF;");
}
