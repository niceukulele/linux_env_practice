#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <pthread.h>
#include <stdexcept>

using namespace std;

class ThreadModel {
public:
    ThreadModel();
    virtual ~ThreadModel();
    virtual int ThreadLoop() = 0;
    void run() {}
    pthread_t getId() { return mThreadId; }
private:
    static void *_threadLoop(void *private_data);
    pthread_t mThreadId;
};
ThreadModel::ThreadModel() : mThreadId(0)
{
    int err = pthread_create(&mThreadId, NULL, ThreadModel::_threadLoop, this);
    if (err)
        throw new runtime_error("create thread fail");
}
ThreadModel::~ThreadModel()
{
    void* ret;
    if (mThreadId)
        pthread_join(mThreadId, &ret);
}
void *ThreadModel::_threadLoop(void *private_data)
{
    int ret;
    ThreadModel *_this = (ThreadModel *)private_data;
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
    ThreadModelTest() : ThreadModel(), dummy(5) {}
    virtual ~ThreadModelTest() {}
    virtual int ThreadLoop()
    {
        dummy = 50;
        cout << "dummy = " << dummy << endl;
        return 1;
    }
private:
    int dummy;
};
int main()
{
    try
    {
        ThreadModelTest tt;
    } catch (exception ex)
    {
        cout << "create ThreadModelTest fail" << endl;
        return -1;
    }
    return 0;
}
