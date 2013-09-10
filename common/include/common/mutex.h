#ifndef __COMMON_MUTEX_H__
#define __COMMON_MUTEX_H__

namespace Common {

    struct MutexImpl;

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

    public:
        MutexImpl * mImpl;
    };

} // namespace Common

#endif // __COMMON_MUTEX_H__
