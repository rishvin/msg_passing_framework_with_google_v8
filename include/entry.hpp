#ifndef _ENTRY_HPP_
#define _ENTRY_HPP_

static Lock g_ProcLock;

#define ENTRY(PROCESS)                                      \
    typedef Singleton<PROCESS> ProcService;                 \
    template<> PROCESS* ProcService::instance = NULL;       \
    template<> Lock& ProcService::lock = g_ProcLock;        \
    int main(int argc, char **argv)                         \
    {                                                       \
        int iErr;                                           \
        PROCESS *pThis = ProcService::Create();             \
        iErr =  pThis->Main(argc, argv);                    \
        ProcService::Delete();                              \
        return iErr;                                        \
    }
#endif
