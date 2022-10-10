#ifndef W_SESSIONCHECKBOX_H
#define W_SESSIONCHECKBOX_H


#include <QWidget>

namespace Ui {
class w_sessionCheckBox;
}

class w_sessionCheckBox : public QWidget
{
    Q_OBJECT

public:
    explicit w_sessionCheckBox(QWidget *parent = nullptr);
    ~w_sessionCheckBox();

private:
    Ui::w_sessionCheckBox *ui;
};

#endif // W_SESSIONCHECKBOX_H
