#include "w_mainwindow.h"
#include "ui_w_mainwindow.h"

w_MainWindow::w_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::w_MainWindow)
{
    ui->setupUi(this);    


}

void w_MainWindow::closeEvent(QCloseEvent *event)
{
    if(this->getMinimizeToTrayOnClose())
    {
        this->hide();
        emit this->myStateChanged(this->windowState(), false);
        event->ignore();
    }
    else
    {
        emit this->myStateChanged(this->windowState(), true);
        event->accept();
    }
}

void w_MainWindow::createLayout()
{
    setStyleSheet("background-color: rgba(255,255,255,255);");

    createModulesScrollArea();
    createTopPanel();
    createInformationsLabel();
}

void w_MainWindow::createModulesScrollArea()
{
    this->modulesScrollArea = new w_modulesScrollArea();
    this->modulesScrollArea->setParent(this);
    this->modulesScrollArea->setGeometry(5, 60, 1000, 480);    
    connect(modulesScrollArea, SIGNAL(ButtonPressed(QString)), this, SLOT(moduleButtonPressed(QString)));

    mCtr = ( dynamic_cast<c_modulesController *>(this->watchedObjectsList["modulesController"]) );

    connect(this, SIGNAL(modulePressed(QString)), mCtr, SLOT(modulePressed(QString)));

    QList<moduleInfo> modulesProperties = ( dynamic_cast<c_modulesController *>(this->watchedObjectsList["modulesController"]) )->getModulesProperties();
    this->modulesScrollArea->loadModules(modulesProperties);
    this->modulesScrollArea->setVisible(true);
    this->modulesScrollArea->refresh();    

}

void w_MainWindow::createTopPanel()
{
    topPanel = w_topPanel::Instance();
    topPanel->setParent(this);
    topPanel->setGeometry(0, 0, 1000, 60);
    connect(topPanel, SIGNAL(ButtonPressed(QString)), this, SLOT(moduleButtonPressed(QString)));

    topPanel->setVisible(true);
}

void w_MainWindow::createInformationsLabel()
{
    informationLabel = w_informationsLabel::Instance();
    informationLabel->setParent(this);
    informationLabel->setGeometry(0, 540, 1000, 60);
    informationLabel->setVisible(true);
}

w_MainWindow *w_MainWindow::Instance()
{
    static w_MainWindow * instance = nullptr;
    if ( instance == nullptr ) {
        instance = new w_MainWindow();
    }
    return instance;
}

void w_MainWindow::loadSettings(QMap<QString, QVariant> settings)
{
    this->minimizeToTrayOnStart = settings["minimize_to_tray_on_start"].toBool();
    this->minimizeToTrayOnClose = settings["minimize_to_tray_on_close"].toBool();

}

w_MainWindow::~w_MainWindow()
{
    this->mCtr->deleteLater();
    this->modulesScrollArea->deleteLater();
    this->informationLabel->deleteLater();
    this->topPanel->deleteLater();

    delete ui;
}

void w_MainWindow::refresh()
{
    this->createLayout();
}


bool w_MainWindow::getMinimizeToTrayOnClose() const
{
    return minimizeToTrayOnClose;
}

void w_MainWindow::setMinimizeToTrayOnClose(bool value)
{
    minimizeToTrayOnClose = value;
}

bool w_MainWindow::getMinimizeToTrayOnStart() const
{
    return minimizeToTrayOnStart;
}

void w_MainWindow::setMinimizeToTrayOnStart(bool value)
{
    minimizeToTrayOnStart = value;
}

void w_MainWindow::moduleButtonPressed(QString action)
{
    QMessageBox msgBox;
     QString as = action;
     as += "/n w_mainwindow";
     msgBox.setText(as);
     msgBox.exec();

    emit modulePressed(action);
}
