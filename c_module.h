#ifndef C_MODULE_H
#define C_MODULE_H

#include <QMap>
#include <QVariant>
#include <QObject>

#include "c_mystructures.h"
#include "m_module.h"
#include "c_actionmodulepressed.h"

class c_module : public QObject, public m_module
{
    Q_OBJECT
public:
    c_module(QMap<QString, QVariant> settings);
    ~c_module();

    myStructures::moduleInfo getModuleInfo();

public slots:
    void runModule();

private:

};

#endif // C_MODULE_H
