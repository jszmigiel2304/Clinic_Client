#ifndef W_MODULESSCROLLAREA_H
#define W_MODULESSCROLLAREA_H

#include "w_modulebutton.h"
#include "i_observer.h"
#include "c_mystructures.h"
#include "c_modulebuttonsfactory.h"

#include <QObject>
#include <QWidget>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QGridLayout>
#include <QEvent>
#include <QEnterEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include <QList>

class w_modulesScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    w_modulesScrollArea();
    ~w_modulesScrollArea();
    void loadModules(QList<myStructures::moduleInfo> modulesProperties);
    void refresh();

    bool eventFilter(QObject* obj, QEvent* evt);

public slots:
    void moduleButtonPress(QString moduleAction);


signals:
    void ButtonPressed(QString moduleAction);

protected:
    void wheelEvent(QWheelEvent* event);
    void enterEvent(QEnterEvent* evt);
    void leaveEvent(QEvent* evt);


private:
    void createLayout();

    QWidget * areaContent;
    QGridLayout * contentLayout;

    QList<w_moduleButton *> moduleButtons;
    QList<myStructures::moduleInfo> modules;

    void setScrollBars();
};

#endif // W_MODULESSCROLLAREA_H
