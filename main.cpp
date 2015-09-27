#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <pthread.h>
#include <stdexcept>

using namespace std;

class ThreadModel
{
public:
    ThreadModel();
    virtual ~ThreadModel();
    virtual int ThreadLoop() = 0;
    int run();
    int join();
    pthread_t getId()
    {
        return mThreadId;
    }
private:
    static void *_threadLoop(void *private_data);
    pthread_t mThreadId;
    ThreadModel *mHoldSelf;
};
ThreadModel::ThreadModel() : mThreadId(0)
{
}
ThreadModel::~ThreadModel()
{
    void* ret;
    if (mThreadId)
        pthread_join(mThreadId, &ret);
}
int ThreadModel::run()
{
    mHoldSelf = this;
    cout << "mHoldSelf = " << this << endl;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int err = pthread_create(&mThreadId, NULL, _threadLoop, this);
    pthread_attr_destroy(&attr);
    return err;
}
int ThreadModel::join()
{
    int ret = pthread_join(mThreadId, NULL);
    return ret;
}
void *ThreadModel::_threadLoop(void *private_data)
{
    int ret;
    ThreadModel* const _this = (ThreadModel *)private_data;
    while (true)
    {
        ret = _this->ThreadLoop();
        if (ret)
            break;
    }
}
class ThreadModelTest : public ThreadModel
{
public:
    ThreadModelTest() : dummy(5) {}
    virtual ~ThreadModelTest() {}
    virtual int ThreadLoop();
private:
    int dummy;
};
int ThreadModelTest::ThreadLoop()
{
    //dummy = 50;
    cout << "dummy = " << dummy << endl;
    return 1;
}
int main()
{
    try
    {
        ThreadModelTest *pt = new ThreadModelTest();
        pt->run();
        pt->join();
        delete pt;
    }
    catch (exception ex)
    {
        cout << "create ThreadModelTest fail" << endl;
        return -1;
    }
    return 0;
}
