#include "w_modulesscrollarea.h"

w_modulesScrollArea::w_modulesScrollArea()
{
    this->setScrollBars();
    this->setEnabled(true);


    this->setMouseTracking(true);
    this->setFrameShape(QFrame::NoFrame);

    this->areaContent = new QWidget();
    this->setWidget(this->areaContent);

    this->contentLayout = new QGridLayout();
    areaContent->setLayout(contentLayout);

}

w_modulesScrollArea::~w_modulesScrollArea()
{
    delete contentLayout;
    delete areaContent;
}

void w_modulesScrollArea::loadModules(QList<myStructures::moduleInfo> modulesProperties)
{
    QListIterator<myStructures::moduleInfo> iterator(modulesProperties);

    while(iterator.hasNext())
    {
        this->modules.push_back(iterator.next());
    }
}

void w_modulesScrollArea::refresh()
{
    this->createLayout();
}


bool w_modulesScrollArea::eventFilter(QObject *, QEvent *evt)
{
    if (evt->type() == QEvent::Wheel)
    {
        evt->accept();
    } else if (evt->type() == QEvent::Enter)
    {
        evt->accept();
    } else if (evt->type() == QEvent::Leave)
    {
        evt->accept();
    }



    return false;
}

void w_modulesScrollArea::moduleButtonPress(QString moduleAction)
{
    emit ButtonPressed(moduleAction);
}

void w_modulesScrollArea::wheelEvent(QWheelEvent *event)
{
    if(!event->angleDelta().isNull())
    {
        if(event->angleDelta().y() > 0)
        {
            this->horizontalScrollBar()->setValue( this->horizontalScrollBar()->value() - 50 );
        } else if(event->angleDelta().y() < 0)
        {
            this->horizontalScrollBar()->setValue( this->horizontalScrollBar()->value() + 50 );
        }
    }

    event->accept();
}

void w_modulesScrollArea::enterEvent(QEnterEvent *)
{
    this->horizontalScrollBar()->show();
}

void w_modulesScrollArea::leaveEvent(QEvent *)
{
    this->horizontalScrollBar()->hide();
}

void w_modulesScrollArea::createLayout()
{
    c_moduleButtonsFactory *buttonFactory = new c_moduleButtonsFactory();

    int height = 460;
    int b = modules.count() % 2 == 0 ? 0 : 1;
    int width = 90 + ( 220 * ( b + ( modules.count() / 2 ) ) );
    areaContent->setGeometry(0,0, width, height);

    for(int i = 0; i < modules.count(); i++)
    {
        w_moduleButton * moduleButton;

        if( ( moduleButton = buttonFactory->getButton(myTypes::BIG_BUTTON) ) != nullptr)
        {
                moduleButton->setName(modules[i].name);
                moduleButton->setDisplayedName(modules[i].displayedName);
                moduleButton->setIcon(modules[i].icon);
                moduleButton->setDescription(modules[i].description);
                moduleButton->setPressedAction(modules[i].pressedAction);
                moduleButton->loadButton();

                connect(moduleButton, SIGNAL(moduleClicked(QString) ), this, SLOT(moduleButtonPress(QString) ) );

                this->contentLayout->addWidget(moduleButton, i % 2, static_cast<int>(i/2), 1, 1);
        }
    }

    delete buttonFactory;
}

void w_modulesScrollArea::setScrollBars()
{
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    this->horizontalScrollBar()->setStyleSheet("QScrollBar::handle:horizontal {"
                                               "background: #F0FFF0;"
                                               "border-radius: 8px;"
                                               "}"
                                               "QScrollBar::left-arrow:horizontal, QScrollBar::right-arrow:horizontal {"
                                               "border: none;"
                                               "color: none;"
                                               "background: none;"
                                               "}"
                                               "QScrollBar::add-line:horizontal {"
                                               "border: none;"
                                               "background: none;"
                                               "}"
                                               "QScrollBar::sub-line:horizontal {"
                                               "background: none;"
                                               "border: none;"
                                               "}");

}
