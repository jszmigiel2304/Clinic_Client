#include "w_sessioncheckbox.h"
#include "ui_w_sessioncheckbox.h"

w_sessionCheckBox::w_sessionCheckBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::w_sessionCheckBox)
{
    ui->setupUi(this);
}

w_sessionCheckBox::~w_sessionCheckBox()
{
    delete ui;
}
