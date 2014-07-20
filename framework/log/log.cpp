#include "common.hpp"

#define LOG_FILE "log"
#define MAX_READ_BLOCK 512

LogReader::LogReader(Buffer<std::string> &logBuffer)
    :logBuffer(logBuffer),
     Thread("LogReader")
{
}

int LogReader::Open()
{
    _iErr = logFile.Open(LOG_FILE, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);
    if(EEXIST == _iErr)
    {
        _iErr = logFile.Open(LOG_FILE, O_RDONLY);
    }
    if(0 == _iErr)
    {
        Create(); //Start the thread only if file is opened
    }
    return _iErr;
}

int LogReader::Run()
{
    int iErr;
	while(1)
	{
	    char cBuffer[MAX_READ_BLOCK] = {0};
		size_t ulRead = 0;
		while(1)
		{
		    //iErr = logFile.LockBytes(F_RDLCK, SEEK_CUR, 0, MAX_READ_BLOCK);
		    iErr = logFile.Read(cBuffer, MAX_READ_BLOCK, ulRead);
			//logFile.UnLockBytes(SEEK_CUR, 0, MAX_READ_BLOCK);
		    if(0 < ulRead)
		    {
		        std::string sLog;
			    sLog.assign(cBuffer, ulRead);
		        {
					MutexGuard mutex(logBuffer);
			        logBuffer.PushBack(sLog);
			    }
				NotifyRecv();
		    }
			break;
		}
	}
	return iErr;
}


int LogReader::NotifyRecv()
{
    return raise(SIGUSR1);
}


LogWriter::LogWriter()
    :Thread("LogWriter")
{
}

LogWriter::~LogWriter()
{
}
int LogWriter::Open()
{
    _iErr = logFile.Open(LOG_FILE, O_CREAT | O_RDWR | O_APPEND, S_IWUSR | S_IRUSR);
    if(EEXIST == _iErr)
    {
        _iErr = logFile.Open(LOG_FILE, O_WRONLY | O_APPEND);
    }
    if(0 == _iErr)
    {
        Create(); //Start the thread only if file is opened
    }
    return _iErr;
};

int LogWriter::Write(std::stringstream &sStream)
{
    if(0 == _iErr)
    {
        string sLog (sStream.str());
        MutexGuard mutex(logBuffer);
        logBuffer.PushBack(sLog);
        if(1 == logBuffer.Size())
        {
            condMutex.Signal();
        }
    }
    return _iErr;
}

int LogWriter::Run()
{
    size_t ulWrite;
    size_t ulLogSize;
    int iErr;
    while(1)
    {
        string sLog;
        {
            MutexGuard mutex(logBuffer);
            if(true == logBuffer.IsEmpty())
            {
                condMutex.Wait(logBuffer);
            }
            sLog = logBuffer.PopFront();
        }
        
        ulLogSize = sLog.length();
        iErr = EAGAIN;
        while(EAGAIN == iErr) // Trying to lock bytes
        {
            iErr = logFile.LockBytes(F_WRLCK, SEEK_SET, 0 , ulLogSize);
        }
        logFile.Write(sLog.c_str(), ulLogSize, ulWrite);
        logFile.UnLockBytes(SEEK_SET, 0 , ulLogSize);
    }
    return 0;
}
