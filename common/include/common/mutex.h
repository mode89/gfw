#ifndef __COMMON_MUTEX_H__
#define __COMMON_MUTEX_H__

namespace Common {

    class Mutex
    {
    public:
        void
        Lock();

        void
        Unlock();

    public:
        Mutex();
        ~Mutex();

    private:
        void *  mImpl;
    };

} // namespace Common

#endif // __COMMON_MUTEX_H__
