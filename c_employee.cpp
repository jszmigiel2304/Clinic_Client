#include "c_employee.h"

c_employee::c_employee(bool allocSupervisor, QObject *parent) : m_employee(parent)
{
    if(allocSupervisor)
        setSupervisor( new c_employee() );
}

c_employee::~c_employee()
{

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

    map["supervisor_id"] = this->getSupervisor()->getId();
    map["supervisor_name"] = this->getSupervisor()->getName();
    map["supervisor_last_name"] = this->getSupervisor()->getLast_name();

    return map;
}

void c_employee::setProperties(QMap<QString, QVariant> employeeInfo)
{
    setId( employeeInfo["id"].toUInt() );
    setName( employeeInfo["name"].toString() );
    setSecond_name( employeeInfo["second_name"].toString() );
    setLast_name( employeeInfo["last_name"].toString() );
    setGender( employeeInfo["gender"].toChar() );

    QMetaEnum metaEnum = QMetaEnum::fromType<m_employee::Position>();
    setPosition( static_cast<m_employee::Position>( metaEnum.keyToValue(employeeInfo["position_name"].toString().toStdString().c_str() ) ));
    if(!employeeInfo["position_female_form"].toString().isEmpty() && (getGender() == 'k' || getGender() == 'K'))
        setPositionStringForm( employeeInfo["position_female_form"].toString() );
    else
        setPositionStringForm( employeeInfo["position_male_form"].toString() );

    setSalary_base( employeeInfo["salary_base"].toDouble() );
    setSalary_bonus( employeeInfo["salary_bonus"].toDouble() );

    c_employee * supervisor = new c_employee(false);
    supervisor->setId(  employeeInfo["supervisor_id"].toUInt() );
    supervisor->setName( employeeInfo["supervisor_name"].toString() );
    supervisor->setLast_name( employeeInfo["supervisor_last_name"].toString() );
    setSupervisor(supervisor);

    setPhone_number( employeeInfo["phone_number"].toString() );
    setPhone_number_2( employeeInfo["phone_number_2"].toString() );
    setAddress_living( employeeInfo["address_living"].toString() );
    setPostal_code_living( employeeInfo["postal_code_living"].toString() );
    setCity_living( employeeInfo["city_living"].toString() );
    setAddress_contact( employeeInfo["address_contact"].toString() );
    setPostal_code_contact( employeeInfo["postal_code_contact"].toString() );
    setCity_contact( employeeInfo["city_contact"].toString() );
    setPesel( employeeInfo["pesel"].toString() );
    setUser_name( employeeInfo["user_name"].toString() );
    QDate dat = QDate::fromString(employeeInfo["employment_date"].toString(), Qt::ISODate);
    setEmployment_date( QDate::fromString( employeeInfo["employment_date"].toString(), Qt::ISODate ) );
    setGender( employeeInfo["gender"].toChar() );
    setPhoto(  QByteArray(employeeInfo["photo"].toByteArray()) );

    emit propertiesSaved();
    QMap<QString, QVariant> map = this->getProperties();
    emit passProperties(map);
}



