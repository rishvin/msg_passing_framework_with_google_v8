#include "common.hpp"

Thread::Thread(const char *cThreadName)
    :cThreadName(cThreadName)
{
}


void Thread::Create()
{
    pthread_create(&thread, NULL, (void * (*)(void *))&Thread::CreateThread, this);
}

int Thread::Join(void ** ppvStatus)
{
    pthread_join(thread, ppvStatus);
}

const char * Thread::GetName() const
{
    return cThreadName;
}


void * Thread::CreateThread(void *pParam)
{
    Thread *pThread = (Thread *)(pParam);
    pThread->Run();
}
