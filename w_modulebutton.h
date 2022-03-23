#ifndef W_MODULEBUTTON_H
#define W_MODULEBUTTON_H

#include <QObject>
#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include <QFont>
#include <QEvent>
#include <QAction>
#include <QVBoxLayout>
#include <QMessageBox>

#include "w_informationslabel.h"
#include "c_mystructures.h"
#include "m_module.h"

class w_moduleButton : public QWidget, public m_module
{
    Q_OBJECT
public:
    w_moduleButton();
    ~w_moduleButton();

    virtual void loadButton();
};

#endif // W_MODULEBUTTON_H
