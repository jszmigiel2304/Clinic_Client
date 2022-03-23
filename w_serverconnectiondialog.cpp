#include "w_serverconnectiondialog.h"
#include "ui_w_serverconnectiondialog.h"

w_ServerConnectionDialog::w_ServerConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::w_ServerConnectionDialog)
{
    ui->setupUi(this);

    this->setWindowTitle(qApp->applicationName());

    QPushButton * okButton = new QPushButton();
    QPushButton * applyButton = new QPushButton();

    okButton->setText(tr("OK"));
    applyButton->setText(tr("Zastosuj"));

    okButton->setDefault(true);

    okButton->setEnabled(true);
    applyButton->setEnabled(false);
    ui->b_refresh->setEnabled(false);

    connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonPressed()));
    connect(applyButton, SIGNAL(clicked()), this, SLOT(applyButtonPressed()));
    connect(ui->b_refresh, SIGNAL(clicked()), this, SLOT(resetButtonPressed()));

    ui->buttonBox->addButton(okButton, QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(applyButton, QDialogButtonBox::ActionRole);


    ui->f_port->setValidator(new QIntValidator(0, 65535, this));

    connect(ui->f_address, SIGNAL(editingFinished()), this, SLOT(validateAddress()));
    connect(ui->f_port, SIGNAL(editingFinished()), this, SLOT(validatePort()));

    connect(ui->f_address, SIGNAL(textChanged(QString)), this, SLOT(settingsChanged(QString)));
    connect(ui->f_port, SIGNAL(textChanged(QString)), this, SLOT(settingsChanged(QString)));

    this->setTabOrder(ui->f_address, ui->f_port);
    this->setTabOrder(ui->f_port, okButton);
    this->setTabOrder(okButton, applyButton);
    this->setTabOrder(applyButton, ui->b_refresh);
    this->setTabOrder(ui->b_refresh, ui->b_test);
    this->setTabOrder(ui->b_test, ui->f_address);

    this->addStyleSheets();


}

w_ServerConnectionDialog::~w_ServerConnectionDialog()
{
    delete ui;
}

void w_ServerConnectionDialog::update()
{
    QMap<QString, QVariant> properties = this->GetWatchedObjectProperties("loggingDialog", "serverOnly");

    ui->f_address->setText(properties["address"].toString());
    ui->f_port->setText(properties["port"].toString());


    this->isChanged = false;
}

void w_ServerConnectionDialog::addStyleSheets()
{
    ui->buttonBox->buttons()[0]->setStyleSheet("QPushButton {"
                                                              "background-color: rgba(255, 255, 255, 0);"
                                                                "width: 80px;"
                                                                "height: 25px;"
                                                              "border-style: solid;"
                                                              "border-width: 1px;"
                                                              "border-radius: 3px;"
                                                              "font: 75 8pt \"Tahoma\";"
                                                              "color: rgb(0, 0, 0);"
                                                              "border-color: rgba(0, 0, 0, 255);"
                                                              "}"
                                                              "QPushButton:hover {"
                                                              "background-color: rgba(0, 0, 0, 10);"
                                                              "}"
                                                              "QPushButton:pressed {"
                                                              "background-color: rgba(0, 0, 0, 20);"
                                                              "}"
                                               "QPushButton:disabled {"
                                               "border-color: rgba(0, 0, 0, 0);"
                                               "}");

    ui->buttonBox->buttons()[1]->setStyleSheet("QPushButton {"
                                                              "background-color: rgba(255, 255, 255, 0);"
                                               "width: 80px;"
                                               "height: 25px;"
                                                              "border-style: solid;"
                                                              "border-width: 1px;"
                                                              "border-radius: 3px;"
                                                              "font: 75 8pt \"Tahoma\";"
                                                              "color: rgb(0, 0, 0);"
                                                              "border-color: rgba(0, 0, 0, 255);"
                                                              "}"
                                                              "QPushButton:hover {"
                                                              "background-color: rgba(0, 0, 0, 10);"
                                                              "}"
                                                              "QPushButton:pressed {"
                                                              "background-color: rgba(0, 0, 0, 20);"
                                                              "}"
                                               "QPushButton:disabled {"
                                               "border-color: rgba(0, 0, 0, 0);"
                                               "}");
}

