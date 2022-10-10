#ifndef W_SERVERCONNECTIONDIALOG_H
#define W_SERVERCONNECTIONDIALOG_H

#include "i_observer.h"
#include "c_settingscontroller.h"
#include "w_loggingdialog.h"
#include "c_myconnectiontester.h"
#include "c_myfiles.h"

#include <QDialog>
#include <QWidget>
#include <QPushButton>
#include <QApplication>
#include <QHostAddress>
#include <QTimer>
#include <QIntValidator>
#include <QCloseEvent>
#include <QRegularExpression>

namespace Ui {
class w_ServerConnectionDialog;
}

class w_ServerConnectionDialog : public QDialog, public i_Observer
{
    Q_OBJECT
public:
    explicit w_ServerConnectionDialog(QWidget *parent = nullptr);
    ~w_ServerConnectionDialog();
    void update();

private:
    Ui::w_ServerConnectionDialog *ui;
    bool isChanged;
    QString address;
    quint16 port;

    void addStyleSheets();


private slots:
    void settingsChanged(bool);
    void settingsChanged(QString);
    void validateAddress();
    void validatePort();
    void okButtonPressed();
    void resetButtonPressed();
    void applyButtonPressed();
    void closeEvent(QCloseEvent * e);
    bool addressIsValid();
    bool portIsValid();
    void on_b_test_clicked();
    void refreshButtonOnTimeOut();
};

#endif // W_SERVERCONNECTIONDIALOG_H
