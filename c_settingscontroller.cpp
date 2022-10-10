#include "c_settingscontroller.h"

c_SettingsController::c_SettingsController()
{
    this->settingsFilePath = QString("%1/config.ini").arg(QCoreApplication::applicationDirPath());
}

c_SettingsController::c_SettingsController(QString settingsFilePath)
{
    this->settingsFilePath = settingsFilePath;
}

void c_SettingsController::LoadSettings()
{
        QSettings sett(this->settingsFilePath, QSettings::IniFormat);
        QStringList groups = sett.childGroups();

        foreach(QString groupName, groups)
        {
            sett.beginGroup(groupName);

            QMap <QString, QVariant> map;

            foreach (const QString &childKey, sett.childKeys())
            {
                map.insert(childKey, sett.value(childKey).toString());
            }

            this->settings.insert(groupName, map);

            sett.endGroup();
        }
}

QString c_SettingsController::getSettingsFilePath() const
{
    return this->settingsFilePath;
}

QMap<QString, QMap<QString, QVariant>> c_SettingsController::getSettings() const
{
    return this->settings;
}

QMap<QString, QVariant> c_SettingsController::getSettings(QString groupName) const
{
    return this->settings[groupName];
}

void c_SettingsController::SaveToFile(QString groupName, QMap<QString, QVariant> map)
{
    QSettings settings(QString("%1/config.ini").arg(qApp->applicationDirPath()), QSettings::IniFormat);

    settings.beginGroup(groupName);
    foreach (QString item, map.keys()) {
        settings.setValue(item, map[item]);
    }
    settings.endGroup();
}

void c_SettingsController::SaveToFile(QString groupName, QMap<QString, QVariant> map, QString filePath)
{
    QSettings settings(filePath, QSettings::IniFormat);

    settings.beginGroup(groupName);
    foreach (QString item, map.keys()) {
        settings.setValue(item, map[item]);
    }
    settings.endGroup();

}

