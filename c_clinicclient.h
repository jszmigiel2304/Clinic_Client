#ifndef C_CLINICCLIENT_H
#define C_CLINICCLIENT_H

#include "i_observer.h"
#include "c_apptrayicon.h"
#include "w_mainwindow.h"
#include "w_loggingdialog.h"
#include "c_settingscontroller.h"
#include "c_modulescontroller.h"
#include "c_loggeduser.h"
#include "c_connectiontoservercontroller.h"
#include "c_logscontroller.h"
#include "c_sessioncontroller.h"
#include "c_threadcontroller.h"
#include "mythread.h"
#include "c_parser.h"
#include "c_mystructures.h"
#include "w_toppanel.h"
#include "w_userpanel.h"
#include "w_authorizationdialog.h"
#include "w_userprofilewindow.h"
#include "c_waitingloop.h"
#include "c_moduleprocess.h"
#include "c_processescontroller.h"

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
#include <QTimer>
#include <QTime>
#include <QAbstractSocket>
#include <QMetaMethod>
#include <QMessageBox>
#include <QPushButton>
#include <QAbstractSocket>
#include <QMetaType>
#include <QEventLoop>
#include <QProcess>

class c_AppTrayIcon;

class c_ClinicClient : public QObject
{
    Q_OBJECT
public:
    explicit c_ClinicClient(QObject *parent= nullptr );
    ~c_ClinicClient();

    void run();
    
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


    c_ThreadController *getThreadCtrlr() const;
    void setThreadCtrlr(c_ThreadController *newThreadCtrlr);

    c_SessionController *getSessionCtrlr() const;
    void setSessionCtrlr(c_SessionController *newSessionCtrlr);

    void refreshWindows(bool show = true);
    void prepareModules();
    void createConnections();

private:
    w_MainWindow *mainWindow;
    c_AppTrayIcon *trayIcon;

    c_SettingsController *settCtrlr;
    c_modulesController *modCtrlr;
    c_connectionToServerController *connectionCtrlr;
    c_SessionController *sessionCtrlr;
    c_ThreadController *threadCtrlr;
    c_processesController *processCtrlr;

    c_Parser parser;

    c_loggedUser *user;    

signals:
    void setUpConnection(QMap<QString, QVariant> settings);
    void runSession();
    void passDataToThread(myStructures::threadData data, qintptr socketDescriptor);
    void packetReceiveConfirmationReceived(myStructures::threadData data);
    void replyReceived(QByteArray ref_md5);
    void sendToServer(myStructures::packet packet);
    void stopAllThreads();
    void sessionConfigured();
    void idleSignalReceived();
    void unlockSessionSignal();
    void logOutUserBeforeCloseApp(qint32 id, QString name, QString password);


public slots:
    void cleanup();
    void showSettingsWindow();
    void showLoggingDialog();
    void showAuthorizationDialogOnIdle(QString username);
    void showUserPanelWindow();
    void closeApplication();
    void applicatioIdleDetected();
    void sessionUnlocked();
    void processApp(QString target, QMap<QString, QString> parameters);

private slots:
    void dataReceived(quint64 data_size, QByteArray data, qintptr socketDescriptor);
};

#endif // C_CLINICCLIENT_H
