#ifndef _PROCESS_HPP_
#define _PROCESS_HPP_

enum eUserSig
{
    SIGUSER1 = SIGUSR1,
    SIGUSER2 = SIGUSR2,
    SIGINTR  = SIGINT,
};


typedef void(*SigHandler)(eUserSig);

class Process
{
    public:
        Process(const char *cProcessName);
		virtual ~Process();
        pid_t SpawnProcess(const char *cPath, char **ppArgv);
        virtual int Main(int iArgc, char **ppArgv) = 0;
        void RegisterSigHandler(eUserSig eSigType, SigHandler pHand); 
        static void BaseSigUserHandler(int iSignal);
        const char *GetName() const;
        pid_t GetProcessId() const;
        std::vector<pid_t> & GetChildPid();
	    void InternalWait();
		void InternalSignal();
		void InternalBroadcast();
    private:
		CondMutex condMutex;
		Lock lock;
        static SigHandler pSigUser1, pSigUser2;
        std::vector<pid_t> vChildPid;
        std::string sPname;
};

#endif
