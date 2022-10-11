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

public slots:
    void moduleButtonPressed(QString action);
    void moduleButtonPressedDoFunction(QString target, QStringList params);
    void moduleButtonPressedShow(QString target, QStringList params);
    void moduleButtonPressedProcess(QString target, QStringList params);


    void Processing(QString processText = QString(""));
    void ProcessingFinished();
    void ProcessingFault(QString processText = QString(""));

    void IdleDetected(QString username = QString());
    void unlockWindow();

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
};

#endif // MAINWINDOW_H
