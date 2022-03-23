#ifndef C_SETTINGSCONTROLLER_H
#define C_SETTINGSCONTROLLER_H

#include "m_defaultinifile.h"

#include <QString>
#include <QFile>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>


class c_SettingsController
{
public:
    c_SettingsController();
    c_SettingsController(QString settingsFilePath);
    void LoadSettings();
    QString getSettingsFilePath() const;
    QMap<QString, QMap<QString, QVariant>> getSettings() const;
    QMap<QString, QVariant> getSettings(QString groupName) const;
    void SaveToFile(QString groupName, QMap<QString, QVariant> map);
    void SaveToFile(QString groupName, QMap<QString, QVariant> map, QString filePath);
    
private:
    QString settingsFilePath;

    QMap<QString, QMap<QString, QVariant>> settings;
    
};

#endif // C_SETTINGSCONTROLLER_H
