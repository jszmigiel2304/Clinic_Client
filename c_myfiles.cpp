#include "c_myfiles.h"

c_myFiles::c_myFiles(QObject *parent) : QObject(parent)
{

}

QString c_myFiles::getConfigFilePath() const
{
    return configFilePath;
}

c_myFiles *c_myFiles::Instance()
{
    static c_myFiles * instance = nullptr;
    if ( instance == nullptr ) {
        instance = new c_myFiles();
    }
    return instance;
}

c_myFiles::~c_myFiles()
{

}

void c_myFiles::setConfigFilePath(const QString &value)
{
    configFilePath = value;
}

QString c_myFiles::getCssFilePath() const
{
    return cssFilePath;
}

void c_myFiles::setCssFilePath(const QString &value)
{
    cssFilePath = value;
}