void w_ServerConnectionDialog::settingsChanged(bool)
{
    this->isChanged = true;
    (ui->buttonBox->buttons()[1])->setEnabled(true);
    ui->b_refresh->setEnabled(true);
}

void w_ServerConnectionDialog::settingsChanged(QString)
{
    this->isChanged = true;
    (ui->buttonBox->buttons()[1])->setEnabled(true);
    ui->b_refresh->setEnabled(true);
}

void w_ServerConnectionDialog::validateAddress()
{
    QString address = ui->f_address->text();
    QRegularExpression ip4RegExp( "[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}" );

    if(ip4RegExp.match(address).isValid())
    {
        ui->f_address->setStyleSheet("");
    }
    else
    {
        ui->f_address->setStyleSheet("color: red;");
    }
}

void w_ServerConnectionDialog::validatePort()
{
    QString port = ui->f_port->text();

    if( port.toInt() >= 1024 && port.toInt() <= 49151   )
    {
        ui->f_port->setStyleSheet("");
    }
    else
    {
        ui->f_port->setStyleSheet("color: red;");
    }
}

void w_ServerConnectionDialog::okButtonPressed()
{
    this->isChanged = false;
    emit(close());
}

void w_ServerConnectionDialog::resetButtonPressed()
{
    this->update();

    this->isChanged = false;
    (ui->buttonBox->buttons()[1])->setEnabled(false);
    ui->b_refresh->setEnabled(false);

    this->validateAddress();
    this->validatePort();
}

void w_ServerConnectionDialog::applyButtonPressed()
{
    if( addressIsValid() && portIsValid() )
    {
        QMap<QString, QVariant> properties;

        properties.insert("address", ui->f_address->text());
        properties.insert("port", ui->f_port->text().toInt());

        this->watchedObjectsList["loggingDialog"]->UpdateProperties(properties);
        (dynamic_cast<w_LoggingDialog *>(this->watchedObjectsList["loggingDialog"]))->refresh();

        c_SettingsController settCtrl(c_myFiles::Instance()->getConfigFilePath());

        settCtrl.SaveToFile("server", properties);

        this->isChanged = false;
        ui->b_refresh->setEnabled(false);
        (ui->buttonBox->buttons()[1])->setEnabled(false);
    }
    else
    {
        QMessageBox msgBox;

        QString msg = QString("Niepoprawne dane. \nPopraw dane zaznaczone na czerwono.");

        msgBox.setText(msg);

        msgBox.exec();
    }

}

void w_ServerConnectionDialog::closeEvent(QCloseEvent *e)
{
    if(this->isChanged)
    {
        QMessageBox::StandardButton close;
          close = QMessageBox::question(this, "Zakończ", "Ustawienia zostały zmienione. Zapisać ?",
                                        QMessageBox::Yes|QMessageBox::No);
          if (close == QMessageBox::Yes) {
            this->applyButtonPressed();
              e->accept();
          } else {
            e->accept();
          }
    } else
    {
        e->accept();
    }

}

bool w_ServerConnectionDialog::addressIsValid()
{
    QString address = ui->f_address->text();
    QRegularExpression ip4RegExp( "[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}" );

    if(ip4RegExp.match(address).isValid())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool w_ServerConnectionDialog::portIsValid()
{
    QString port = ui->f_port->text();

    if( port.toInt() >= 1024 && port.toInt() <= 49151   )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void w_ServerConnectionDialog::on_b_test_clicked()
{
    c_MyConnectionTester * tester = new c_MyConnectionTester(ui->f_address->text(), quint16(ui->f_port->text().toInt()), this);

    ui->b_refresh->setEnabled(false);

    QTimer * timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshButtonOnTimeOut()));

    if(tester->TestConnection(5000))
    {
        QIcon icon(":/icons/test_ok_green.png");
        ui->b_test->setIcon(icon);
        ui->b_test->setIconSize(QSize(16,16));
        timer->start(1500);
    }
    else
    {
        QIcon icon(":/icons/test_fail.png");
        ui->b_test->setIcon(icon);
        ui->b_test->setIconSize(QSize(16,16));
        timer->start(1500);
    }


    delete tester;
}

void w_ServerConnectionDialog::refreshButtonOnTimeOut()
{
    QIcon icon(":/icons/test_connection.png");
    ui->b_test->setIcon(icon);
    ui->b_test->setIconSize(QSize(16,16));
    ui->b_test->setEnabled(true);
    delete sender();
}
