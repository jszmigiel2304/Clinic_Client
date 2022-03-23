#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "i_watched.h"
#include "i_observer.h"
#include "w_toppanel.h"
#include "w_modulesscrollarea.h"
#include "c_modulescontroller.h"
#include "c_mystructures.h"
#include "w_informationslabel.h"

#include <QMainWindow>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

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

public slots:
    void moduleButtonPressed(QString action);

private:
    explicit w_MainWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent * event);
    Ui::w_MainWindow *ui;

    void createLayout();
    void createModulesScrollArea();
    void createTopPanel();
    void createInformationsLabel();


    bool minimizeToTrayOnClose;
    bool minimizeToTrayOnStart;

    w_modulesScrollArea * modulesScrollArea;
    w_informationsLabel * informationLabel;
    w_topPanel * topPanel;

    c_modulesController * mCtr;


signals:
    void stateChanged(Qt::WindowStates);
    void myStateChanged(Qt::WindowStates, bool);
    void modulePressed(QString action);
};

#endif // MAINWINDOW_H
