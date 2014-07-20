#ifndef _SINGLETON_HPP_
#define _SINGLETON_HPP_

template<class T>
class Singleton
{
   public:
       static T*  Create()
       {
           MutexGuard(static_cast<Lock &>(Singleton<T>::lock));
           if(NULL == Singleton<T>::instance)
           {
               Singleton<T>::instance = new T(); // Assuming we have no param to pass
           }
		   return instance;
       }

       template<class P1>
       static T*  Create(P1 param)
       {
           MutexGuard(static_cast<Lock &>(Singleton<T>::lock));
           if(NULL == Singleton<T>::instance)
           {
               Singleton<T>::instance = new T(param); // Assuming we have 1 param to pass
           }
		   return instance;
       }

       static T * Get() // let user check if it is NULL or not
       {
           MutexGuard(static_cast<Lock &>(Singleton<T>::lock));
           return Singleton<T>::instance;
       }
       static void Delete()
       {
           MutexGuard(static_cast<Lock &>(Singleton<T>::lock));
           if(NULL != Singleton<T>::instance) // What if some created singleton object before calling create.
           {
               delete Singleton<T>::instance;
			   instance = NULL;
           }
       }
    private:
        static Lock &lock;
        static T *instance;
};

#endif
