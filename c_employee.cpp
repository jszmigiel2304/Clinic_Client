#include "c_employee.h"

c_employee::c_employee(QObject *parent) : m_employee(parent)
{
    setSupervisor( new c_employee() );
}

c_employee::~c_employee()
{
    getSupervisor()->deleteLater();
}

c_employee *c_employee::getSupervisor() const
{
    return supervisor;
}

void c_employee::setSupervisor(c_employee *newSupervisor)
{
    supervisor = newSupervisor;
}

QMap<QString, QVariant> c_employee::getProperties(bool combinedAddress)
{

}

