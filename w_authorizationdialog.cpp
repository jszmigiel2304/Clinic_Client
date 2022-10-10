#include "w_authorizationdialog.h"
#include "ui_w_authorizationdialog.h"

w_AuthorizationDialog::w_AuthorizationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::w_AuthorizationDialog)
{
    ui->setupUi(this);

    this->addStyleSheets();

    connect(this, SIGNAL(finished(int)), this, SLOT(deleteLater()));
    connect(this, SIGNAL(rejected()), this, SLOT(deleteLater()));


    connect(ui->f_userName, SIGNAL(editingFinished()), this, SLOT(validateUserName()));
    connect(ui->f_password, SIGNAL(editingFinished()), this, SLOT(validateUserPassword()));
    //connect(ui->b_logIn, SIGNAL(clicked()), this, SLOT(on_b_logIn_clicked()));


    logResultTimer = new QTimer();

    connect(logResultTimer, SIGNAL(timeout()), this, SLOT(logResultTimerFinished()));
    closeOnLoggingFinished = false;

    ui->w_loggingMask->setEnabled(false);
    ui->w_loggingMask->setGeometry(0,0,0,0);
    ui->w_loggingMask->hide();
    ui->f_password->setEnabled(true);
    ui->f_userName->setEnabled(true);
}

w_AuthorizationDialog::~w_AuthorizationDialog()
{
    logResultTimer->deleteLater();
    delete ui;
}

void w_AuthorizationDialog::closeEvent(QCloseEvent *e)
{
     e->accept();
}

void w_AuthorizationDialog::authorizationProcessing(QString text)
{
    ui->l_logInStateText->setText(text);
    this->setEnabled(false);
    ui->w_loggingMask->setEnabled(true);
    ui->w_loggingMask->setGeometry(0,0,350,300);
    ui->w_loggingMask->show();
}

void w_AuthorizationDialog::authorizationFinished()
{
    closeOnLoggingFinished = true;
    ui->l_logInStateText->setText(QString("Pomyślnie wykonano"));

    logResultTimer->start(1000);
}

void w_AuthorizationDialog::authorizationFault(QString text)
{
    closeOnLoggingFinished = false;

    ui->l_logInStateText->setText(text);

    logResultTimer->start(1000);
}

void w_AuthorizationDialog::setUsernameLabel(QString name)
{
    ui->f_userName->setText(name);
}

void w_AuthorizationDialog::addStyleSheets()
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

void w_AuthorizationDialog::logResultTimerFinished()
{
    logResultTimer->stop();

    this->setEnabled(true);
    ui->w_loggingMask->setEnabled(false);
    ui->w_loggingMask->setGeometry(0,0,0,0);
    ui->w_loggingMask->hide();

    if(closeOnLoggingFinished)
        this->close();
}

void w_AuthorizationDialog::validateUserName()
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

void w_AuthorizationDialog::validateUserPassword()
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

void w_AuthorizationDialog::on_b_logIn_clicked()
{
    emit processRequest(ui->f_userName->text(), ui->f_password->text());
}

