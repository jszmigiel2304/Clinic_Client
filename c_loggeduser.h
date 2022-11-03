#ifndef C_LOGGEDUSER_H
#define C_LOGGEDUSER_H

#include "m_loggeduser.h"
#include "c_mystructures.h"
#include "c_threadcontroller.h"
#include "mythread.h"
#include "cv_processdata.h"
#include "c_loggeduserthread.h"

#include <QApplication>
#include <QIODevice>
#include <QObject>
#include <QAuthenticator>
#include <QString>
#include <QDataStream>
#include <QByteArray>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>
#include <QMetaMethod>
#include <QMetaEnum>

class c_loggedUser : public m_loggedUser /*, public cv_ProcessData */
{
    Q_OBJECT
public:
    c_loggedUser();
    ~c_loggedUser();

    QAuthenticator getAuthenticator();
    void setUpThread();

    c_loggedUserThread*thread() const;
    void setThread(std::unique_ptr<c_loggedUserThread> newThread);

    QMap<QString, QVariant> getUserProperties();
    QMap<QString, QVariant> getEmployeeProperties();

    QList<myStructures::myLog> *getDbLogs();
    void setDbLogs(QList<myStructures::myLog> &newDbLogs);
    void setDbLogs(QList<QMap<QString, QVariant>> logs);

public slots:
    void forceLogOut();
    void setProperties(QMap<QString, QVariant> userInfo);

private:
    std::unique_ptr<c_loggedUserThread> mThread;

    QList<myStructures::myLog> dbLogs;

private slots:
    void cleanUpThread();

signals:
    void threadAssigned(MyThread * thread);
    void logOutUser();
    void aboutToLogOut();
    void getLogsSignal(qint32 id, QString name, QString password);
    void propertiesSaved();
    void passProperties(QMap<QString, QVariant> userInfo);
    void logsSaved();
    void passLogs(QList<myStructures::myLog> logs);
};

#endif // C_LOGGEDUSER_H
