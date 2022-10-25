#include "w_userprofilewindow.h"
#include "ui_w_userprofilewindow.h"

w_UserProfileWindow::w_UserProfileWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::w_UserProfileWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    connect(this->ui->b_more_logs_button, SIGNAL(clicked(bool)), this, SLOT(moreLogsButtonClicked(bool)));

    waitingLoop = new c_waitingLoop::c_waitingLoop();

    this->setEnabled(true);
    ui->w_mask->setEnabled(false);
    ui->w_mask->hide();
}

void w_UserProfileWindow::moreLogsButtonClicked(bool checked)
{
    w_moreLogsWindow * moreLogsWindow = new w_moreLogsWindow(&Logs, this);
    connect(this, SIGNAL(aboutToClose(QWidget *)), moreLogsWindow, SLOT(deleteLater()));
    moreLogsWindow->show();
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
        l_log->setGeometry( 0, 25 * (i + 1), 355, 25 );
        l_log->setIndent(5);
        l_log->setText( QString("%2\t%3\t[ %1 ]").arg( Logs[i].log_text, Logs[i].time.toString(), Logs[i].ip_address.toString() ) );
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
    emit aboutToClose( dynamic_cast<QWidget *>(this) );

    event->accept();
}

w_moreLogsWindow::w_moreLogsWindow(QList<myStructures::myLog> * Logs, QWidget *parent) : QMainWindow(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    setWindowModality(Qt::NonModal);
    setWindowTitle(QString("Clinic Clinet - more logs"));
    setStyleSheet(QString("background-color: rgb(221, 221, 221);\ncolor: rgb(0,0,0);"));
    setGeometry(0,0,450,700);
    setMinimumSize(450,700);
    setMaximumSize(450,700);

    central.setStyleSheet(QString("background-color: rgb(66, 66, 66);\ncolor: rgb(221, 221, 221);"));
    this->setCentralWidget(&central);
    setGeometry(0,0,450,690);

    scrollArea.setParent(&central);
    scrollArea.setGeometry(0,0,450,690);
    scrollArea.setStyleSheet(QString("background-color: rgb(66, 66, 66);\ncolor: rgb(221, 221, 221);"));
    scrollArea.setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea.setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    scrollArea.setWidget(&scrollAreaWidget);
    scrollAreaWidget.setGeometry(0,0,430, 25 * Logs->size() + 20);
    scrollAreaWidget.setStyleSheet(QString("background-color: rgb(66, 66, 66);\ncolor: rgb(221, 221, 221);"));

    for( int i = 0; i < Logs->size(); i++) {
        QLabel * l_log = new QLabel(&scrollAreaWidget);
        l_log->setStyleSheet( QString("color: rgb(221, 221, 221);") );
        l_log->setGeometry( 0, 25 * (i + 1), 450, 25 );
        l_log->setIndent(5);
        l_log->setText( QString("%4: %2\t%3\t[ %1 ]").arg( (*Logs)[i].log_text, (*Logs)[i].time.toString(), (*Logs)[i].ip_address.toString(), QString("%1").arg(i+1) ) );
        l_log->show();
    }

    central.show();
    scrollArea.show();
    scrollAreaWidget.show();
}

w_moreLogsWindow::~w_moreLogsWindow()
{

}
