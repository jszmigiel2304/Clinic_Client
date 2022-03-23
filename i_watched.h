#ifndef I_WATCHED_H
#define I_WATCHED_H

#include <QMap>
#include <QVariant>

class i_Watched
{
public:
    i_Watched();

    virtual QMap<QString, QVariant> ShareProperties(QString sharedData = "all");
    virtual QString ShareMessege();
    virtual void UpdateProperties(QMap<QString, QVariant> map);

};

#endif // I_WATCHED_H
