#ifndef C_MODULESCONTROLLER_H
#define C_MODULESCONTROLLER_H

#include "i_observer.h"
#include "i_watched.h"
#include "c_module.h"
#include "c_mystructures.h"

#include <QMap>
#include <QHash>
#include <QObject>
#include <QStringList>
#include <QMessageBox>

class c_modulesController : public QObject, public i_Watched
{
    Q_OBJECT
public:
    c_modulesController();
    ~c_modulesController();

    void LoadSettings(QMap<QString, QVariant> settings);
    void LoadModuleSettings(QString moduleName, QMap<QString, QVariant> settings);
    void LoadModules(QStringList modulesNames);

    QStringList getModulesNames();
    QList<moduleInfo> getModulesProperties();

    QStringList parseModuleActionString(QString action);

public slots:
    void modulePressed(QString action);

signals:
    void closeModuleButtonPressed();
    void settingsModuleButtonPressed();
    void loginModuleButtonPressed();
    void logoutModuleButtonPressed();
    void processModuleButtonPressed(QString processName);

private:
    QList<c_module*> moduleList;
    QStringList modulesNames;
    QMap<QString, QMap<QString, QVariant> > modulesSettings;
};

#endif // C_MODULESCONTROLLER_H
