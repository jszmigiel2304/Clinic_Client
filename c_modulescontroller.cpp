#include "c_modulescontroller.h"

c_modulesController::c_modulesController()
{

}

c_modulesController::~c_modulesController()
{

}

void c_modulesController::LoadSettings(QMap<QString, QVariant> settings)
{
    QString str = settings["modules"].toString();
    QStringList strs = str.split(','/*, QString::SkipEmptyParts*/);
    foreach (QString name, strs) {
        this->modulesNames << name;
    }

}

void c_modulesController::LoadModuleSettings(QString moduleName, QMap<QString, QVariant> settings)
{
    this->modulesSettings.insert(moduleName, settings);
}

void c_modulesController::LoadModules(QStringList modulesNames)
{
    QMap<QString, QVariant> sett;
    sett.clear();
    foreach (QString name, modulesNames) {

            sett.insert("name", (this->modulesSettings[name])["name"].toString() );
            sett.insert("displayed_name", (this->modulesSettings[name])["displayed_name"].toString() );
            sett.insert("icon", (this->modulesSettings[name])["icon"].toString() );
            sett.insert("description", (this->modulesSettings[name])["description"].toString() );
            sett.insert("pressed_action", (this->modulesSettings[name])["pressed_action"].toString() );

            c_module * module = new c_module(sett);

            this->moduleList.push_back(module);
    }
}

QStringList c_modulesController::getModulesNames()
{
    return modulesNames;
}

QList<myStructures::moduleInfo> c_modulesController::getModulesProperties()
{
    QList<myStructures::moduleInfo> list;
    foreach (c_module * module, this->moduleList) {
        list.push_back( module->getModuleInfo() );
    }

    return list;
}

QStringList c_modulesController::parseModuleActionString(QString action)
{
    QStringList strings = action.split(',');

    QString type = strings[0];
    QString target = strings[1];

    QStringList ret;
    ret << type;
    ret << target;

    return ret;
}

void c_modulesController::modulePressed(QString action)
{
}
