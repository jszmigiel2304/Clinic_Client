#ifndef W_THREADINFOGROUPBOX_H
#define W_THREADINFOGROUPBOX_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class w_ThreadInfoGroupBox;
}

class w_ThreadInfoGroupBox : public QWidget
{
    Q_OBJECT

public:
    explicit w_ThreadInfoGroupBox(QWidget *parent = nullptr);
    ~w_ThreadInfoGroupBox();


    Ui::w_ThreadInfoGroupBox *getUi() const;
    void setUi(Ui::w_ThreadInfoGroupBox *newUi);

    QLabel * getLID();
    QLabel * getLNote();
    QLabel * getLParentIdentifier();
    QLabel * getLMyParentConnector();


private:
    Ui::w_ThreadInfoGroupBox *ui;
};

#endif // W_THREADINFOGROUPBOX_H
