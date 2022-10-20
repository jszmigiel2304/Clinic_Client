#ifndef C_MODULEPROCESS_H
#define C_MODULEPROCESS_H

#include "c_actionExecutive.h"
#include "c_moduleprocessconnection.h"

#include <QProcess>
#include <QObject>
#include <QLocalSocket>
#include <QCryptographicHash>

class c_moduleProcess : public QProcess
{
    Q_OBJECT
public:
    c_moduleProcess(QObject *parent = nullptr);
    ~c_moduleProcess();

    c_actionExecutive *getExecutive() const;
    void setExecutive(c_actionExecutive *newExecutive);

    void setModuleProcessName(const QString &newModuleProcessName);
    const QString &getModuleProcessName() const;
    QByteArray getModuleProcessNameHash(QCryptographicHash::Algorithm algorithm = QCryptographicHash::Md5, bool toHex = true);

private:
    c_actionExecutive *executive;
    QString moduleProcessName;

private slots:

signals:
    void passModuleProcessToController(c_moduleProcess * process);



};

#endif // C_MODULEPROCESS_H
