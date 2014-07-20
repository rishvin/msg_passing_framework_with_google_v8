#include "common.hpp"

static Lock g_DispachLock;
static Lock g_RecvLock;
static Lock g_ListenerLock;

template<> DispatchHandler* DispatchService::instance = NULL;
template<> Lock& DispatchService::lock = g_DispachLock;

template<> RecvHandler* RecvService::instance = NULL;
template<> Lock& RecvService::lock = g_RecvLock;


template<> ListenerHandler* ListenerService::instance = NULL;
template<> Lock& ListenerService::lock = g_ListenerLock;

DispatchHandle::DispatchHandle()
    :sock(AF_INET, SOCK_STREAM, 0)
{
}

int DispatchHandle::operator()(SockAddr &sockAddr)
{
    int iErr = sock.Open();
    if(0 != iErr)
    {
        TRACE(this, "Sock create ", iErr);
    }
    else
    {
        while(1)
        {
            iErr = sock.Connect(sockAddr);
            if(EINPROGRESS == iErr)
            {
                TRACE(this, "Connect", "PROGRESS");
                poll(NULL,0,100); // Wait for some 100 ms
                continue;
            }
            else
            {
                break;
            }
        }
        if((EISCONN != iErr) 
        &&(0 != iErr))
        {
            TRACE(this, "Connect", iErr);
        }
        else
        {
            TRACE(this, "Connect", "ESTABLISHED");
        }
    }
    return iErr;
}


DispatchHandler::DispatchHandler()
    :Thread("DispatchHandler")
{
}

int DispatchHandler::Init()
{
    Create();
    return 0;
}

int DispatchHandler::Run()
{
    int iErr;
    while(1)
    {
        DispatchBuffer *pData = NULL;
        {
            MutexGuard mutex(dispatchBuffer);
            if(true == dispatchBuffer.IsEmpty())
            {
                //cerr<<endl<<"Dispach buffer empty";
                continue;
            }
            pData = dispatchBuffer.PopFront();
        }
        // Currently we dont have any other method to get the ret code

        DispatchHandle *pHandle = pData->pHandle;
        Socket &sock = pHandle->sock;
        TLV *pTLV = (TLV *)pData->pMsg;
        unsigned int uLen = pTLV->tl.uLength + MSG_HEADER_SIZE;
        size_t iSendLen;
        iErr = sock.Send(pTLV, uLen, 0, iSendLen);
		if(iErr == 0)
		{
            TRACE(this, "Send", "SUCCESS");
		}
		else
		{
		    TRACE(this, "Send",iErr);
		}
    }
}

int DispatchHandler::Send(char *pMsg, DispatchHandle &handle)
{
    DispatchBuffer *pData = new DispatchBuffer();
    memset(pData, 0 , sizeof(DispatchBuffer));
    pData->pHandle = &handle;
    pData->pMsg = pMsg;
    MutexGuard mutex(dispatchBuffer);
    dispatchBuffer.PushBack(pData);
    return 0;
}

RecvHandler::RecvHandler(MsgBuffer *pMsgBuffer)
    :pMsgBuffer(pMsgBuffer),
    Thread("RecvHandler")
{
}

int RecvHandler::Init()
{
    Create();
    return 0;
}

void RecvHandler:: RegisterInstance(Socket * pSocket)
{
    MutexGuard mutex(connBuffer);
    connBuffer.PushBack(pSocket);
    if(1 == connBuffer.Size())
    {
        // Wake the rcvr thread
    }
}

// For unregistring we need to make sure that we are not in the middle of iterating when we are
// deleteing, currenly not handling that
void RecvHandler::UnRegisterInstance(Socket * pSocket)
{
}

// Not handling duplicate fd(s)
int RecvHandler::Run()
{
    int iErr;
    while(1)
    {
        int iIndex = 0;
        int iSize = -1;
        {
            MutexGuard mutex(connBuffer);
            iSize = connBuffer.Size();
        }

        while(iIndex < iSize)
        {
            Socket * pSocket = connBuffer[iIndex]; 
            pSocket->SetMode(1);//Change the socket to non-blocking;

            //First recv the header
            TL tl = {0};
            size_t uLen = 0;
            iErr = pSocket->Recv(&tl, MSG_HEADER_SIZE, 0, uLen); //Non-blocking call
            if((0 == iErr)
            && (0 != uLen)) // Length guard to make sure connection is alive
            {
                // Recv the remaining part
                TLV tlv;
                tlv.tl = tl;
                int iStartIndex = 0;
                char acValue[tl.uLength];
                uLen = tl.uLength;
                memset(acValue, 0 , uLen);
                while(uLen)
                {
                    size_t uRecvLen = 0; 
                    iErr = pSocket->Recv(&acValue[iStartIndex], uLen, 0, uRecvLen);
                    if(0 < uRecvLen)
                    {
                        
                        iStartIndex = uRecvLen -1;
                        uLen -= uRecvLen;
                    }
                    else
                    {
                        TRACE(this, "Recv", iErr);
                        break;
                    }
                }
                tlv.pcValue = acValue;
                // Now create the message and put it in message box
                unsigned int uMsgLen = 0;
                char *pMsg = NULL;
                (void)MsgOper::Create(pMsg, uMsgLen, tlv);
                // Store the message and notify receiver
                pMsgBuffer->PushBack(pMsg);

                NotifyRecv();
            }
            else
            if((EWOULDBLOCK == iErr) 
            || (EAGAIN == iErr))
            {
                //We have nothing to recv
            }
            else  // Remove broken sessions [recvLen = 0]
            {
                connBuffer.Erase(iIndex);
                iSize = connBuffer.Size();
                continue;
            }
            ++iIndex;
        }
    }
}
            
int RecvHandler::NotifyRecv()
{
    return raise(SIGUSR1);
}


ListenerHandler::ListenerHandler(SockAddr& sockAddr)
    :sockAddr(sockAddr),
     sock(AF_INET, SOCK_STREAM, 0),
     Thread("ListenerHandler")
{
}

int ListenerHandler::Init()
{
    int iErr;
    iErr = sock.Open(0); //Blocking socket
    if(0 != iErr)
    {
        TRACE(this, "Sock create", iErr);
        return iErr;
    }

    iErr = sock.Bind(sockAddr);
    if(0 != iErr)
    {
        TRACE(this, "Sock bind", iErr);
        return iErr;
    }

    Create();
    return 0;
}

int ListenerHandler::Run()
{
    int iErr;

    RecvService recvService;
    RecvHandler *pRecvHandler = recvService.Get();
    sock.Listen(20);
    while(1)
    {
        int iRecvLen;
        Socket *pClient = NULL;
        iErr = sock.Accept(iRecvLen, pClient); 
        if(NULL == pClient)
        {
            continue;
        }
        
        pRecvHandler->RegisterInstance(pClient);
    }
}

