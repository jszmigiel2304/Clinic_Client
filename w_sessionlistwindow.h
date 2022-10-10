#ifndef W_SESSIONLISTWINDOW_H
#define W_SESSIONLISTWINDOW_H


#include "w_sessioncheckbox.h"

#include <QMainWindow>

namespace Ui {
class w_sessionListWindow;
}

class w_sessionListWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit w_sessionListWindow(QWidget *parent = nullptr);
    ~w_sessionListWindow();

private:
    Ui::w_sessionListWindow *ui;
};

#endif // W_SESSIONLISTWINDOW_H
