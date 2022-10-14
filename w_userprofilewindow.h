#ifndef W_USERPROFILEWINDOW_H
#define W_USERPROFILEWINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <QCloseEvent>

namespace Ui {
class w_UserProfileWindow;
}

class w_UserProfileWindow : public QMainWindow
{
    Q_OBJECT

public:
    static w_UserProfileWindow *Instance();
    ~w_UserProfileWindow();
    void setProperties(QMap<QString, QVariant> userProperties, QMap<QString, QVariant> employeeProperties, bool refresh = 0);
    void setUserProperties(QMap<QString, QVariant> userProperties, bool refresh = 0);
    void setEmployeeProperties(QMap<QString, QVariant> employeeProperties, bool refresh = 0);
    void setLogs(QList<QString> list, bool refresh = 0);

public slots:
    void refresh();
    void closeEvent(QCloseEvent * event);

private:
    explicit w_UserProfileWindow(QWidget *parent = nullptr);
    Ui::w_UserProfileWindow *ui;

    QList<QString> Logs;
    QMap<QString, QVariant> userProperties;
    QMap<QString, QVariant> employeeProperties;

private slots:
    void on_more_logs_button_clicked();
    void refreshUserInfo();
    void refreshEmployeeInfo();
    void refreshLogs();
};

#endif // W_USERPROFILEWINDOW_H
