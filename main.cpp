#include "w_mainwindow.h"
#include "c_settingscontroller.h"
#include "c_versioncontroller.h"
#include "c_apptrayicon.h"
#include "w_loggingdialog.h"
#include "c_myfiles.h"
#include "c_clinicclient.h"

#include <QApplication>
#include <QSettings>
#include <QFile>
#include <QSql>
#include <QStatusBar>
#include <QMessageBox>
#include <QSystemTrayIcon>


int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("./");

    QApplication a(argc, argv);
    a.setApplicationName("Clinic - Client");    

    c_myFiles * files = c_myFiles::Instance();

    files->setConfigFilePath(a.applicationDirPath() + "/config.ini");

    c_VersionController * versionCtrlr = new c_VersionController(qApp);

    QStringList * tempFileList = new QStringList();
    tempFileList->clear();

    if( !versionCtrlr->checkFiles(tempFileList) )
    {
        versionCtrlr->repairFiles(tempFileList);

        tempFileList->clear();
        if( !versionCtrlr->checkFiles(tempFileList) ) {
            QMessageBox msgBox;

            QString msg = QString("Nie udało się odczytać niezbędnych plików\nZamykam aplikację");

            msgBox.setText(msg);
            msgBox.exec();

            a.exit(0);
        }


        /*     1. konfiguracja połączenia z serwerem
         *     2. sprawdzenie aktualnej wersji
         *     3. pobranie wersji
         *     4. ponowne sprawdzenie plikow
         */
    }
    delete tempFileList;


    c_ClinicClient *client = new c_ClinicClient();
    client->run();

//    c_AppTrayIcon * appTrayIcon = c_AppTrayIcon::Instance();

//    appTrayIcon->show();
    //appTrayIcon->run();




    return a.exec();
}
