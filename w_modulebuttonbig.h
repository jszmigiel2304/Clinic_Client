#ifndef W_MODULEBUTTONBIG_H
#define W_MODULEBUTTONBIG_H

#include <QWidget>
#include <QPushButton>

#include "w_modulebutton.h"
#include "w_informationslabel.h"

class w_moduleButtonBig : public w_moduleButton
{
    Q_OBJECT
public:
    explicit w_moduleButtonBig(QWidget *parent = nullptr);
    ~w_moduleButtonBig();

    QPushButton *getToolButton() const;

    void loadButton();

signals:
    void moduleClicked(QString action);

protected:
    bool eventFilter(QObject* , QEvent* evt);
    void enterEvent(QEnterEvent *);
    void leaveEvent(QEvent *);
    void mouseReleaseEvent(QMouseEvent *);


private:
    QPushButton *toolButton;
    QLabel * nameLabel;

    void showNameLabel();
    void hideNameLabel();

    void CreateToolButton();
    void CreateNameLabel();

private slots:
    void toolButtonclicked(bool);

private:

};

#endif // W_MODULEBUTTONBIG_H
