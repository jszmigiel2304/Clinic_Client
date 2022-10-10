#ifndef W_AUTHORIZATIONDIALOG_H
#define W_AUTHORIZATIONDIALOG_H

#include <QApplication>
#include <QDialog>
#include <QAuthenticator>
#include <QRegularExpression>
#include <QTimer>
#include <QCloseEvent>

namespace Ui {
class w_AuthorizationDialog;
}

class w_AuthorizationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit w_AuthorizationDialog(QWidget *parent = nullptr);
    ~w_AuthorizationDialog();

    void closeEvent(QCloseEvent *e) override;

public slots:
    void authorizationProcessing(QString text);
    void authorizationFinished();
    void authorizationFault(QString text);
    void setUsernameLabel(QString name);



private:
    Ui::w_AuthorizationDialog *ui;    

    QTimer * logResultTimer;

    bool closeOnLoggingFinished;

    bool closeApp;
    void addStyleSheets();


private slots:
    void logResultTimerFinished();
    void validateUserName();
    void validateUserPassword();

    void on_b_logIn_clicked();

signals:
    void processRequest(QString userName, QString userPassword);
    void closed();
};

#endif // W_AUTHORIZATIONDIALOG_H
