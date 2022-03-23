#include "m_module.h"

m_module::m_module()
{

}

QString m_module::getName() const
{
    return name;
}

void m_module::setName(const QString &value)
{
    name = value;
}

QString m_module::getDisplayedName() const
{
    return displayedName;
}

void m_module::setDisplayedName(const QString &value)
{
    displayedName = value;
}

QString m_module::getIcon() const
{
    return icon;
}

void m_module::setIcon(const QString &value)
{
    icon = value;
}

QString m_module::getDescription() const
{
    return description;
}

void m_module::setDescription(const QString &value)
{
    description = value;
}

QString m_module::getPressedAction() const
{
    return pressedAction;
}

void m_module::setPressedAction(const QString &value)
{
    pressedAction = value;
}
