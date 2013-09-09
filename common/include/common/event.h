#ifndef __COMMON_EVENT_H__
#define __COMMON_EVENT_H__

namespace Common {

    struct EventImpl;

    class Event
    {
    public:
        void
        Wait();

        void
        Notify();

    public:
        Event();
        ~Event();

    private:
        EventImpl * mImpl;
    };

} // namespace Common

#endif // __COMMON_EVENT_H__
