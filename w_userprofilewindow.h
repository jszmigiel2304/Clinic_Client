#ifndef W_USERPROFILEWINDOW_H
#define W_USERPROFILEWINDOW_H

#include <QMainWindow>

namespace Ui {
class w_UserProfileWindow;
}

class w_UserProfileWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit w_UserProfileWindow(QWidget *parent = nullptr);
    ~w_UserProfileWindow();

private:
    Ui::w_UserProfileWindow *ui;
};

#endif // W_USERPROFILEWINDOW_H
