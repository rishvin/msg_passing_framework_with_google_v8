#include "common.hpp"

SockAddr::SockAddr()
{
    sAddr.sin_family = 0;
    sAddr.sin_addr.s_addr = 0;
    sAddr.sin_port = 0;
}


SockAddr::SockAddr(unsigned int & uAddr, unsigned short & usPort, short & sFamily)
{
    sAddr.sin_family = sFamily;
    sAddr.sin_addr.s_addr = uAddr;
    sAddr.sin_port = usPort;
}

SockAddr &  SockAddr::operator () (const unsigned int & uAddr, const unsigned short & usPort, const short & sFamily)
{
    sAddr.sin_family      = sFamily;
    sAddr.sin_addr.s_addr = uAddr;
    sAddr.sin_port        = usPort;
    return (*this);
}

SockAddr & SockAddr::operator = (const SockAddr & param)
{
    sAddr.sin_family      = param.sAddr.sin_family;
    sAddr.sin_addr.s_addr = param.sAddr.sin_addr.s_addr;
    sAddr.sin_port        = param.sAddr.sin_port;
    return (*this);
}

Socket::Socket(int iDomain,int iType,int iProto)
    :iDomain(iDomain),
    iType(iType),
    iProto(iProto)
{}

// By deault it wil be non-blocking
int Socket::Open(int iFlag)
{
    int iErr;
    // What if we alredy have a socket
    if(0 < iFd)
    {
        close(iFd);
    }
    
    iFd = socket(iDomain, iType, iProto);
    iErr =  ((0 <= iFd)? 0: errno);
    if(0 == iErr)
    {
        iErr = SetMode(iFlag);
    }
    return iErr;
}

int Socket::Bind(SockAddr & sockAddr)
{
    int iErr = bind(iFd, (sockaddr *)&(sockAddr.sAddr), sizeof(sockAddr.sAddr));
    if(-1 != iErr)
    {
        bindAddr = sockAddr;
        iErr = 0;
    }
    else
    {
        iErr = errno;
    }
    return iErr;
}


void Socket::Listen(int iBackLog)
{
    listen(iFd, iBackLog);
}

int Socket::Accept(int & iRecvLen, Socket * (&pClient))
{
    pClient = NULL;
    SockAddr clientBind;
    socklen_t RecvLen = sizeof(clientBind.sAddr);
    int iRecvFid = accept(iFd, (sockaddr *) &(clientBind.sAddr), &RecvLen);
    if(0 <= iRecvFid)
    {
        // We know our sockets are of same type.
        pClient = new Socket(this->iDomain, this->iType, this->iProto);
        pClient->bindAddr = clientBind;
        pClient->iFd = iRecvFid;
        iRecvLen = RecvLen;
        return 0;
    }
    
    return errno;
}


int Socket::Connect(SockAddr & sockAddr)
{
    int iErr;
    iErr = connect(iFd, (const sockaddr *)&(sockAddr.sAddr), sizeof(sockAddr.sAddr));
    if(-1 == iErr)
    {
        
        iErr = errno;
    }
    else
    {
        iErr = 0;
    }
    return iErr;
}

int Socket::Send(const void *pvBuffer, size_t sBuffLen, int iFlags, size_t &iSendLen)
{
    int iLen;
	int iErr;
    iLen = send(iFd, pvBuffer, sBuffLen, iFlags);
    if(-1 == iLen)
    {
		iErr = errno;
    }
	else
	{
		iErr = 0;
	}
	iSendLen = iLen;
	return iErr;
}

int Socket::Recv(void *pvBuffer, size_t sBuffLen, int iFlags, size_t &iRecvLen)
{
    int iLen;
	int iErr;
    iLen = recv(iFd, pvBuffer, sBuffLen, iFlags);
	if(-1 == iLen)
    {
		iErr = errno;
    }
	else
	{
		iErr = 0;
	}
	iRecvLen = iLen;
	return iErr;
}


