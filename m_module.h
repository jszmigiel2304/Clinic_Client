#ifndef M_MODULE_H
#define M_MODULE_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QVariant>

class m_module
{
public:
    m_module();

    QString getName() const;
    void setName(const QString &value);

    QString getDisplayedName() const;
    void setDisplayedName(const QString &value);

    QString getIcon() const;
    void setIcon(const QString &value);

    QString getDescription() const;
    void setDescription(const QString &value);

    QString getPressedAction() const;
    void setPressedAction(const QString &value);

signals:

public slots:

private:
    QString name;
    QString displayedName;
    QString icon;
    QString description;
    QString pressedAction;
};

#endif // M_MODULE_H
