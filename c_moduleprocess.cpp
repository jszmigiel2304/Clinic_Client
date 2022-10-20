#include "c_moduleprocess.h"

c_moduleProcess::c_moduleProcess(QObject *parent) : QProcess(parent)
{
    executive = new c_actionExecutive(this);
}

c_moduleProcess::~c_moduleProcess()
{

}

c_actionExecutive *c_moduleProcess::getExecutive() const
{
    return executive;
}

void c_moduleProcess::setExecutive(c_actionExecutive *newExecutive)
{
    executive = newExecutive;
}

void c_moduleProcess::setModuleProcessName(const QString &newModuleProcessName)
{
    moduleProcessName = newModuleProcessName;
}

const QString &c_moduleProcess::getModuleProcessName() const
{
    return moduleProcessName;
}

QByteArray c_moduleProcess::getModuleProcessNameHash(QCryptographicHash::Algorithm algorithm, bool toHex)
{
    QByteArray encryptedName;

    QByteArray encryptionSeed;
    QDataStream encryptionStream(&encryptionSeed, QIODevice::ReadWrite);
    encryptionStream.setVersion(QDataStream::Qt_6_0);
    encryptionStream << moduleProcessName.toStdString().c_str();

    QCryptographicHash hasher(algorithm);
    hasher.addData(encryptionSeed);
    encryptedName = hasher.result();

    QString hax = encryptedName.toHex();

    return (toHex ? encryptedName.toHex() : encryptedName);
}

