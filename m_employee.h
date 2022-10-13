#ifndef M_EMPLOYEE_H
#define M_EMPLOYEE_H

#include <QObject>
#include <QPixmap>
#include <QDateTime>
#include <QDate>
#include <QCryptographicHash>
#include <QIODevice>
#include <QMetaType>
#include <QMetaEnum>

class m_employee : public QObject
{
    Q_OBJECT

public:
    enum Position { LEKARZ = 0x0000, PIELEGNIARZ = 0x0001, POLOZNIK = 0x0002, RATOWNIK_MEDYCZNY = 0x0003, FARMACEUTA = 0x0004, DIAGNOSTA_LABORATORYJNY = 0x0005,
                  FELCZER = 0x0006, FIZJOTERAPEUTA = 0x0007, ASYSTENTKA_STOMATOLOGICZNA = 0x0008, DIETETYK = 0x0009, ELEKTROKARDIOLOG = 0x000A, HIGIENISTKA_STOMATOLOGICZNA = 0x000B,
                   LOGOPEDA = 0x000C, MASAZYSTA = 0x000D, OPIEKUN_MEDYCZNY = 0x000E, OPTOMETRYSTA = 0x000F, OPTYK_OKULAROWY = 0x0010, ORTOPTYSTA = 0x0011, PROTETYK_SLUCHU = 0x0012,
                   PSYCHOTERAPEUTA = 0x0013, TECHNIK_ANALITYKI_MEDYCZNEJ = 0x0014, TECHNIK_DENTYSTYCZNY = 0x0015, TECHNIK_FARMACEUTYCZNY = 0x0016, TECHNIK_ORTOPEDA = 0x0017, TERAPEUTA_ZAJECIOWY = 0x0018,
                   KIEROWNIK = 0x0019, ASYSTENT = 0x001A, PRACOWNIK_REJESTRACJI = 0x001B, SPRZATACZ = 0x001C, BRAK = 0x001D
                  };
    Q_ENUM(Position)

    explicit m_employee(QObject *parent = nullptr);
    m_employee(quint32 id, QString name, QString last_name, QObject *parent = nullptr);

    quint32 getId() const;
    void setId(quint32 newId);

    const QString &getName() const;
    void setName(const QString &newName);

    const QString &getSecond_name() const;
    void setSecond_name(const QString &newSecond_name);

    const QString &getLast_name() const;
    void setLast_name(const QString &newLast_name);

    m_employee::Position getPosition() const;
    void setPosition(m_employee::Position newPosition);

    const QString &getPositionStringForm() const;
    void setPositionStringForm(const QString &newPositionStringForm);

    double getSalary_base() const;
    void setSalary_base(double newSalary_base);

    double getSalary_bonus() const;
    void setSalary_bonus(double newSalary_bonus);

    const QString &getPhone_number() const;
    void setPhone_number(const QString &newPhone_number);

    const QString &getPgone_number_2() const;
    void setPgone_number_2(const QString &newPgone_number_2);

    const QString &getAddress_living() const;
    void setAddress_living(const QString &newAddress_living);

    const QString &getPostal_code_living() const;
    void setPostal_code_living(const QString &newPostal_code_living);

    const QString &getCity_living() const;
    void setCity_living(const QString &newCity_living);
    QString getAddressLiving() const;

    const QString &getAddress_contact() const;
    void setAddress_contact(const QString &newAddress_contact);

    const QString &getPostal_code_contact() const;
    void setPostal_code_contact(const QString &newPostal_code_contact);

    const QString &getCity_contact() const;
    void setCity_contact(const QString &newCity_contact);
    QString getAddressContact() const;

    const QString &getPesel() const;
    void setPesel(const QString &newPesel);

    const QString &getUser_name() const;
    void setUser_name(const QString &newUser_name);

    const QDate &getEmployment_date() const;
    void setEmployment_date(const QDate &newEmployment_date);

    const QChar &getGender() const;
    void setGender(const QChar &newGender);

    const QByteArray &getPhoto() const;
    void setPhoto(const QByteArray &newPhoto);

private:
    quint32 id;
    QString name;
    QString second_name;
    QString last_name;
    m_employee::Position position;
    QString positionStringForm;
    double salary_base;
    double salary_bonus;
    QString phone_number;
    QString pgone_number_2;
    QString address_living;
    QString postal_code_living;
    QString city_living;
    QString address_contact;
    QString postal_code_contact;
    QString city_contact;
    QString pesel;
    QString user_name;
    QDate employment_date;
    QChar gender;
    QByteArray photo;
};

#endif // M_EMPLOYEE_H
