#ifndef W_USERPANEL_H
#define W_USERPANEL_H

#include "w_modulebutton.h"
#include "i_observer.h"
#include "c_mystructures.h"
#include "c_modulebuttonsfactory.h"

#include <QObject>
#include <QWidget>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QGridLayout>
#include <QEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include <QList>
#include <QLabel>
#include <QTime>
#include <QAbstractSocket>

class w_userPanel : public QWidget
{
    Q_OBJECT
public:
    explicit w_userPanel(QWidget *parent = nullptr);
    ~w_userPanel();

    void refresh();

    bool eventFilter(QObject*, QEvent* evt);

protected:

signals:
    void ButtonPressed(QString action);

public slots:
    void moduleButtonPress(QString moduleAction);
    void sessionTimeChanged(QTime time);
    void connectionToServerStatusChanged(QAbstractSocket::SocketState state);
    void userLogged(QString name, QString role);
    void userNotLogged();

private:
    void createLayout();

    QHBoxLayout * contentLayout;

    w_moduleButton * controlButton;
    w_moduleButton * logStateButton;
    w_moduleButton * connectionStateButton;
    w_moduleButton * loggedUserName;
    w_moduleButton * closeButton;
    w_moduleButton * sessionTimeOutLabel;

    void createControlButton();
    void createLogStateButton();
    void createConnectionStateButton();
    void createLoggedUserName();
    void createCloseButton();
    void createSessionTimeOutLabel();


};

#endif // W_USERPANEL_H
