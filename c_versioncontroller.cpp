#include "c_versioncontroller.h"

c_VersionController::c_VersionController(QObject *parent) : QObject(parent)
{
    this->setConfigIniFileName(QString("%1/config.ini").arg(QCoreApplication::applicationDirPath()));
}

bool c_VersionController::checkFiles(QStringList *files)
{
    if( !checkConfigIni() )
    {
        files->push_back(QString("config.ini"));
    }

    if(files->empty())
        return true;
    else
        return false;
}

void c_VersionController::repairFiles(QStringList *filesToRepair)
{
    if(filesToRepair->contains("config.ini")) {
        this->createConfigIni();
        if( !checkConfigIni() ) {
            QMessageBox msgBox;

            QString msg = QString("BŁĄD !!! \nNie udało się utworzyć config.ini\n");

            msgBox.setText(msg);
            msgBox.exec();

        } else {
            QMessageBox msgBox;

            QString msg = QString("Nie udało się odczytać pliku [ %1 ].\nUtworzono domyślny plik config.ini\n"
                                  "Może być wymagana konfiguracja ustawień").arg(this->getConfigIniFileName());

            msgBox.setText(msg);
            msgBox.exec();
        }
    }


}

bool c_VersionController::checkConfigIni()
{
    QFile file(this->getConfigIniFileName());

    return file.exists();
}

bool c_VersionController::checkConfigIni(QString fileName)
{
    QFile file(fileName);

    return file.exists();
}

void c_VersionController::createConfigIni()
{
    QSettings settings(this->getConfigIniFileName(), QSettings::IniFormat);

    settings.beginGroup("application");
    settings.setValue("auto_log_in", APPLICATION_AUTO_LOG_IN);
    settings.setValue("version", APPLICATION_VERSION);
    settings.endGroup();

    settings.beginGroup("window");
    settings.setValue("minimize_to_tray_on_start", WINDOW_MINIMIZE_ON_START);
    settings.setValue("minimize_to_tray_on_close", WINDOW_MINIMIZE_ON_CLOSE);
    settings.endGroup();

    settings.beginGroup("server");
    settings.setValue("address", SERVER_HOST_ADDRESS);
    settings.setValue("port", SERVER_PORT);
    settings.endGroup();

    settings.beginGroup("user");
    settings.setValue("name", USER_LOGIN);
    settings.setValue("password", USER_PWD);
    settings.endGroup();

    settings.sync();
}

QString c_VersionController::getConfigIniFileName() const
{
    return configIniFileName;
}

void c_VersionController::setConfigIniFileName(const QString &value)
{
    configIniFileName = value;
}
