#ifndef _MASTER_HPP_
#define _MASTER_HPP_

class Master:public Process
{
    public:
        Master();
		~Master();
    int Main(int argc, char **argv);
     static void SignalHandler(eUserSig eSignal);
    void PrintChildPid();
    private:
        static MsgBuffer messageBox;
};


#endif
