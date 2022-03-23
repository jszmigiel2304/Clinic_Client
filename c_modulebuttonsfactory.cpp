#include "c_modulebuttonsfactory.h"

c_moduleButtonsFactory::c_moduleButtonsFactory()
{

}

c_moduleButtonsFactory::~c_moduleButtonsFactory()
{

}

w_moduleButton *c_moduleButtonsFactory::getButton(buttonType buttonType)
{
    switch ( static_cast<int>(buttonType) ) {
    case BIG_BUTTON:
        return new w_moduleButtonBig();
    case SMALL_BUTTON:
        return  new w_moduleButtonSmall();
    case LABEL_BUTTON:
        return new w_moduleButtonLabel();
    default:
        return nullptr;
    }
}

