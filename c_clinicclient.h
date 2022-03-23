#ifndef C_CLINICCLIENT_H
#define C_CLINICCLIENT_H

#include "i_observer.h"
#include "c_apptrayicon.h"
#include "w_mainwindow.h"
#include "w_loggingdialog.h"
#include "c_settingscontroller.h"
#include "c_modulescontroller.h"
#include "c_loggeduser.h"
#include "w_logswindow.h"
#include "c_connectiontoservercontroller.h"
#include "c_logscontroller.h"
#include "c_sessioncontroller.h"
#include "c_threadcontroller.h"
#include "c_parser.h"
#include "c_mystructures.h"

#include <QApplication>
#include <QObject>
#include <QSystemTrayIcon>
#include <QActionGroup>
#include <QMenu>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QString>
#include <QDataStream>
#include <QByteArray>
#include <QRandomGenerator>

class c_AppTrayIcon;

class c_ClinicClient : public QObject
{
    Q_OBJECT
public:
    explicit c_ClinicClient(QObject *parent= nullptr );
    ~c_ClinicClient();


    void run();

    void pushTestDataToServer();
    
    w_MainWindow *getMainWindow() const;
    void setMainWindow(w_MainWindow *value);

    c_SettingsController *getSettCtrlr() const;
    void setSettCtrlr(c_SettingsController *value);

    c_modulesController *getModCtrlr() const;
    void setModCtrlr(c_modulesController *value);

    c_connectionToServerController *getConnectionCtrlr() const;
    void setConnectionCtrlr(c_connectionToServerController *value);

    c_loggedUser *getUser() const;
    void setUser(c_loggedUser *value);

    c_AppTrayIcon *getTrayIcon() const;
    void setTrayIcon(c_AppTrayIcon *value);


    w_logsWindow * logsWindow;

    c_ThreadController *getThreadCtrlr() const;
    void setThreadCtrlr(c_ThreadController *newThreadCtrlr);

private:

    w_MainWindow *mainWindow;
    c_AppTrayIcon *trayIcon;

    c_SettingsController *settCtrlr;
    c_modulesController *modCtrlr;
    c_connectionToServerController *connectionCtrlr;
    c_SessionController *sessionCtrlr;
    c_ThreadController *threadCtrlr;
    c_Parser parser;

    c_loggedUser *user;
    

signals:

public slots:

private slots:
    void dataReceived(quint32 data_size, QByteArray data);
    void loggingDialogAccepted();
    void loggingDialogFinished(int r);
    void loggingDialogRejected();
    void logIn(QString userName, QString userPassword);

};

#endif // C_CLINICCLIENT_H
