#ifndef C_ACTIONMODULEPRESSED_H
#define C_ACTIONMODULEPRESSED_H

#include "c_mystructures.h"

#include <QObject>



class c_actionModulePressed : public QObject
{
    Q_OBJECT
public:
    explicit c_actionModulePressed(QObject *parent = nullptr);

signals:

public slots:
};

#endif // C_ACTIONMODULEPRESSED_H
