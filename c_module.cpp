#include "c_module.h"

c_module::c_module(QMap<QString, QVariant> settings)
{
    setName(settings["name"].toString());
    setDisplayedName(settings["displayed_name"].toString());
    setIcon(settings["icon"].toString());
    setDescription(settings["description"].toString());
    setPressedAction(settings["pressed_action"].toString());
}

c_module::~c_module()
{

}

myStructures::moduleInfo c_module::getModuleInfo()
{
    myStructures::moduleInfo infos;

    infos.description = getDescription();
    infos.name = getName();
    infos.displayedName = getDisplayedName();
    infos.icon = getIcon();
    infos.pressedAction = getPressedAction();

    return infos;
}

void c_module::runModule()
{

}
