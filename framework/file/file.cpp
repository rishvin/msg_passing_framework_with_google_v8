#include "common.hpp"


int File::Open(int iFlag)
{
    return 0;
}

int File::Open(const char *pcFileName, int iFlag)
{
    int iErr;
    iErr = open(pcFileName, iFlag);
    if(-1 == iErr)
    {
        iErr = errno;
    }
    else
    {
        iFd = iErr;
        iErr = 0;
    }
    return iErr;
}

int File::Open(const char *pcFileName, int iFlag, mode_t tMode)
{
    int iErr;
    iErr = open(pcFileName, iFlag, tMode);
    if(-1 == iErr)
    {
        iErr = errno;
    }
    else
    {
        iFd = iErr;
        iErr = 0;
    }
    return iErr;
}


int File::LockBytes(long lLockType, long lPos,long lStart,long lLen)
{
    int iErr;
    struct flock sFl;
    sFl.l_type = lLockType;
    sFl.l_whence = lPos;
    sFl.l_start = lStart;
    sFl.l_len = lLen;
    iErr = fcntl(iFd, F_SETLK, &sFl);
    return ((-1 == iErr)? errno : 0);
}

int File::UnLockBytes(long lPos,long lStart,long lLen)
{
    int iErr;
    struct flock sFl;
    sFl.l_type = F_UNLCK;
    sFl.l_whence = lPos;
    sFl.l_start = lStart;
    sFl.l_len = lLen;
    iErr = fcntl(iFd, F_SETLK, &sFl);
    return ((-1 == iErr)? errno : 0);
}


int File::SetMode(int iFlag)
{
    int iErr;
    switch(iFlag)
    {
       case 0: // Enable blocking mode
       {
           int iOpts;
           iOpts = fcntl(iFd,F_GETFL);
           iOpts = (iOpts) & (~O_NONBLOCK);
           iErr = fcntl(iFd, F_SETFL, iOpts); 
           break;
       }
       case 1: // Enable non-blocking mode
       {
           iErr = fcntl(iFd, F_SETFL, O_NONBLOCK);
           break;
       }
    }
    return iErr;
}

