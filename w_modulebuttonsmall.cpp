#include "w_modulebuttonsmall.h"

w_moduleButtonSmall::w_moduleButtonSmall(QWidget *parent)
{
    setParent(parent);

    this->setGeometry(0,0,220,220);
    this->setMaximumSize(QSize(220,220));
    this->setMinimumSize(QSize(220,220));

    this->setMouseTracking(true);

    this->CreateToolButton();    

    connect(toolButton, SIGNAL(clicked(bool)), this, SLOT(toolButtonclicked(bool)));
}

w_moduleButtonSmall::~w_moduleButtonSmall()
{
    delete toolButton;
}

QPushButton *w_moduleButtonSmall::getToolButton() const
{
    return toolButton;
}

void w_moduleButtonSmall::loadButton()
{
    QIcon ico(getIcon());
    this->toolButton->setIcon(ico);
}

bool w_moduleButtonSmall::eventFilter(QObject *, QEvent *evt)
{
    if (evt->type() == QEvent::Wheel)
    {
        evt->accept();
    } else if (evt->type() == QEvent::HoverMove)
    {
        evt->accept();
    } else if (evt->type() == QEvent::Leave)
    {
        evt->accept();
    } else if (evt->type() == QEvent::MouseButtonPress)
    {
        evt->accept();
    } else if (evt->type() == QEvent::MouseButtonRelease)
    {
        evt->accept();
    }


    return false;
}

void w_moduleButtonSmall::enterEvent(QEnterEvent *)
{
    this->setCursor(Qt::PointingHandCursor);
        toolButton->setStyleSheet("QPushButton {"
                                  "margin: 0px;"
                                  "background-color: #E6E6FA;"
                                  "border-style: solid;"
                                  "border-color: #000000;"
                                  "border-radius: 2px;"
                                  "border-width: 2px;"
                                  "}");
        w_informationsLabel::Instance()->setInformationText(getDescription());
}

void w_moduleButtonSmall::leaveEvent(QEvent *)
{
    this->setCursor(Qt::ArrowCursor);
    toolButton->setStyleSheet("QPushButton {"
                              "margin: 0px;"
                              "background-color: #FFFFFF;"
                              "border-style: solid;"
                              "border-radius: 2px;"
                              "border-width: 1px;"
                              "border-color: #808080;"
                              "}");
    w_informationsLabel::Instance()->setInformationText("");
}

void w_moduleButtonSmall::CreateToolButton()
{
    this->toolButton = new QPushButton(this);
    this->toolButton->setMouseTracking(true);
    this->toolButton->setEnabled(true);
    this->toolButton->setGeometry(0,0, 40, 40);
    this->toolButton->setStyleSheet("QPushButton {"
                                    "margin: 0px;"
                                    "background-color: #FFFFFF;"
                                    "border-style: solid;"
                                    "border-radius: 2px;"
                                    "border-width: 1px;"
                                    "border-color: #808080;"
                                    "}");
    this->toolButton->setIconSize(QSize(25,25));

    this->toolButton->installEventFilter(this);

}

void w_moduleButtonSmall::toolButtonclicked(bool)
{
    emit moduleClicked(getPressedAction());
}
