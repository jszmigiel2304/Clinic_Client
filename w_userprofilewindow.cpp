#include "w_userprofilewindow.h"
#include "ui_w_userprofilewindow.h"

w_UserProfileWindow::w_UserProfileWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::w_UserProfileWindow)
{
    ui->setupUi(this);
}

void w_UserProfileWindow::on_more_auth_logs_button_clicked()
{

}

void w_UserProfileWindow::on_more_Clinic_logs_button_clicked()
{

}

void w_UserProfileWindow::refreshUserInfo()
{
    ui->l_username->setText( userProperties["name"].toString() );
    ui->l_id->setText( QString("%1").arg(userProperties["id"].toInt()) );
    ui->l_name->setText( userProperties["name"].toString() );
    ui->l_role->setText( userProperties["role"].toString() );
    ui->l_create_date->setText( userProperties["create_date"].toString() );
    ui->l_email->setText( userProperties["email"].toString() );
    ui->l_is_blocked->setText( userProperties["is_blocked"].toBool() ? QString("TAK") : QString("NIE") );
    ui->l_verified->setText( userProperties["is_verified"].toBool() ? QString("TAK") : QString("NIE") );
    ui->l_blocked_date->setText( userProperties["blocked_date"].toString() );
    ui->l_verified_date->setText( userProperties["verified_date"].toString() );
    ui->l_is_logged->setText( userProperties["is_logged"].toBool() ? QString("TAK") : QString("NIE"));
}

void w_UserProfileWindow::refreshEmployeeInfo()
{

}

void w_UserProfileWindow::refreshLogs()
{
//    for(int i = 0; i < 5; i++) {
//        if(i < clinicLogs.size()) {
//            QLabel log_label(ui->w_clinic_db_logs_container);
//            log_label.setGeometry(0, (25*(i+1)), 355, 25);
//            log_label.setStyleSheet(QString("background-color: rgb(66, 66, 66);\ncolor: rgb(221, 221, 221);"));
//            log_label.setText( clinicLogs[i] );
//        }
//    }

//    for(int i = 0; i < 3; i++) {
//        if(i < authLogs.size()) {
//            QLabel log_label(ui->w_auth_db_logs_container);
//            log_label.setGeometry(0,(25*(i+1)), 355, 25);
//            log_label.setStyleSheet(QString("background-color: rgb(66, 66, 66);\ncolor: rgb(221, 221, 221);"));
//            log_label.setText( authLogs[i] );
//        }
//    }
}

w_UserProfileWindow *w_UserProfileWindow::Instance()
{
    static w_UserProfileWindow * instance = nullptr;
    if ( instance == nullptr ) {
        instance = new w_UserProfileWindow();
    }
    instance->setAttribute(Qt::WA_DeleteOnClose, true);
    return instance;
}

w_UserProfileWindow::~w_UserProfileWindow()
{
    delete ui;
}

void w_UserProfileWindow::setProperties(QMap<QString, QVariant> userProperties, QMap<QString, QVariant> employeeProperties, bool refresh)
{
    this->userProperties = userProperties;
    this->employeeProperties =employeeProperties;

    if(refresh)
        this->refresh();
}

void w_UserProfileWindow::setUserProperties(QMap<QString, QVariant> userProperties, bool refresh)
{
    this->userProperties = userProperties;

    if(refresh)
        this->refresh();
}

void w_UserProfileWindow::setEmployeeProperties(QMap<QString, QVariant> employeeProperties, bool refresh)
{
    this->employeeProperties =employeeProperties;

    if(refresh)
        this->refresh();
}

void w_UserProfileWindow::setLogs(QList<QString> list, bool refresh)
{

}

void w_UserProfileWindow::refresh()
{
    this->refreshUserInfo();
    this->refreshEmployeeInfo();
    this->refreshLogs();
}

void w_UserProfileWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}
