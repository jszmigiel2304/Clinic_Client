#include "w_userprofilewindow.h"
#include "ui_w_userprofilewindow.h"

w_UserProfileWindow::w_UserProfileWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::w_UserProfileWindow)
{
    ui->setupUi(this);
}

w_UserProfileWindow::~w_UserProfileWindow()
{
    delete ui;
}
