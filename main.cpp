#include "w_mainwindow.h"
#include "c_settingscontroller.h"
#include "c_versioncontroller.h"
#include "c_apptrayicon.h"
#include "w_loggingdialog.h"
#include "c_myfiles.h"
#include "c_clinicclient.h"
#include "c_threadcontroller.h"
#include "w_logswindow.h"
#include "w_threadslistwindow.h"
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

//    QApplication a(argc, argv);
    MyApplication a(argc, argv);
    a.setApplicationName("Clinic - Client");    


//    c_myFiles * files = c_myFiles::Instance();

//    files->setConfigFilePath(a.applicationDirPath() + "/config.ini");

//    c_VersionController * versionCtrlr = new c_VersionController(qApp);

//    QStringList * tempFileList = new QStringList();
//    tempFileList->clear();

//    if( !versionCtrlr->checkFiles(tempFileList) )
//    {
//        versionCtrlr->repairFiles(tempFileList);

//        tempFileList->clear();
//        if( !versionCtrlr->checkFiles(tempFileList) ) {
//            QMessageBox msgBox;

//            QString msg = QString("Nie udało się odczytać niezbędnych plików\nZamykam aplikację");

//            msgBox.setText(msg);
//            msgBox.exec();

//            a.exit(0);
//        }


//        /*     1. konfiguracja połączenia z serwerem
//         *     2. sprawdzenie aktualnej wersji
//         *     3. pobranie wersji
//         *     4. ponowne sprawdzenie plikow
//         */
//    }
//    delete tempFileList;


    c_ThreadController *threadCtrlr = new c_ThreadController();
    c_ClinicClient *client = new c_ClinicClient();
    client->setThreadCtrlr(threadCtrlr);

    QObject::connect(client, SIGNAL(passDataToThread(threadData)), threadCtrlr, SLOT(dataReceived(threadData)));
    QObject::connect(&a, SIGNAL(idleDetected()), client, SLOT(applicatioIdleDetected()), Qt::DirectConnection);
    QObject::connect(client->getSessionCtrlr()->thread(), SIGNAL(sessionTimeSet(quint32)), &a, SLOT(idleTimeReceived(quint32)), Qt::DirectConnection);

    c_AppTrayIcon *trayIcon = c_AppTrayIcon::Instance(client);
    client->setTrayIcon(trayIcon);
    w_MainWindow *mainWnd = w_MainWindow::Instance();
    client->setMainWindow(mainWnd);
    w_logsWindow *logsWnd = w_logsWindow::Instance();
    client->setLogsWindow(logsWnd);
    w_ThreadsListWindow *threadsWnd = w_ThreadsListWindow::Instance(threadCtrlr);
    client->setThreadsListWindow(threadsWnd);


    QObject::connect( client, SIGNAL(newLog(QString)), logsWnd, SLOT( addLog(QString) ) );
    QObject::connect( threadCtrlr, SIGNAL(newLog(QString)), logsWnd, SLOT( addLog(QString) ) );


    QObject::connect( &a, SIGNAL(aboutToQuit()), client, SLOT( deleteLater() ) );
    QObject::connect( &a, SIGNAL(aboutToQuit()), mainWnd, SLOT( deleteLater() ) );
    QObject::connect( mainWnd, SIGNAL(destroyed(QObject*)), logsWnd, SLOT(close()) );
    QObject::connect( mainWnd, SIGNAL(destroyed(QObject*)), threadsWnd, SLOT(close()) );
    QObject::connect( mainWnd, SIGNAL(destroyed(QObject*)), trayIcon, SLOT(deleteLater()) );


    client->run();


    return a.exec();
}
