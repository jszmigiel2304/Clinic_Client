#include "w_settingswindow.h"
#include "ui_w_settingswindow.h"

w_SettingsWindow::w_SettingsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::w_SettingsWindow)
{
    ui->setupUi(this);
}

w_SettingsWindow *w_SettingsWindow::Instance()
{
    static w_SettingsWindow * instance = nullptr;
    if ( instance == nullptr ) {
        instance = new w_SettingsWindow();
    }
    return instance;
}

w_SettingsWindow::~w_SettingsWindow()
{
    delete ui;
}

void w_SettingsWindow::closeEvent(QCloseEvent *event)
{
    emit aboutToClose( dynamic_cast<QWidget *>(this) );

    event->accept();
}
