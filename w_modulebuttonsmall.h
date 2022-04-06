#ifndef W_MODULEBUTTONSMALL_H
#define W_MODULEBUTTONSMALL_H

#include <QWidget>
#include <QMessageBox>
#include <QPushButton>

#include "w_modulebutton.h"

class w_moduleButtonSmall : public w_moduleButton
{
    Q_OBJECT
public:
    explicit w_moduleButtonSmall(QWidget *parent = nullptr);
    ~w_moduleButtonSmall();

    QPushButton *getToolButton() const;

    void loadButton();

signals:
    void moduleClicked(QString action);

protected:
    bool eventFilter(QObject* , QEvent* evt);
    void enterEvent(QEnterEvent *);
    void leaveEvent(QEvent *);

private:
    QPushButton *toolButton;

    void showNameLabel();
    void hideNameLabel();

    void CreateToolButton();

private slots:
    void toolButtonclicked(bool);
};

#endif // W_MODULEBUTTONSMALL_H
