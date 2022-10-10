#include "w_sessionlistwindow.h"
#include "ui_w_sessionlistwindow.h"

w_sessionListWindow::w_sessionListWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::w_sessionListWindow)
{
    ui->setupUi(this);
}

w_sessionListWindow::~w_sessionListWindow()
{
    delete ui;
}
