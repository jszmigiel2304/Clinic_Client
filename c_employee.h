#ifndef C_EMPLOYEE_H
#define C_EMPLOYEE_H

#include "m_employee.h"

#include <QObject>
#include <QMap>
#include <QList>

class c_employee : public m_employee
{
    Q_OBJECT
public:
    c_employee(QObject *parent = nullptr);
    ~c_employee();


    c_employee *getSupervisor() const;
    void setSupervisor(c_employee *newSupervisor);

    QMap<QString, QVariant> getProperties(bool combinedAddress = false);

public slots:


private:
    c_employee * supervisor;
};

#endif // C_EMPLOYEE_H
