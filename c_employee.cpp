#include "c_employee.h"

c_employee::c_employee(bool allocSupervisor, QObject *parent) : m_employee(parent)
{
    if(allocSupervisor)
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

QMap<QString, QVariant> c_employee::getProperties(bool combinedAddress, bool combinedPhones)
{
    QMap<QString, QVariant> map;

    map["id"] = this->getId();
    map["name"] = this->getName();
    map["second_name"] = this->getSecond_name();
    map["last_name"] = this->getLast_name();

    map["position"] = this->getPositionStringForm();
    map["salary_base"] = this->getSalary_base();
    map["salary_bonus"] = this->getSalary_bonus();

    if(combinedPhones) {
        map["phone_number"] = QString("%1, %2").arg(this->getPhone_number(), this->getPhone_number_2());
    } else {
        map["phone_number"] = this->getPhone_number();
        map["pgone_number_2"] = this->getPhone_number_2();
    }

    if(combinedAddress) {
        map["address_living"] = this->getAddressLiving();
        map["address_contact"] = this->getAddressContact();
    } else {
        map["address_living"] = this->getAddress_living();
        map["postal_code_living"] = this->getPostal_code_living();
        map["city_living"] = this->getCity_living();

        map["address_contact"] = this->getAddress_contact();
        map["postal_code_contact"] = this->getPostal_code_contact();
        map["city_contact"] = this->getCity_contact();
    }

    map["pesel"] = this->getPesel();
    map["user_name"] = this->getUser_name();
    map["employment_date"] = this->getEmployment_date().toString();
    map["gender"] = this->getGender();
    map["photo"] = this->getPhoto();

    return map;
}



