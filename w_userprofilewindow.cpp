#include "w_userprofilewindow.h"
#include "ui_w_userprofilewindow.h"

w_UserProfileWindow::w_UserProfileWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::w_UserProfileWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    connect(this->ui->b_more_logs_button, SIGNAL(clicked(bool)), this, SLOT(on_more_logs_button_clicked(bool)));

    waitingLoop = new c_waitingLoop::c_waitingLoop();

    this->setEnabled(true);
    ui->w_mask->setEnabled(false);
    ui->w_mask->hide();
}

void w_UserProfileWindow::on_more_logs_button_clicked(bool checked)
{

}

void w_UserProfileWindow::refreshUserInfo()
{
    ui->l_username->setText( userProperties["name"].toString() );
    ui->l_id->setText( QString("%1").arg(userProperties["id"].toInt()) );
    ui->l_name->setText( userProperties["name"].toString() );
    ui->l_role->setText( userProperties["role"].toString() );
    ui->l_create_date->setText( userProperties["create_date"].toString() );
    ui->l_email->setText( userProperties["email"].toString() );
    ui->l_is_blocked->setText( userProperties["is_blocked"].toBool() ? QString("TAK") : QString("NIE") );
    ui->l_verified->setText( userProperties["is_verified"].toBool() ? QString("TAK") : QString("NIE") );
    ui->l_blocked_date->setText( userProperties["blocked_date"].toString() );
    ui->l_verified_date->setText( userProperties["verified_date"].toString() );
    ui->l_is_logged->setText( userProperties["is_logged"].toBool() ? QString("TAK") : QString("NIE"));
}

void w_UserProfileWindow::refreshEmployeeInfo()
{   
    ui->l_names->setText( QString("%1 %2").arg(employeeProperties["name"].toString(), employeeProperties["second_name"].toString()) );
    ui->l_last_name->setText( employeeProperties["last_name"].toString() );
    ui->l_pesel->setText( employeeProperties["pesel"].toString() );

    switch( employeeProperties["gender"].toChar().toLatin1()){
    case 'K': { ui->l_gender->setText( QString("Kobieta") ); break;}
    case 'k': { ui->l_gender->setText( QString("Kobieta") ); break;}
    case 'M': { ui->l_gender->setText( QString("Mężczyzna") ); break;}
    case 'm': { ui->l_gender->setText( QString("Mężczyzna") ); break;}
    case '0': { ui->l_gender->setText( QString("Nie określono") ); break;}
    default : { ui->l_gender->setText( QString("Brak") ); break;}
    }

    ui->l_phone_numbers->setText( QString("%1, %2").arg(employeeProperties["phone_number"].toString(), employeeProperties["phone_number_2"].toString())  );
    ui->l_address_living->setText( QString("%1\n%2, %3").arg(
                                                    employeeProperties["address_living"].toString(),
                                                    employeeProperties["postal_code_living"].toString(),
                                                    employeeProperties["city_living"].toString()
                                                        ) );
    ui->l_address_contact->setText( QString("%1\n%2, %3").arg(
                                                    employeeProperties["address_contact"].toString(),
                                                    employeeProperties["postal_code_contact"].toString(),
                                                    employeeProperties["city_contact"].toString()
                                                        ) );
    ui->l_position->setText( employeeProperties["position"].toString() );
    ui->l_employment_date->setText(employeeProperties["employment_date"].toString());
    ui->l_supervisor->setText( QString("%1 %2").arg(employeeProperties["supervisor_name"].toString(), employeeProperties["supervisor_last_name"].toString()) );
    ui->l_salary_base->setText( QString("%1").arg( employeeProperties["salary_base"].toDouble() ));
    ui->l_salary_bonus->setText( QString("%1").arg( employeeProperties["salary_bonus"].toDouble()) );

}

void w_UserProfileWindow::refreshLogs()
{
    for( int i = 0; i < 11 && i < Logs.size(); i++) {
        QLabel * l_log = new QLabel(ui->w_logs_container);
        l_log->setStyleSheet( QString("color: rgb(221, 221, 221);") );
        l_log->setGeometry( 0, 25 * (i + 1), 255, 25 );
        l_log->setIndent(5);
        l_log->setText( QString("%1\t\t(%2)").arg(Logs[i].log_text, Logs[i].time.toString()) );
        l_log->show();
    }
}

void w_UserProfileWindow::processing(QString text)
{
    ui->l_processingText->setText(text);
    this->setEnabled(false);
    ui->w_mask->setEnabled(true);
    ui->w_mask->show();
}

void w_UserProfileWindow::processingFinished(int code)
{
    this->setEnabled(true);
    ui->w_mask->setEnabled(false);
    ui->w_mask->hide();

    this->refresh();
}

void w_UserProfileWindow::processingFault(QString text)
{
    ui->l_processingText->setText(text);
    this->setEnabled(true);
    ui->w_mask->setEnabled(false);
    ui->w_mask->hide();
}


w_UserProfileWindow::~w_UserProfileWindow()
{
    if(waitingLoop != nullptr)
        delete waitingLoop;
    delete ui;
}

void w_UserProfileWindow::setProperties(QMap<QString, QVariant> userProperties, QMap<QString, QVariant> employeeProperties, bool refresh)
{
    this->userProperties = userProperties;
    this->employeeProperties =employeeProperties;

    if(refresh)
        this->refresh();
}

void w_UserProfileWindow::setUpLoop()
{
    waitingLoop->setExpireTime(15000);
}

c_waitingLoop::c_waitingLoop *w_UserProfileWindow::getWaitingLoop() const
{
    return waitingLoop;
}

void w_UserProfileWindow::setWaitingLoop(c_waitingLoop::c_waitingLoop *newWaitingLoop)
{
    waitingLoop = newWaitingLoop;
}

void w_UserProfileWindow::setUserProperties(QMap<QString, QVariant> userProperties)
{
    this->userProperties = userProperties;
}

void w_UserProfileWindow::setEmployeeProperties(QMap<QString, QVariant> employeeProperties)
{
    this->employeeProperties = employeeProperties;
}

void w_UserProfileWindow::setLogs(QList<myStructures::myLog> list)
{
    this->Logs = list;
}

void w_UserProfileWindow::refresh()
{
    this->refreshUserInfo();
    this->refreshEmployeeInfo();
    this->refreshLogs();
}

void w_UserProfileWindow::refreshProperties()
{
    //emit getUserPanelProperties();
    emit getUserPanelProperties(&userProperties, &employeeProperties, &Logs);

    if(waitingLoop->getConditionsNumber() > 0)
        waitingLoop->startExec();
}

void w_UserProfileWindow::closeEvent(QCloseEvent *event)
{
    this->Logs.clear();
    this->employeeProperties.clear();
    this->userProperties.clear();

    event->accept();
}
