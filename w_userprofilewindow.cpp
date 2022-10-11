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

w_UserProfileWindow *w_UserProfileWindow::Instance()
{
    static w_UserProfileWindow * instance = nullptr;
    if ( instance == nullptr ) {
        instance = new w_UserProfileWindow();
    }
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


void w_UserProfileWindow::refresh()
{
}

void w_UserProfileWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}
