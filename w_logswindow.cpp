#include "w_logswindow.h"
#include "ui_w_logswindow.h"

w_logsWindow::w_logsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::w_logsWindow)
{
    ui->setupUi(this);
}


w_logsWindow *w_logsWindow::Instance()
{
    static w_logsWindow * instance = nullptr;
    if ( instance == nullptr ) {
        instance = new w_logsWindow();
    }
    return instance;
}

w_logsWindow::~w_logsWindow()
{
    delete ui;
}

QTextBrowser *w_logsWindow::getLogBrowser()
{
    return ui->logsTextBrowser;
}

void w_logsWindow::addLog(QString log)
{
    QStringList strlist = log.split('\n');

    foreach(QString str, strlist) {
        getLogBrowser()->append(str);
    }
}

void w_logsWindow::addSeparator(QChar sepChar)
{
    QString str("\n");
    for(int i = 0; i < 30; i++)
    {
        str += sepChar;
    }
    str += '\n';
    getLogBrowser()->append(str);
}
