#include "w_mainwindow.h"
#include "ui_w_mainwindow.h"

w_MainWindow::w_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::w_MainWindow)
{
    ui->setupUi(this);

    processingTimer = new QTimer();
    connect(processingTimer, SIGNAL(timeout()), this, SLOT(processingTimerFinished()));
}

void w_MainWindow::closeEvent(QCloseEvent *event)
{
    setMinimizeToTrayOnClose(false);
    emit closeApplicationButtonClicked();
    if(this->getMinimizeToTrayOnClose())
    {
        this->hide();
        emit this->myStateChanged(this->windowState(), false);
        event->ignore();
    }
    else
    {
        setMinimizeToTrayOnClose(false);
        emit closeApplicationButtonClicked();
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

    createProcessingMask();
}

void w_MainWindow::createModulesScrollArea()
{
    this->modulesScrollArea = new w_modulesScrollArea();
    this->modulesScrollArea->setParent(ui->centralWidget);
    this->modulesScrollArea->setGeometry(5, 60, 1000, 480);    
    connect(modulesScrollArea, SIGNAL(ButtonPressed(QString)), this, SLOT(moduleButtonPressed(QString)));

    mCtr = ( dynamic_cast<c_modulesController *>(this->watchedObjectsList["modulesController"]) );

    connect(this, SIGNAL(modulePressed(QString)), mCtr, SLOT(modulePressed(QString)));

    QList<myStructures::moduleInfo> modulesProperties = ( dynamic_cast<c_modulesController *>(this->watchedObjectsList["modulesController"]) )->getModulesProperties();
    this->modulesScrollArea->loadModules(modulesProperties);
    this->modulesScrollArea->setVisible(false);
    this->modulesScrollArea->setEnabled(false);
    this->modulesScrollArea->refresh();    

}

void w_MainWindow::createTopPanel()
{
    topPanel = w_topPanel::Instance();
    topPanel->setParent(ui->centralWidget);
    topPanel->setGeometry(0, 0, 1000, 60);
    connect(topPanel, SIGNAL(ButtonPressed(QString)), this, SLOT(moduleButtonPressed(QString)));

    topPanel->setVisible(true);
}

void w_MainWindow::createInformationsLabel()
{
    informationLabel = w_informationsLabel::Instance();
    informationLabel->setParent(ui->centralWidget);
    informationLabel->setGeometry(0, 540, 1000, 60);
    informationLabel->setVisible(true);
}

void w_MainWindow::createProcessingMask()
{
    mask = new QWidget(ui->centralWidget);
    mask->setGeometry(0,0,1200,600);
    mask->setStyleSheet("background-color: rgba(255,255,255,200);");
    mask->setEnabled(false);

    processingtextLabel = new QLabel(mask);
    processingtextLabel->setWordWrap(true);
    processingtextLabel->setStyleSheet(QString("color: rgb(0, 0, 0);"
                                               "background-color: rgba(255,255,255,200);"
                                               "font-family: Segoe Ul;"
                                               "font-size:  30px;"));

    processingtextLabel->setGeometry(0,0,1200,600);
    processingtextLabel->setAlignment(Qt::AlignCenter);
    mask->hide();
}

void w_MainWindow::processingTimerFinished()
{
    processingTimer->stop();

    this->setEnabled(true);
    mask->setEnabled(false);
    mask->hide();
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
    this->mask->deleteLater();
    this->processingtextLabel->deleteLater();

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

bool w_MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonPress || event->type() == QEvent::KeyPress) {
          event->accept();
    }

    return false;

}

w_userPanel *w_MainWindow::getUserPanel() const
{
    return topPanel->getUserPanel();
}

void w_MainWindow::newAddWindow(QWidget * window)
{
    if(window != nullptr)
        addWindows.append(window);
}

void w_MainWindow::removeAddWindow(QWidget *window)
{
    addWindows.removeAll(window);
    addWindows.squeeze();
}

void w_MainWindow::moduleButtonPressed(QString action)
{
    QStringList args = action.split(',', Qt::SkipEmptyParts);
    QMap<QString, QString> paramsMap;
    foreach(QString arg, args) {
        QStringList temp = arg.split('=');
        if(!temp.isEmpty())
            paramsMap[temp[0]] = temp[1];
    }
    QStringList parametersList = paramsMap["params"].split(QString("||"), Qt::SkipEmptyParts);

    QMap<QString, QString> parameters;
    foreach(QString param, parametersList) {
        QStringList temp = param.split(QString("::::"));
        if(!temp.isEmpty())
          parameters[temp[0]] = temp[1];
    }


    if(paramsMap["type"] == "NULL") {return;}
    if(paramsMap["type"] == "doFunction") {moduleButtonPressedDoFunction(paramsMap["target"], parameters);  return;}
    if(paramsMap["type"] == "show") {moduleButtonPressedShow(paramsMap["target"], parameters); return;}
    if(paramsMap["type"] == "process") {moduleButtonPressedProcess(paramsMap["target"], parameters); return;}

}

void w_MainWindow::moduleButtonPressedDoFunction(QString target, QMap<QString, QString> params)
{
    if(target == "NULL") {return;}
    if(target == "close_application") { setMinimizeToTrayOnClose(false); emit closeApplicationButtonClicked(); }
    if(target == "logout") { emit logOutModuleClicked(-1, QString(), QString()); return;}
}

void w_MainWindow::moduleButtonPressedShow(QString target, QMap<QString, QString> params)
{
    if(target == "NULL") {return;}
    if(target == "configure_window") { emit settingsWindowModuleClicked(); return;}
    if(target == "login_dialog") { emit loggingWindowModuleClicked(); return;}
}

void w_MainWindow::moduleButtonPressedProcess(QString target, QMap<QString, QString> params)
{
    if(target == "NULL") {return;}
    if(target == "USER_PROFILE_CARD") { emit userProfileButtonClicked(); return;}

    emit processAppButtonClicked(target, params);
    return;
}

void w_MainWindow::Processing(QString processText)
{
    processingtextLabel->setText(processText);
    mask->show();
    mask->setEnabled(true);
    this->setEnabled(false);
}

void w_MainWindow::ProcessingFinished()
{
    processingtextLabel->setText(QString("Processing finished."));

    processingTimer->start(500);
}

void w_MainWindow::ProcessingFault(QString processText)
{
    processingtextLabel->setText(processText);

    processingTimer->start(1000);
}

void w_MainWindow::IdleDetected(QString username)
{
    processingtextLabel->setText(QString("Sesja zablokowana.\n %1 Podaj hasło aby kontynuować.").arg(username));
    mask->show();
    mask->setEnabled(true);
    this->setEnabled(false);
}

void w_MainWindow::unlockWindow()
{
    processingtextLabel->setText(QString(""));
    mask->hide();
    mask->setEnabled(false);
    this->setEnabled(true);
}

void w_MainWindow::lockModulesScrollArea()
{
    this->modulesScrollArea->setEnabled(false);
    this->modulesScrollArea->setVisible(false);
}

void w_MainWindow::closeAllAddWindows()
{
    while(addWindows.size() != 0) {
        addWindows.takeFirst()->close();
    }
}

void w_MainWindow::unLockModulesScrollArea(QString n, QString r)
{
    this->modulesScrollArea->setEnabled(true);
    this->modulesScrollArea->setVisible(true);
}

