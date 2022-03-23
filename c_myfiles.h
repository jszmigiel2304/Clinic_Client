#ifndef C_MYFILES_H
#define C_MYFILES_H

#include <QObject>

class c_myFiles : public QObject
{
    Q_OBJECT
public:
    static c_myFiles *Instance();
    ~c_myFiles();

    QString getConfigFilePath() const;
    void setConfigFilePath(const QString &value);

    QString getCssFilePath() const;
    void setCssFilePath(const QString &value);

signals:

public slots:

private:
    explicit c_myFiles(QObject *parent = nullptr);
    QString configFilePath;
    QString cssFilePath;
};

#endif // C_MYFILES_H
