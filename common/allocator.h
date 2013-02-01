#ifndef __COMMON_ALLOCATOR_H__
#define __COMMON_ALLOCATOR_H__

namespace Common {

    class IAllocator
    {
    public:
        virtual void *  Alloc(uint32_t size) = 0;

        virtual void    Free(void *) = 0;

        virtual void    FreeAsync(void *) = 0;

        virtual void    GarbageCollect() = 0;

        virtual         ~IAllocator() { }
    };

    IAllocator * GetDefaultAllocator();

} // namespace Common

inline void * operator new(uint32_t size, Common::IAllocator * a)
{
    return a->Alloc(size);
}

inline void operator delete(void * data, Common::IAllocator * a)
{
    a->Free(data);
}

#endif // __COMMON_ALLOCATOR_H__
