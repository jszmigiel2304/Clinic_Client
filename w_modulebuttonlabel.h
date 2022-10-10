#ifndef W_MODULEBUTTONLABEL_H
#define W_MODULEBUTTONLABEL_H

#include "w_modulebutton.h"

#include <QWidget>
#include <QLabel>

class w_moduleButtonLabel : public w_moduleButton
{
    Q_OBJECT
public:
    w_moduleButtonLabel(QWidget *parent = nullptr);
    ~w_moduleButtonLabel();

    QLabel *getLabel() const;
    void setLabel(QLabel *newLabel);

    void loadButton();

protected:
    bool eventFilter(QObject* , QEvent* evt);
    void enterEvent(QEnterEvent *);
    void leaveEvent(QEvent *);

signals:
    void moduleClicked(QString action);

private:
    QLabel *label;

    void showNameLabel();
    void hideNameLabel();

    void createLabel();

private slots:
    void labelClicked();


};

#endif // W_MODULEBUTTONLABEL_H
