#ifndef W_TOPPANEL_H
#define W_TOPPANEL_H

#include "w_userpanel.h"

#include <QObject>
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QEvent>
#include <QFont>
#include <QHBoxLayout>

class w_topPanel : public QWidget
{
    Q_OBJECT
public:
    static w_topPanel *Instance();
    ~w_topPanel();

signals:
    void ButtonPressed(QString action);

public slots:
    void moduleButtonPress(QString moduleAction);

private:
    explicit w_topPanel(QWidget *parent = nullptr);

    w_userPanel * userPanel;
    QWidget * buttonListPanel;

    QHBoxLayout * lay;

    void createUserPanel();
    void createButtonListPanel();
};


#endif // W_TOPPANEL_H
