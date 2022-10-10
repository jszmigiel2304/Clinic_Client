#include "w_threadinfogroupbox.h"
#include "ui_w_threadinfogroupbox.h"

w_ThreadInfoGroupBox::w_ThreadInfoGroupBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::w_ThreadInfoGroupBox)
{
    ui->setupUi(this);
}

w_ThreadInfoGroupBox::~w_ThreadInfoGroupBox()
{
    delete ui;
}

Ui::w_ThreadInfoGroupBox *w_ThreadInfoGroupBox::getUi() const
{
    return ui;
}

void w_ThreadInfoGroupBox::setUi(Ui::w_ThreadInfoGroupBox *newUi)
{
    ui = newUi;
}

QLabel *w_ThreadInfoGroupBox::getLID()
{
    return this->ui->l_ID;
}

QLabel *w_ThreadInfoGroupBox::getLNote()
{
    return this->ui->l_Note;
}

QLabel *w_ThreadInfoGroupBox::getLParentIdentifier()
{
    return this->ui->l_ParentIdentifier;
}

QLabel *w_ThreadInfoGroupBox::getLMyParentConnector()
{
    return this->ui->l_MyParentConnector;
}
