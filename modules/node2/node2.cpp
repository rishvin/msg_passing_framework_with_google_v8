#include "common.hpp"
#include "entry.hpp"
#include "node2.hpp"

ENTRY(Node2);


Node2::Node2()
    :Process("Node2")
{}

int Node2::Main(int iArgc, char **ppArgv)
{
    int iErr;

    unsigned int uAddr    = atoi(ppArgv[1]);
    unsigned short usPort = htons(atoi(ppArgv[2]));
    short sFamily         = AF_INET;
    
    SockAddr serverAddr(uAddr , usPort, sFamily);
    DispatchHandler *pDispatcher = DispatchService::Create();
    assert(NULL != pDispatcher);

    pDispatcher->Init(); // It is important to call this to start the service

    // Making two connections
    DispatchHandle dHandle1, dHandle2;
    dHandle1(serverAddr);
    dHandle2(serverAddr);

    for(int i = 1; i <= 2000 ;++i)
    {
        char aMessage[64];
        int iLen = snprintf(aMessage, sizeof(aMessage), "Test message %d", i);
        TLV tlv;
        tlv.tl.iType   = 0;
        tlv.tl.uLength = iLen;
        tlv.pcValue    = aMessage;

        unsigned int uMsgLen = 0;
        char *pMsg = NULL;
        (void)MsgOper::Create(pMsg, uMsgLen, tlv);
        if(NULL != pMsg)
        {
            if(0 == (i % 2))
            {
                pDispatcher->Send(pMsg, dHandle1);
            }
            else
            {
                pDispatcher->Send(pMsg, dHandle2);
            }
            //Testing message integrity
            TLV test;
            MsgOper::Get(pMsg, test);
            std::string str(test.pcValue, test.tl.uLength);
            TRACE(this, "Send Message", str);
        }
        else
        {
            TRACE(this, "Create msg", "FAILED");
        }
    }
    sleep(10);
    return 0;
}




