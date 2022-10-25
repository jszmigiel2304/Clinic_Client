#ifndef W_SETTINGSWINDOW_H
#define W_SETTINGSWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

namespace Ui {
class w_SettingsWindow;
}

class w_SettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    static w_SettingsWindow * Instance();
    ~w_SettingsWindow();

public slots:
    void closeEvent(QCloseEvent * event);

private:
    explicit w_SettingsWindow(QWidget *parent = nullptr);
    Ui::w_SettingsWindow *ui;

signals:
    void aboutToClose(QWidget * wnd);
};

#endif // W_SETTINGSWINDOW_H
