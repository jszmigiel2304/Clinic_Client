#ifndef C_MODULEBUTTONSFACTORY_H
#define C_MODULEBUTTONSFACTORY_H

#include "w_modulebutton.h"
#include "w_modulebuttonbig.h"
#include "w_modulebuttonsmall.h"
#include "c_mystructures.h"
#include "w_modulebuttonlabel.h"

#include <QObject>
#include <QString>

class c_moduleButtonsFactory : public QObject
{
    Q_OBJECT
public:
    c_moduleButtonsFactory();
    ~c_moduleButtonsFactory();

    w_moduleButton *getButton(myTypes::buttonType buttonType);

private:
};

#endif // C_MODULEBUTTONSFACTORY_H
