#include "m_employee.h"

m_employee::m_employee(QObject *parent) : QObject(parent)
{

}

m_employee::m_employee(quint32 id, QString name, QString last_name, QObject *parent)
    : QObject(parent), id(id), name(name), last_name(last_name)
{

}

quint32 m_employee::getId() const
{
    return id;
}

void m_employee::setId(quint32 newId)
{
    id = newId;
}

const QString &m_employee::getName() const
{
    return name;
}

void m_employee::setName(const QString &newName)
{
    name = newName;
}

const QString &m_employee::getSecond_name() const
{
    return second_name;
}

void m_employee::setSecond_name(const QString &newSecond_name)
{
    second_name = newSecond_name;
}

const QString &m_employee::getLast_name() const
{
    return last_name;
}

void m_employee::setLast_name(const QString &newLast_name)
{
    last_name = newLast_name;
}

m_employee::Position m_employee::getPosition() const
{
    return position;
}

void m_employee::setPosition(m_employee::Position newPosition)
{
    position = newPosition;
}

const QString &m_employee::getPositionStringForm() const
{
    return positionStringForm;
}

void m_employee::setPositionStringForm(const QString &newPositionStringForm)
{
    positionStringForm = newPositionStringForm;
}

double m_employee::getSalary_base() const
{
    return salary_base;
}

void m_employee::setSalary_base(double newSalary_base)
{
    salary_base = newSalary_base;
}

double m_employee::getSalary_bonus() const
{
    return salary_bonus;
}

void m_employee::setSalary_bonus(double newSalary_bonus)
{
    salary_bonus = newSalary_bonus;
}

const QString &m_employee::getPhone_number() const
{
    return phone_number;
}

void m_employee::setPhone_number(const QString &newPhone_number)
{
    phone_number = newPhone_number;
}

const QString &m_employee::getAddress_living() const
{
    return address_living;
}

void m_employee::setAddress_living(const QString &newAddress_living)
{
    address_living = newAddress_living;
}

const QString &m_employee::getPostal_code_living() const
{
    return postal_code_living;
}

void m_employee::setPostal_code_living(const QString &newPostal_code_living)
{
    postal_code_living = newPostal_code_living;
}

const QString &m_employee::getCity_living() const
{
    return city_living;
}

void m_employee::setCity_living(const QString &newCity_living)
{
    city_living = newCity_living;
}

QString m_employee::getAddressLiving() const
{
    return QString("%1\n%2, %3").arg(getAddress_living(), getPostal_code_living(), getCity_living());
}

const QString &m_employee::getAddress_contact() const
{
    return address_contact;
}

void m_employee::setAddress_contact(const QString &newAddress_contact)
{
    address_contact = newAddress_contact;
}

const QString &m_employee::getPostal_code_contact() const
{
    return postal_code_contact;
}

void m_employee::setPostal_code_contact(const QString &newPostal_code_contact)
{
    postal_code_contact = newPostal_code_contact;
}

const QString &m_employee::getCity_contact() const
{
    return city_contact;
}

void m_employee::setCity_contact(const QString &newCity_contact)
{
    city_contact = newCity_contact;
}

QString m_employee::getAddressContact() const
{
    return QString("%1\n%2, %3").arg(getAddress_contact(), getPostal_code_contact(), getCity_contact());
}

const QString &m_employee::getPesel() const
{
    return pesel;
}

void m_employee::setPesel(const QString &newPesel)
{
    pesel = newPesel;
}

const QString &m_employee::getUser_name() const
{
    return user_name;
}

void m_employee::setUser_name(const QString &newUser_name)
{
    user_name = newUser_name;
}

const QDate &m_employee::getEmployment_date() const
{
    return employment_date;
}

void m_employee::setEmployment_date(const QDate &newEmployment_date)
{
    employment_date = newEmployment_date;
}

const QChar &m_employee::getGender() const
{
    return gender;
}

void m_employee::setGender(const QChar &newGender)
{
    gender = newGender;
}

const QByteArray &m_employee::getPhoto() const
{
    return photo;
}

void m_employee::setPhoto(const QByteArray &newPhoto)
{
    photo = newPhoto;
}

const QString &m_employee::getPhone_number_2() const
{
    return phone_number_2;
}

void m_employee::setPhone_number_2(const QString &newPhone_number_2)
{
    phone_number_2 = newPhone_number_2;
}
