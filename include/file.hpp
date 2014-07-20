#ifndef _FILE_HPP_
#define _FILE_HPP_

class File
{
    public:
        virtual ~File()
        {
            close(iFd);
            iFd = -1;
        }
        virtual int Open(int iFlag);
        virtual int Open(const char *pcFileName, int iFlag);
        virtual int Open(const char *pcFileName, int iFlag, mode_t tMode);

        /* Advisory Locks */
        int LockBytes(long lLockType, long lPos, long lStart, long lLen);
        int UnLockBytes(long lPos, long lStart, long lLen);
        
        template <class T>
        size_t Read(T tBuffer, size_t sSize, size_t & sRead)
        {
            int iErr;
            iErr = read(iFd, tBuffer, sSize);
            if(-1 == iErr)
            {
                iErr = errno;
            }
            else
            {
                sRead = iErr;
                iErr = 0;
            }
            return iErr;
        }
        
        template <class T>
        size_t Write(T tBuffer, size_t sSize, size_t &sWrite)
        {
            int iErr;
            iErr = write(iFd, tBuffer, sSize);
            if(-1 == iErr)
            {
                iErr = errno;
            }
            else
            {
                sWrite = iErr;
                iErr = 0;
            }
            return iErr;
        }

        int SetMode(int iFlag);
    protected:
        int iFd;
};


#endif
