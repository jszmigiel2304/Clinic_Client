#include "c_logscontroller.h"

c_LogsController *c_LogsController::Instance()
{
    static c_LogsController * instance = nullptr;
    if ( instance == nullptr ) {
        instance = new c_LogsController();
    }
    return instance;
}

void c_LogsController::saveLogToFile(QString sender, QString notes, QByteArray data)
{
    if (!logsFile.open(QFile::WriteOnly | QIODevice::Append)) {
        return;
     }

     QTextStream is(&logsFile);

     is << QString("\n<begin \" %1 \"  \" %2 \" >").arg(sender).arg(QDateTime::currentDateTime().toString());
     is << QString("\n<note \" %1 \" >").arg(notes);

     if(data.size() > 0)
     {
         QString hexData = data.toHex();

         for(int i = 0; i < hexData.length(); i++)
         {
             if(i % 16 == 0)
                 is << "\n";
             if(i % 2 == 0)
                 is << " ";

             is << hexData[i];
         }
     }

     is << QString("\n<end>");

     logsFile.close();
     return;
}

c_LogsController::c_LogsController(QObject *parent) : QObject(parent)
{
    QDateTime current(QDateTime::currentDateTime());

    identifier = QString("%1_%2_%3_%4_%5").arg(current.date().year())
            .arg(current.date().month())
            .arg(current.date().day())
            .arg(current.time().hour())
            .arg(current.time().minute());

    logsFile.setFileName(QString("logs//LOGS_ClinicC_%1.txt").arg(identifier));

}
