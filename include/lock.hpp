#ifndef _LOCK_HPP_
#define _LOCK_HPP_

class Lock
{
    protected:
        friend class MutexGuard;
        friend class CondMutex;
        pthread_mutex_t mutex;
};

class MutexGuard
{
    public:
        MutexGuard(Lock& lock)
        {
            pthread_mutex_lock(&lock.mutex);
            _lock = &lock.mutex;
        }
        ~MutexGuard()
        {
            pthread_mutex_unlock(_lock);
        }

    private:
        pthread_mutex_t *_lock;
};

class CondMutex
{
    public:
        void Wait(Lock &lock)
        {
            pthread_cond_wait(&_cond_lock, &lock.mutex);
        }

        void Signal()
        {
            pthread_cond_signal(&_cond_lock);
        }
        void Broadcast()
        {
            pthread_cond_broadcast(&_cond_lock);
        }
    private:
        pthread_cond_t _cond_lock;
};

#endif

