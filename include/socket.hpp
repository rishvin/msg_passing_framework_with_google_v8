#ifndef _SOCKET_HPP_
#define _SOCKET_HPP_


class SockAddr
        {
            public:
                SockAddr(unsigned int & uAddr, unsigned short & usPort, short & sFamily);
                SockAddr();
                SockAddr & operator () (const unsigned int & uAddr, const unsigned short & usPort, const short & sFamily);
                SockAddr &  operator = (const SockAddr & param);
            private:
            friend class Socket;
            struct sockaddr_in sAddr;
        };


class Socket:public File
{
    public:
        Socket(int iDomain, int iType, int iProto);
        int Open(int iFlag = 1);
        int Bind(SockAddr & sockAddr);
        void Listen(int iBackLog);
        int Accept(int & iRecvLen, Socket * (&pClient));
        int Connect(SockAddr & sockaddr);
        int Send(const void *pvBuffer, size_t sBuffLen, int iFlags, size_t &iSendLen);
        int Recv(void *pvBuffer, size_t sBuffLen, int iFlags, size_t &iRecvLen);

    private:
        SockAddr bindAddr;
        int iDomain;
        int iType;
        int iProto;

};


#if 0
class SockUtil
{
    public:
        static int Select(FdSet &readFd, FdSet &writeFd, FdSet &excepFd, timeval & sTimeVal);
};
#endif


#endif
