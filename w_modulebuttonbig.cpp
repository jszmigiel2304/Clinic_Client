#include "w_modulebuttonbig.h"

w_moduleButtonBig::w_moduleButtonBig(QWidget *parent)
{
    setParent(parent);

    this->setGeometry(0,0,220,220);
    this->setMaximumSize(QSize(220,220));
    this->setMinimumSize(QSize(220,220));

    this->setMouseTracking(true);

    this->CreateToolButton();
    this->CreateNameLabel();

    connect(toolButton, SIGNAL(clicked(bool)), this, SLOT(toolButtonclicked(bool)));
}

w_moduleButtonBig::~w_moduleButtonBig()
{
    delete toolButton;
    delete nameLabel;
}

QPushButton *w_moduleButtonBig::getToolButton() const
{
    return toolButton;
}

void w_moduleButtonBig::loadButton()
{
    QIcon ico(getIcon());
    this->toolButton->setIcon(ico);

    this->nameLabel->setText(getDisplayedName());


}

bool w_moduleButtonBig::eventFilter(QObject *, QEvent *evt)
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

void w_moduleButtonBig::enterEvent(QEnterEvent *)
{
    this->setCursor(Qt::PointingHandCursor);
        toolButton->setStyleSheet("QPushButton {"
                                  "margin: 0px;"
                                  "background-color: #E6E6FA;"
                                  "border-style: solid;"
                                  "border-color: #000000;"
                                  "border-radius: 5px;"
                                  "border-width: 2px;"
                                  "}");

        showNameLabel();
        w_informationsLabel::Instance()->setInformationText(getDescription());
}

void w_moduleButtonBig::leaveEvent(QEvent *)
{
    this->setCursor(Qt::ArrowCursor);
    toolButton->setStyleSheet("QPushButton {"
                              "margin: 0px;"
                              "background-color: #F0FFF0;"
                              "border-style: solid;"
                              "border-radius: 5px;"
                              "border-width: 1px;"
                              "border-color: #808080;"
                              "}");

    hideNameLabel();
    w_informationsLabel::Instance()->setInformationText("");
}

void w_moduleButtonBig::mouseReleaseEvent(QMouseEvent *)
{
    emit moduleClicked(getPressedAction());
}

void w_moduleButtonBig::showNameLabel()
{
    //QString str = nameLabel->text();
    this->nameLabel->setVisible(true);
}

void w_moduleButtonBig::hideNameLabel()
{
    this->nameLabel->setVisible(false);
}

void w_moduleButtonBig::CreateToolButton()
{
    this->toolButton = new QPushButton(this);
    this->toolButton->setEnabled(true);
    this->toolButton->setMouseTracking(true);
    this->toolButton->setGeometry(0,0, 220, 220);
    this->toolButton->setStyleSheet("QPushButton {"
                                    "margin: 0px;"
                                    "background-color: #F0FFF0;"
                                    "border-style: solid;"
                                    "border-radius: 5px;"
                                    "border-width: 1px;"
                                    "border-color: #808080;"
                                    "}");
    this->toolButton->setIconSize(QSize(150,150));
    this->toolButton->installEventFilter(this);
}

void w_moduleButtonBig::CreateNameLabel()
{
    this->nameLabel = new QLabel(this);
    this->nameLabel->setMouseTracking(true);
    this->nameLabel->setGeometry(0,180, 220, 40);
    this->nameLabel->setEnabled(true);
    QFont font("Tahoma", 12, QFont::Bold);
    this->nameLabel->setFont(font);
    this->nameLabel->setStyleSheet("QLabel {"
                                   "color: white;"
                                   "background-color: rgba(0,0,0,150);"
                                   "}");
    this->nameLabel->setVisible(false);
    this->nameLabel->setAlignment(Qt::AlignCenter);
    this->nameLabel->installEventFilter(this);
}

void w_moduleButtonBig::toolButtonclicked(bool)
{
    emit moduleClicked(getPressedAction());
}

