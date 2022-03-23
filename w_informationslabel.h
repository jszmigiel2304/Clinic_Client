#ifndef W_INFORMATIONSLABEL_H
#define W_INFORMATIONSLABEL_H

#include <QObject>
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QEvent>
#include <QFont>

class w_informationsLabel : public QWidget
{
    Q_OBJECT
public:
    static w_informationsLabel *Instance();
    ~w_informationsLabel();
    void setInformationText(QString text);

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent* evt);

private:
    explicit w_informationsLabel(QWidget *parent = nullptr);

    QLabel * informationLabel;

};

#endif // W_INFORMATIONSLABEL_H
