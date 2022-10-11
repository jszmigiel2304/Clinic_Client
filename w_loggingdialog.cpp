#include "w_loggingdialog.h"
#include "ui_w_loggingdialog.h"
#include "w_serverconnectiondialog.h"

w_LoggingDialog::w_LoggingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::w_LoggingDialog)
{
    ui->setupUi(this);

    this->addStyleSheets();

    connect(this, SIGNAL(finished(int)), this, SLOT(deleteLater()));
    connect(this, SIGNAL(rejected()), this, SLOT(deleteLater()));


    this->setWindowTitle(qApp->applicationName());

    ui->w_serverInfoArea->hide();
    ui->w_loggingMask->hide();

    this->serverAreaShown = false;

    connect(ui->f_userName, SIGNAL(editingFinished()), this, SLOT(validateUserName()));
    connect(ui->f_password, SIGNAL(editingFinished()), this, SLOT(validateUserPassword()));

    logResultTimer = new QTimer();

    connect(logResultTimer, SIGNAL(timeout()), this, SLOT(logResultTimerFinished()));
    closeOnLoggingFinished = false;
}

void w_LoggingDialog::addStyleSheets()
{
    ui->b_logIn->setStyleSheet("QPushButton {"
                        "background-color: rgba(255, 255, 255, 0);"
                        "border-style: solid;"
                        "border-width: 1px;"
                        "font: 75 12pt \"Tahoma\";"
                        "color: rgb(0, 0, 0);"
                        "border-color: rgba(0, 0, 0, 255);"
                        "}"
                        "QPushButton:hover {"
                        "background-color: rgba(0, 0, 0, 10);"
                        "}"
                        "QPushButton:pressed {"
                        "background-color: rgba(0, 0, 0, 20);"
                        "}");
}

w_LoggingDialog *w_LoggingDialog::Instance()
{
    static w_LoggingDialog * instance = nullptr;
    if ( instance == nullptr ) {
        instance = new w_LoggingDialog();
    }    
    //instance->setAttribute(Qt::WA_DeleteOnClose, true);
    return instance;
}

w_LoggingDialog::~w_LoggingDialog()
{
   //this->disconnect(SIGNAL(getUserId(QString,QString)), this);

    logResultTimer->deleteLater();
    delete ui;
}

void w_LoggingDialog::setServerConnectionSettings(QMap<QString, QVariant> settings, bool refresh)
{
    serverAddress = settings["address"].toString();
    serverPort = settings["port"].toInt();

    if(refresh)
        this->refresh();
}

void w_LoggingDialog::setAuthorizationSettings(QAuthenticator authorizationData, bool refresh)
{
    userName = authorizationData.user();
    userPassword = authorizationData.password();

    if(refresh)
        this->refresh();
}

void w_LoggingDialog::setWindowSettings(QMap<QString, QVariant> settings, bool refresh)
{
    this->minimizeToTrayOnStart = settings["minimize_to_tray_on_start"].toBool();
    this->minimizeToTrayOnClose = settings["minimize_to_tray_on_close"].toBool();

    if(refresh)
        this->refresh();
}

QMap<QString, QVariant> w_LoggingDialog::ShareProperties(QString sharedData)
{
    QMap<QString, QVariant> map;

    if ( sharedData == "all" || sharedData == "userOnly" )
    {
        map.insert("name", this->userName);
        map.insert("passsword", this->userPassword);
    }

    if ( sharedData == "all" || sharedData == "serverOnly" )
    {
        map.insert("address", this->serverAddress);
        map.insert("port", this->serverPort);
    }


    return map;
}

void w_LoggingDialog::UpdateProperties(QMap<QString, QVariant> map)
{
    if( !map["name"].toString().isEmpty() )
        this->userName = map["name"].toString();

    if( !map["password"].toString().isEmpty() )
        this->userPassword = map["password"].toString();

    if( !map["address"].toString().isEmpty() )
        this->serverAddress = map["address"].toString();

    if( map["port"].toInt() )
        this->serverPort = map["port"].toInt();
}


void w_LoggingDialog::on_b_showHideButton_clicked()
{
    if( this->serverAreaShown ) {
        ui->w_serverInfoArea->hide();

        QIcon icon(":/icons/arrow_down.png");
        ui->b_showHideButton->setIcon(icon);
        ui->b_showHideButton->setIconSize(QSize(16,16));

        this->serverAreaShown = false;
    } else {
        ui->w_serverInfoArea->show();

        QIcon icon(":/icons/arrow_up.png");
        ui->b_showHideButton->setIcon(icon);
        ui->b_showHideButton->setIconSize(QSize(16,16));

        this->serverAreaShown = true;
    }
}

void w_LoggingDialog::refresh()
{
    ui->f_password->setText(this->userPassword);
    ui->f_userName->setText(this->userName);

    ui->l_Address->setText(this->serverAddress);
    ui->l_Port->setText( QString("%1").arg(this->serverPort) );
}

void w_LoggingDialog::closeEvent(QCloseEvent *event)
{

    event->accept();
//    if(this->minimizeToTrayOnClose)
//    {
//        this->hide();
//        emit this->myStateChanged(this->windowState(), false);
//        event->ignore();
//    }
//    else
//    {
//        emit this->myStateChanged(this->windowState(), true);
//        event->accept();
//    }
}

void w_LoggingDialog::logInProcessing(QString text)
{
    ui->l_logInStateText->setText(text);
    this->setEnabled(false);
    ui->w_loggingMask->setEnabled(true);
    ui->w_loggingMask->show();
}

void w_LoggingDialog::logInFinished()
{
    closeOnLoggingFinished = true;
    ui->l_logInStateText->setText(QString("Pomyślnie zalogowano"));

    logResultTimer->start(1000);
}

void w_LoggingDialog::logInFault(QString text)
{
    closeOnLoggingFinished = false;

    ui->l_logInStateText->setText(text);

    logResultTimer->start(1000);
}

void w_LoggingDialog::setUsernameLabel(QString name)
{
    ui->f_userName->setText(name);
}

void w_LoggingDialog::on_b_configureButton_clicked()
{
    w_ServerConnectionDialog * newDialog = new w_ServerConnectionDialog(this);
    newDialog->AddWatchedObject("loggingDialog", dynamic_cast<i_Watched *>(this));
    newDialog->update();
    newDialog->show();
}

void w_LoggingDialog::validateUserName()
{
    QString name = ui->f_userName->text();
    QRegularExpression nameRegExp( "[A-Za-z0-9_]{1,32}" );

    if(nameRegExp.match(name).isValid())
    {
        ui->f_userName->setStyleSheet("");
    }
    else
    {
        ui->f_userName->setStyleSheet("border: 2px solid red;");
    }
}

void w_LoggingDialog::validateUserPassword()
{
    QString password = ui->f_password->text();
    QRegularExpression passwordRegExp( "[A-Za-z0-9_]{1,32}" );

    if(passwordRegExp.match(password).isValid())
    {
        ui->f_password->setStyleSheet("");
    }
    else
    {
        ui->f_password->setStyleSheet("border: 2px solid red;");
    }
}

void w_LoggingDialog::on_b_logIn_clicked()
{
    emit getUserId(ui->f_userName->text(), ui->f_password->text());
}

void w_LoggingDialog::logResultTimerFinished()
{
    logResultTimer->stop();

    this->setEnabled(true);
    ui->w_loggingMask->setEnabled(false);
    ui->w_loggingMask->hide();

    if(closeOnLoggingFinished)
        this->close();
}



