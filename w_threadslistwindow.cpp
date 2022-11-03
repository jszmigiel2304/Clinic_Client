#include "w_threadslistwindow.h"
#include "ui_w_threadslistwindow.h"

w_ThreadsListWindow::w_ThreadsListWindow(c_ThreadController * threadCtrlr, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::w_ThreadsListWindow)
{
    threadCtr = threadCtrlr;



    ui->setupUi(this);

    connect(ui->refreshPushButton, SIGNAL(pressed()), this, SLOT(refresh()));
}

w_ThreadsListWindow *w_ThreadsListWindow::Instance(c_ThreadController * threadCtrlr, QWidget *parent)
{
    static w_ThreadsListWindow * instance = nullptr;
    if ( instance == nullptr ) {
        instance = new w_ThreadsListWindow(threadCtrlr, parent);
    }
    return instance;
}

w_ThreadsListWindow::~w_ThreadsListWindow()
{
    delete ui;
}

void w_ThreadsListWindow::refresh()
{
    QWidget * contentwidget = new QWidget();
    contentwidget->setGeometry(0, 0, 400, 800);
    QVBoxLayout * layout = new QVBoxLayout(ui->scrollAreaWidgetContents);

    for(int i = 0; i < threadCtr->getThreadsList().size(); i++)
    {
        w_ThreadInfoGroupBox * box = new w_ThreadInfoGroupBox(ui->scrollAreaWidgetContents);

        box->setGeometry(0, i*100 + 10*i, 380, 100);
        box->getLMyParentConnector()->setText(  QString("%1").arg(dynamic_cast<MyThread *>(threadCtr->getThreadsList()[i])->getMyParentConnector()->metaObject()->className()) );
        box->getLID()->setText( QString("%1").arg(dynamic_cast<MyThread *>(threadCtr->getThreadsList()[i])->getId()) );
        box->getLNote()->setText( QString("%1").arg(dynamic_cast<MyThread *>(threadCtr->getThreadsList()[i])->getNote()) );
        box->getLParentIdentifier()->setText( QString("%1").arg(dynamic_cast<MyThread *>(threadCtr->getThreadsList()[i])->getParentIdentifier()) );
        layout->addWidget(box);
    }
    contentwidget->setLayout(layout);

    ui->scrollArea->takeWidget();
    ui->scrollArea->setWidget(contentwidget);
}
