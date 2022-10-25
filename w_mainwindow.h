#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "i_watched.h"
#include "i_observer.h"
#include "w_toppanel.h"
#include "w_userpanel.h"
#include "w_modulesscrollarea.h"
#include "c_modulescontroller.h"
#include "c_mystructures.h"
#include "w_informationslabel.h"
#include "c_sessioncontroller.h"
#include "w_settingswindow.h"

#include <QMainWindow>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QWidget>

namespace Ui {
class w_MainWindow;
}

class w_MainWindow : public QMainWindow, public i_Observer
{
    Q_OBJECT
public:    
    static w_MainWindow * Instance();
    void loadSettings(QMap<QString, QVariant> settings);
    ~w_MainWindow();
    void refresh();

    bool getMinimizeToTrayOnClose() const;
    void setMinimizeToTrayOnClose(bool value);

    bool getMinimizeToTrayOnStart() const;
    void setMinimizeToTrayOnStart(bool value);

    bool eventFilter(QObject *watched, QEvent *event) override;
    w_userPanel *getUserPanel() const;

    void newAddWindow(QWidget * window);

public slots:
    void moduleButtonPressed(QString action);
    void moduleButtonPressedDoFunction(QString target, QMap<QString, QString> params);
    void moduleButtonPressedShow(QString target, QMap<QString, QString> params);
    void moduleButtonPressedProcess(QString target, QMap<QString, QString> params);


    void Processing(QString processText = QString(""));
    void ProcessingFinished();
    void ProcessingFault(QString processText = QString(""));

    void IdleDetected(QString username = QString());
    void unlockWindow();

    void unLockModulesScrollArea(QString n, QString r);
    void lockModulesScrollArea();

    void closeAllAddWindows();
    void removeAddWindow(QWidget * window);

private:
    explicit w_MainWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent * event) override;
    Ui::w_MainWindow *ui;

    void createLayout();
    void createModulesScrollArea();
    void createTopPanel();
    void createInformationsLabel();
    void createProcessingMask();


    bool minimizeToTrayOnClose;
    bool minimizeToTrayOnStart;

    w_modulesScrollArea * modulesScrollArea;
    w_informationsLabel * informationLabel;
    w_topPanel * topPanel;
    QWidget * mask;
    QLabel * processingtextLabel;

    c_modulesController * mCtr;    

    QTimer * processingTimer;

    QList<QWidget *> addWindows;

private slots:
    void processingTimerFinished();


signals:
    void stateChanged(Qt::WindowStates);
    void myStateChanged(Qt::WindowStates, bool);
    void modulePressed(QString action);
    void actionDetected();
    void logOutModuleClicked(qint32 id, QString name, QString password);
    void settingsWindowModuleClicked();
    void loggingWindowModuleClicked();
    void closeApplicationButtonClicked();
    void userProfileButtonClicked();
    void processAppButtonClicked(QString target, QMap<QString, QString> params);
};

#endif // MAINWINDOW_H
