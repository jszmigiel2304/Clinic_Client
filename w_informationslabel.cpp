#include "w_informationslabel.h"

w_informationsLabel *w_informationsLabel::Instance()
{
    static w_informationsLabel * instance = nullptr;
    if ( instance == nullptr ) {
        instance = new w_informationsLabel();
    }
    return instance;
}

w_informationsLabel::~w_informationsLabel()
{
    informationLabel->deleteLater();
}

void w_informationsLabel::setInformationText(QString text)
{
    this->informationLabel->setText(text);
}

void w_informationsLabel::resizeEvent(QResizeEvent *evt)
{
    this->informationLabel->setGeometry(0,0, this->width(), this->height());
}

w_informationsLabel::w_informationsLabel(QWidget *parent) : QWidget(parent)
{
    this->setStyleSheet("background-color: #000000;");
    this->informationLabel = new QLabel(this);
    this->informationLabel->setText("");
    this->informationLabel->setGeometry(0,0, 1000, 60);
    this->informationLabel->setEnabled(true);

    QFont font("Tahoma", 12, QFont::Normal);
    this->informationLabel->setFont(font);
    this->informationLabel->setStyleSheet("QLabel {"
                                          "color: black;"
                                          "background-color: rgba(255,255,255,255);"
                                          "}");
    this->informationLabel->setAlignment(Qt::AlignCenter);
}
