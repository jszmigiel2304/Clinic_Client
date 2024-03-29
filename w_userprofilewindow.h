#ifndef W_USERPROFILEWINDOW_H
#define W_USERPROFILEWINDOW_H

#include "c_mystructures.h"
#include "c_waitingloop.h"

#include <QMainWindow>
#include <QEvent>
#include <QCloseEvent>
#include <QPushButton>
#include <QAbstractButton>
#include <QWidget>
#include <QScrollArea>
#include <QScrollBar>
#include <QLabel>

namespace Ui {
class w_UserProfileWindow;
}

class w_UserProfileWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit w_UserProfileWindow(QWidget *parent = nullptr);
    ~w_UserProfileWindow();
    void setProperties(QMap<QString, QVariant> userProperties, QMap<QString, QVariant> employeeProperties, bool refresh = 0);
    void setUpLoop();

    QList<myStructures::myLog> Logs;
    QMap<QString, QVariant> userProperties;
    QMap<QString, QVariant> employeeProperties;

    c_waitingLoop::c_waitingLoop *waitingLoop;

    c_waitingLoop::c_waitingLoop *getWaitingLoop() const;
    void setWaitingLoop(c_waitingLoop::c_waitingLoop *newWaitingLoop);

public slots:
    void refresh();
    void refreshProperties();
    void closeEvent(QCloseEvent * event);
    void setUserProperties(QMap<QString, QVariant> userProperties);
    void setEmployeeProperties(QMap<QString, QVariant> employeeProperties);
    void setLogs(QList<myStructures::myLog> list);

private:
    Ui::w_UserProfileWindow *ui;



private slots:
    void moreLogsButtonClicked(bool checked);
    void refreshUserInfo();
    void refreshEmployeeInfo();
    void refreshLogs();    

    void processing(QString text);
    void processingFinished(int code);
    void processingFault(QString text);

signals:
    void getUserPanelProperties(QMap<QString, QVariant> * userProperties, QMap<QString, QVariant> * employeeProperties, QList<myStructures::myLog> * Logs);
    void aboutToClose(QWidget * wnd);
};

class w_moreLogsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit w_moreLogsWindow(QList<myStructures::myLog> * Logs, QWidget *parent = nullptr);
    ~w_moreLogsWindow();

private:
    QWidget central;
    QScrollArea scrollArea;
    QWidget scrollAreaWidget;
};

#endif // W_USERPROFILEWINDOW_H
