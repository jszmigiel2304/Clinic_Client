#include "c_modulebuttonsfactory.h"

c_moduleButtonsFactory::c_moduleButtonsFactory()
{

}

c_moduleButtonsFactory::~c_moduleButtonsFactory()
{

}

w_moduleButton *c_moduleButtonsFactory::getButton(myTypes::buttonType buttonType)
{
    switch ( static_cast<int>(buttonType) ) {
    case myTypes::BIG_BUTTON:
        return new w_moduleButtonBig();
    case myTypes::SMALL_BUTTON:
        return  new w_moduleButtonSmall();
    case myTypes::LABEL_BUTTON:
        return new w_moduleButtonLabel();
    default:
        return nullptr;
    }
}

