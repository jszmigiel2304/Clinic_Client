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
    void setAuthLogs(QList<QString> list, bool refresh = 0);
    void setClinicLogs(QList<QString> list, bool refresh = 0);

public slots:
    void refresh();
    void closeEvent(QCloseEvent * event);

private:
    explicit w_UserProfileWindow(QWidget *parent = nullptr);
    Ui::w_UserProfileWindow *ui;

    QList<QString> authLogs;
    QList<QString> clinicLogs;
    QMap<QString, QVariant> userProperties;
    QMap<QString, QVariant> employeeProperties;

private slots:
    void on_more_auth_logs_button_clicked();
    void on_more_Clinic_logs_button_clicked();
};

#endif // W_USERPROFILEWINDOW_H
