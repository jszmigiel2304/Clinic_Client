#ifndef C_LOGSCONTROLLER_H
#define C_LOGSCONTROLLER_H

#include <QObject>
#include <QWidget>
#include <QDateTime>
#include <QFile>
#include <QByteArray>
#include <QTextStream>

class c_LogsController : public QObject
{
    Q_OBJECT
public:
    static c_LogsController *Instance();


public slots:
    void saveLogToFile(QString sender, QString notes, QByteArray data);


private:
    explicit c_LogsController(QObject *parent = nullptr);

    QString identifier;
    QFile logsFile;

signals:

};

#endif // C_LOGSCONTROLLER_H
