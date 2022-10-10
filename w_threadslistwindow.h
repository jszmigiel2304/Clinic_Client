#ifndef W_THREADSLISTWINDOW_H
#define W_THREADSLISTWINDOW_H

#include "mythread.h"
#include "c_threadcontroller.h"
#include "w_threadinfogroupbox.h"

#include <QMainWindow>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QString>

namespace Ui {
class w_ThreadsListWindow;
}

class w_ThreadsListWindow : public QMainWindow
{
    Q_OBJECT

public:
    static w_ThreadsListWindow *Instance(c_ThreadController * threadCtrlr = nullptr, QWidget *parent = nullptr);
    ~w_ThreadsListWindow();

public slots:
    void refresh();

private:
    explicit w_ThreadsListWindow(c_ThreadController * threadCtrlr = nullptr, QWidget *parent = nullptr);
    Ui::w_ThreadsListWindow *ui;

    c_ThreadController * threadCtr;

};

#endif // W_THREADSLISTWINDOW_H
