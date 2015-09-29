#include <pthread.h>

class Mutex {
    pthread_mutex_t mLock;
    Mutex(const Mutex&);
    Mutex& operator=(const Mutex&);
public:
    Mutex();
    ~Mutex();
    int tryLock();
    int lock();
    int unlock();
    class Autolock {
    public:
        Autolock(Mutex& m);
        ~Autolock();
    private:
        Mutex& mLock;
    };
};

inline Mutex::Mutex()
{
    pthread_mutex_init(&mLock, NULL);
}
inline Mutex::~Mutex()
{
    pthread_mutex_destroy(&mLock);
}
inline int Mutex::tryLock()
{
    return pthread_mutex_trylock(&mLock);
}
inline int Mutex::lock()
{
    return pthread_mutex_lock(&mLock);
}
inline int Mutex::unlock()
{
    return pthread_mutex_unlock(&mLock);
}
inline Mutex::Autolock::Autolock(Mutex& m)
:mLock(m)
{
    mLock.lock();
}
inline Mutex::Autolock::~Autolock()
{
    mLock.unlock();
}
typedef Mutex::Autolock AutoMutex;
