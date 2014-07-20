#ifndef _LOG_HPP_
#define _LOG_HPP_

class LogWriter : public Thread
{
    public:
        LogWriter();
        ~LogWriter();
        int Open();
        int Write(std::stringstream &sStream);
        int Run();
    private:
        CondMutex condMutex;
        int _iErr;
        File logFile;
        Buffer<std::string> logBuffer;
    
};

class LogReader : public Thread	
{
    public:
		LogReader(Buffer<std::string> &logBuffer);
        int Open();
		int Run();
		int NotifyRecv();
    private:
        int _iErr;
        File logFile;
		Buffer<std::string> &logBuffer;
};

#endif
