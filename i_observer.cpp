#include "i_observer.h"

i_Observer::i_Observer()
{

}

void i_Observer::AddWatchedObject(QString name, i_Watched *object)
{
    this->watchedObjectsList.insert(name, object);
}

void i_Observer::ShareWatchedObject(QString sharedObjectName, i_Observer *object)
{
    object->AddWatchedObject(sharedObjectName, this->watchedObjectsList[sharedObjectName]);
}

void i_Observer::DeleteWatchedObject(QString name)
{
    this->watchedObjectsList.remove(name);
}

QMap<QString, QVariant> i_Observer::GetWatchedObjectProperties(QString name, QString sharedData)
{
    return this->watchedObjectsList[name]->ShareProperties(sharedData);
}

QString i_Observer::GetWatchedObjectMessage(QString name)
{
    return this->watchedObjectsList[name]->ShareMessege();
}
