#ifndef _THREAD_HPP_
#define _THREAD_HPP_

class Thread
{
    public:
    Thread(const char *cThreadName);
    void Create();
    int Join(void ** ppvStatus);
    const char *GetName() const;
    protected:
        virtual int Run() = 0;
    private:
    static void * CreateThread(void *); 
    pthread_t thread;
    const char *cThreadName;
};


#endif
