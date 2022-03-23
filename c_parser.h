#ifndef C_PARSER_H
#define C_PARSER_H

#include "c_mystructures.h"

#include <QObject>

class c_Parser : public QObject
{
    Q_OBJECT
public:
    explicit c_Parser(QObject *parent = nullptr);

public slots:
    threadData parseData(quint32 data_size, QByteArray data);

signals:

};

#endif // C_PARSER_H
