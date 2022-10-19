#ifndef C_MODULEPROCESS_H
#define C_MODULEPROCESS_H

#include <QProcess>
#include <QObject>

class c_moduleProcess : public QProcess
{
    Q_OBJECT
public:
    c_moduleProcess(QObject *parent = nullptr);
    ~c_moduleProcess();

private:

private slots:
    // from QProcess signals
    void	moduleProcessErrorOccurred(QProcess::ProcessError error);
    void	moduleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus = NormalExit);
    void	moduleProcessReadyReadStandardError();
    void	moduleProcessReadyReadStandardOutput();
    void	moduleProcessStarted();
    void	moduleProcessStateChanged(QProcess::ProcessState newState);
    //from QIODevice siganls
    void	moduleProcessAboutToClose();
    void	moduleProcessBytesWritten(qint64 bytes);
    void	moduleProcessChannelBytesWritten(int channel, qint64 bytes);
    void	moduleProcessChannelReadyRead(int channel);
    void	moduleProcessReadChannelFinished();
    void	moduleProcessReadyRead();


};

#endif // C_MODULEPROCESS_H
