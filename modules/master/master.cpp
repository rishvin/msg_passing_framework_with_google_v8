#include "common.hpp"
#include "entry.hpp"
#include "master.hpp"

MsgBuffer Master::messageBox;

ENTRY(Master);


Master::Master()
    :Process("Master")
{
}

int Master::Main(int iArgc, char **ppArgv)
{
    if(3 != iArgc)
    {
        cout<<endl<<"Usage:: master <IP> <Port>";
        return 0;
    }
    
    int iErr;
    
    unsigned int uAddr    = atoi(ppArgv[1]);
    unsigned short usPort = htons(atoi(ppArgv[2]));
    short sFamily         = AF_INET;
    SockAddr serverAddr(uAddr , usPort, sFamily);

    RegisterSigHandler(SIGUSER1, Master::SignalHandler);
	RegisterSigHandler(SIGINTR,  Master::SignalHandler);
    ListenerHandler *pListener = ListenerService::Create<SockAddr &>(serverAddr);
    assert(NULL != pListener);
    RecvHandler *pReceiver = RecvService::Create<MsgBuffer *>(&messageBox);
    assert(NULL != pReceiver);
    pListener->Init();
    pReceiver->Init();
    //SpawnProcess("node1", ppArgv);
    TRACE(this, "Master State", "IDLE");
    InternalWait();
	TRACE(this, "Master State", "DOWN");
    return 0;
}

Master::~Master()
{
    ListenerService::Delete();
	RecvService::Delete();
}


// Recv messages with user1 signalling
void Master::SignalHandler(eUserSig eSignal)
{
    switch(eSignal)
    {
        case SIGUSER1:
		{
            // We would also like to disable the user1 signal at this point to get called again.
            char *pMsg = NULL;
            {
                // We also need to iterate also 
                MutexGuard mutex(Master::messageBox);
                pMsg = Master::messageBox.PopFront();
            }
            if(NULL != pMsg)
            {
                TLV tlv;
				ProcService proc;
                MsgOper::Get(pMsg, tlv);
                string str(tlv.pcValue, tlv.tl.uLength);
                TRACE(proc.Get(), "Recvd Message", str);
                MsgOper::Destroy(pMsg);
                pMsg = NULL;
            }
            else
            {
                ProcService proc;
                TRACE(proc.Get(), "Recvd Message", "NULL");
            }
			break;
        }
		case SIGINTR:
		{
			ProcService proc;
			Master *pThis = proc.Get();
			pThis->InternalSignal();
			TRACE(pThis, "Recv Signal", "SIGINTR");
		}
			
    }
}



void Master::PrintChildPid()
{
    vector<pid_t> &vChildPid = GetChildPid();
    vector<pid_t>::iterator itr = vChildPid.begin();
    std::stringstream Buffer;

    while(vChildPid.end() != itr)
    {
        Buffer << *itr << " ";
        ++itr;
    }

    cerr<< endl<<"#### List of Child(s) ####";
    cerr<< endl<<Buffer.str();
    cerr<< endl << "##########################" << endl;
}



//ENTRY(Master);


