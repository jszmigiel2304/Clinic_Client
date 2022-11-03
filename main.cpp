#include "w_mainwindow.h"
#include "c_settingscontroller.h"
#include "c_versioncontroller.h"
#include "c_apptrayicon.h"
#include "w_loggingdialog.h"
#include "c_myfiles.h"
#include "c_clinicclient.h"
#include "c_threadcontroller.h"
#include "myapplication.h"

#include <QApplication>
#include <QSettings>
#include <QFile>
#include <QSql>
#include <QStatusBar>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QObject>


int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("./");
    MyApplication a(argc, argv);
    a.setApplicationName("Clinic - Client");    

    c_ThreadController *threadCtrlr = new c_ThreadController();
    c_ClinicClient *client = new c_ClinicClient();
    client->setThreadCtrlr(threadCtrlr);

    QObject::connect(client, SIGNAL(passDataToThread(threadData, qintptr)), threadCtrlr, SLOT(dataReceived(threadData, qintptr)));
    QObject::connect(&a, SIGNAL(idleDetected()), client, SLOT(applicatioIdleDetected()), Qt::DirectConnection);
    QObject::connect(client->getSessionCtrlr()->thread(), SIGNAL(sessionTimeSet(quint32)), &a, SLOT(idleTimeReceived(quint32)), Qt::DirectConnection);

    c_AppTrayIcon *trayIcon = c_AppTrayIcon::Instance(client);
    client->setTrayIcon(trayIcon);
    w_MainWindow *mainWnd = w_MainWindow::Instance();
    client->setMainWindow(mainWnd);

    QObject::connect( &a, SIGNAL(aboutToQuit()), client, SLOT( deleteLater() ) );
    QObject::connect( &a, SIGNAL(aboutToQuit()), mainWnd, SLOT( deleteLater() ) );
    QObject::connect( mainWnd, SIGNAL(destroyed(QObject*)), trayIcon, SLOT(deleteLater()) );

    client->run();

    return a.exec();
}
