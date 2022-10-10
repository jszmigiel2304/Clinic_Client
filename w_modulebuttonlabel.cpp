#include "w_modulebuttonlabel.h"

w_moduleButtonLabel::w_moduleButtonLabel(QWidget *parent)
{
    setParent(parent);

//    this->setGeometry(0,0,220,220);
//    this->setMaximumSize(QSize(220,220));
//    this->setMinimumSize(QSize(220,220));

    this->setMouseTracking(true);

    this->createLabel();
}

w_moduleButtonLabel::~w_moduleButtonLabel()
{
    delete label;
}

QLabel *w_moduleButtonLabel::getLabel() const
{
    return label;
}

void w_moduleButtonLabel::setLabel(QLabel *newLabel)
{
    label = newLabel;
}

void w_moduleButtonLabel::loadButton()
{
    label->setText("Czas");
}

bool w_moduleButtonLabel::eventFilter(QObject *, QEvent *evt)
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
        this->labelClicked();
        evt->accept();
    } else if (evt->type() == QEvent::MouseButtonRelease)
    {
        evt->accept();
    }
    return false;
}

void w_moduleButtonLabel::enterEvent(QEnterEvent *)
{
    this->setCursor(Qt::PointingHandCursor);
        label->setStyleSheet("QLabel {"
                                  "margin: 0px;"
                                  "background-color: #E6E6FA;"
                                  "border-style: solid;"
                                  "border-color: #000000;"
                                  "border-radius: 2px;"
                                  "border-width: 2px;"
                                  "}");
        w_informationsLabel::Instance()->setInformationText(getDescription());
}

void w_moduleButtonLabel::leaveEvent(QEvent *)
{
    this->setCursor(Qt::ArrowCursor);
    label->setStyleSheet("QLabel {"
                              "margin: 0px;"
                              "background-color: #FFFFFF;"
                              "border-style: solid;"
                              "border-radius: 0px;"
                              "border-width: 0px;"
                              "border-color: #808080;"
                              "}");
    w_informationsLabel::Instance()->setInformationText("");
}

void w_moduleButtonLabel::createLabel()
{
    this->label = new QLabel(this);
    this->label->setMouseTracking(true);
    this->label->setEnabled(true);
    this->label->setGeometry(0,0, 60, 40);
    this->label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    this->label->setFont(QFont("Segoe", 8, QFont::Bold));
    this->label->setIndent(5);
    this->label->setStyleSheet("QLabel {"
                                    "margin: 0px;"
                                    "background-color: #FFFFFF;"
                                    "border-style: solid;"
                                    "border-radius: 0px;"
                                    "border-width: 0px;"
                                    "border-color: #808080;"
                                    "}");

    this->label->installEventFilter(this);
}

void w_moduleButtonLabel::labelClicked()
{
    emit moduleClicked(getPressedAction());
}

