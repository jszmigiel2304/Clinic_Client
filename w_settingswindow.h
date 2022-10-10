#ifndef W_SETTINGSWINDOW_H
#define W_SETTINGSWINDOW_H

#include <QMainWindow>

namespace Ui {
class w_SettingsWindow;
}

class w_SettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    static w_SettingsWindow * Instance();
    ~w_SettingsWindow();

private:
    explicit w_SettingsWindow(QWidget *parent = nullptr);
    Ui::w_SettingsWindow *ui;
};

#endif // W_SETTINGSWINDOW_H
