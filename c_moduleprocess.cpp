#include "c_moduleprocess.h"

c_moduleProcess::c_moduleProcess(QObject *parent) : QProcess(parent)
{
    connect(this, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT());
    connect(this, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT());
    connect(this, SIGNAL(readyReadStandardError()), this, SLOT());
    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT());
    connect(this, SIGNAL(started()), this, SLOT());
    connect(this, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT());

    connect(this, SIGNAL(aboutToClose()), this, SLOT());
    connect(this, SIGNAL(bytesWritten(qint64)), this, SLOT());
    connect(this, SIGNAL(channelBytesWritten(int, qint64)), this, SLOT());
    connect(this, SIGNAL(channelReadyRead(int)), this, SLOT());
    connect(this, SIGNAL(readChannelFinished()), this, SLOT());
    connect(this, SIGNAL(readyRead()), this, SLOT());

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
}

c_moduleProcess::~c_moduleProcess()
{

}

void c_moduleProcess::moduleProcessErrorOccurred(ProcessError error)
{

}

void c_moduleProcess::moduleProcessFinished(int exitCode, ExitStatus exitStatus)
{

}

void c_moduleProcess::moduleProcessReadyReadStandardError()
{

}

void c_moduleProcess::moduleProcessReadyReadStandardOutput()
{

}

void c_moduleProcess::moduleProcessStarted()
{

}

void c_moduleProcess::moduleProcessStateChanged(ProcessState newState)
{

}

void c_moduleProcess::moduleProcessAboutToClose()
{

}

void c_moduleProcess::moduleProcessBytesWritten(qint64 bytes)
{

}

void c_moduleProcess::moduleProcessChannelBytesWritten(int channel, qint64 bytes)
{

}

void c_moduleProcess::moduleProcessChannelReadyRead(int channel)
{

}

void c_moduleProcess::moduleProcessReadChannelFinished()
{

}

void c_moduleProcess::moduleProcessReadyRead()
{

}
