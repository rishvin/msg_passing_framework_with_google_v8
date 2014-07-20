#include "common.hpp"

SigHandler Process::pSigUser1 = NULL;
SigHandler Process::pSigUser2 = NULL;

static Lock g_WriteLock;
template<> LogWriter *LogWriterService::instance = NULL;
template<> Lock & LogWriterService::lock = g_WriteLock;


typedef void(*_SigHandler)(int); 

void _RegSignal(int _iSignal, _SigHandler _sigHandler)
{
    struct sigaction _sSigAction;
    _sSigAction.sa_handler = _sigHandler;
    _sSigAction.sa_flags = 0;
    sigemptyset(&_sSigAction.sa_mask);
    sigaction(_iSignal, &_sSigAction, NULL);
}

Process::Process(const char * cProcessName)
    :sPname(cProcessName)
{
    int iErr;
    LogWriter *pLogWriter = LogWriterService::Create();
    if(NULL == pLogWriter)
    {
        throw TRACE_INIT_EXCEPTION;
    }
    iErr = pLogWriter->Open();
    if(0 != iErr)
    {
        LogWriterService::Delete();
        throw TRACE_INIT_EXCEPTION;
    }
    // Registration of user1 and user2 signal handler
    _RegSignal(SIGUSER1, (_SigHandler)(Process::BaseSigUserHandler));
    _RegSignal(SIGUSER2, (_SigHandler)(Process::BaseSigUserHandler));
}

Process::~Process()
{
    LogWriterService::Delete();
}

pid_t Process::SpawnProcess(const char *cPath, char **ppArgv)
{	
    pid_t pid = fork();
    if(0 == pid)
    {
        execv(cPath, ppArgv);		
    }
    else
    {
        vChildPid.push_back(pid);       
    }

    return pid;
}

void Process::RegisterSigHandler(eUserSig eSigType, SigHandler pHand)
{
    if(NULL == pHand)
    {
        TRACE(this, "SigUser Handler", "NULL");
        return;
    }
    if(SIGUSER1 == eSigType)
    {
        pSigUser1 = pHand;
    }
    else
	if(SIGUSER2 == eSigType)
    {
        pSigUser2 = pHand;
    }
	else
	{
        _RegSignal(eSigType, (_SigHandler)(pHand)); // to be handled by derived class only
	}
}

void Process::BaseSigUserHandler(int iSignal)
{
    if((SIGUSER2 == iSignal)
    &&(NULL != pSigUser2))
    {
        pSigUser2(static_cast<eUserSig>(iSignal));
    }
    else
    {
        pSigUser1(static_cast<eUserSig>(iSignal));
    }
}

pid_t Process::GetProcessId() const
{
    return getpid();
}

const char * Process::GetName() const
{
    return sPname.c_str();
}

std::vector<pid_t> & Process::GetChildPid()
{
    return vChildPid;
}

void Process::InternalWait()
{
    MutexGuard mutex(lock);
	condMutex.Wait(lock);
}

void Process::InternalSignal()
{
    MutexGuard mutex(lock);
	condMutex.Signal();
}

void Process::InternalBroadcast()
{
    MutexGuard mutex(lock);
	condMutex.Broadcast();
}
