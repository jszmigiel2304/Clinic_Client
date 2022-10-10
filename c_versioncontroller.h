#ifndef C_VERSIONCONTROLLER_H
#define C_VERSIONCONTROLLER_H

#include "m_defaultinifile.h"

#include <QObject>
#include <QCoreApplication>
#include <QFile>
#include <QSettings>
#include <QMessageBox>

class c_VersionController : public QObject
{
    Q_OBJECT
public:
    explicit c_VersionController(QObject *parent = nullptr);

    bool checkFiles(QStringList *files);
    void repairFiles(QStringList *filesToRepair);

    bool checkConfigIni();
    bool checkConfigIni(QString fileName);
    void createConfigIni();

    QString getConfigIniFileName() const;
    void setConfigIniFileName(const QString &value);

signals:

public slots:

private:
    QString configIniFileName;
};

#endif // C_VERSIONCONTROLLER_H
