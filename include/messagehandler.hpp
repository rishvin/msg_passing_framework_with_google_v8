#ifndef _MESSAGEHANDLER_HPP_
#define _MESSAGEHANDLER_HPP_

class DispatchHandler;
class RecvHandler;
class ListenerHandler;

typedef Buffer<Socket *> ConnBuffer;
typedef Singleton<DispatchHandler> DispatchService;
typedef Singleton<RecvHandler> RecvService;
typedef Singleton<ListenerHandler> ListenerService;

class DispatchHandle
{
    public:
        DispatchHandle();
        int operator()(SockAddr &sockAddr);
        inline const char *GetName()
        {return "DispatchHandle";}
    private:
        Socket sock;
        friend class DispatchHandler;
};


class DispatchHandler: public Thread
{
    public:
        class DispatchBuffer
        {
            public:
                DispatchHandle *pHandle;
                char *pMsg;
        };
        DispatchHandler();
        int Init();
        int Send(char *pMsg, DispatchHandle &handle);
    private:
        int Run();
        Buffer<DispatchBuffer *> dispatchBuffer;
};


class RecvHandler: public Thread
{
    public:
        RecvHandler(MsgBuffer *pMsgBuffer);
        void RegisterInstance(Socket *pSocket);
        void UnRegisterInstance(Socket *pSocket);
        int Init(); 
        int Recv(Socket *pSocket);
    private:
        int Run();
        int NotifyRecv();
        MsgBuffer *pMsgBuffer;
        ConnBuffer connBuffer;
};


class ListenerHandler:public Thread
{
    public:
       ListenerHandler(SockAddr &sockAddr);
       int Init();
       int Run();
       private:
           Socket sock;
           SockAddr &sockAddr;
};

#endif
