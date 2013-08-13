#ifndef __AUTOREF_H__
#define __AUTOREF_H__

#include "common\typedefs.h"
#include "common\atomic.h"

#define AUTOREF_REFERENCE_DECLARATION(typeName) \
    typedef       Common::AutoRef <       typeName >            typeName ## Ref; \
	typedef       Common::AutoRef < const typeName >   Const ## typeName ## Ref; \
	typedef const Common::AutoRef <       typeName > &          typeName ## In; \
	typedef const Common::AutoRef < const typeName > & Const ## typeName ## In  \

#define AUTOREF_FORWARD_DECLARATION(typeName) \
	class typeName; \
	AUTOREF_REFERENCE_DECLARATION(typeName) \

namespace Common {

    // ObjectClass has to inherit this class
    class ARefCounted
    {
    public:
        ARefCounted()
            : mRefCounter(0)
        {}

        inline
        int GetRefCounter() const { return mRefCounter; }

    protected:
        int mRefCounter;

        template < class ObjectClass > friend class AutoRef;
    };

    // Represents reference to an objects of class ObjectClass
    template < class ObjectClass >
    class AutoRef
    {
    public:

        inline ObjectClass * GetPointer() const { return mObject; }

        inline bool IsAttached() const { return (mObject == NULL) ? 0 : 1; }

        inline bool IsNull() const { return (mObject == NULL) ? 1 : 0; }

        inline void Detach()
        {
            ReleaseObject();
            mObject = NULL;
        }

        AutoRef()
            : mObject(NULL)
        {

        }

        AutoRef(ObjectClass * object)
            : mObject(object)
        {
            AddRefObject();
        }

        template < class SourceClass >
        inline AutoRef(const AutoRef< SourceClass > & ref)
            : mObject(static_cast< ObjectClass* >(ref.GetPointer()))
        {
            AddRefObject();
        }

        inline AutoRef(const AutoRef & ref)
            : mObject(ref.mObject)
        {
            AddRefObject();
        }

        ~AutoRef()
        {
            ReleaseObject();
            mObject = NULL;
        }

        inline ObjectClass & operator * () const { return *mObject; }

        inline ObjectClass * operator-> () const { return mObject; }

        inline const AutoRef & operator= (const AutoRef & ref)
        {
            if (this != &ref)
            {
                ReleaseObject();
                mObject = ref.mObject;
                AddRefObject();
            }

            return *this;
        }

        template < class CastClass >
        inline AutoRef<CastClass> StaticCast() const
        {
            return static_cast<CastClass*>(mObject);
        }

    private:

        inline void ReleaseObject()
        {
            if (mObject != NULL)
            {
                AtomicDecrement(mObject->mRefCounter);
                if (mObject->mRefCounter == 0)
                {
                    delete mObject;
                }
            }
        }

        inline void AddRefObject()
        {
            if (mObject != NULL)
            {
                AtomicIncrement(mObject->mRefCounter);
            }
        }

    protected:
        ObjectClass * mObject;
    };

    // Encapsulates a pointer with ref counting
    template <typename T>
    class Pointer : public ARefCounted
    {
    public:
        Pointer()
            : mData(NULL)
        {}

        Pointer(T * data)
            : mData(data)
        {}

        ~Pointer()
        {
            delete mData;
        }

    private:
        T * mData;

        template < typename Type >
        friend class AutoPointer;
    };

    // Wrapper for AutoRef<Pointer>
#define AUTOREF AutoRef< Pointer< T > >
    template < typename T >
    class AutoPointer : public AUTOREF
    {
    public:
        inline
        AutoPointer(T * object)
            : AUTOREF(new Pointer<T>(object))
        {}

        inline T &
        operator [] (uint32_t index) { return AUTOREF::mObject->mData[index]; }

        inline T &
        operator * () { return *AUTOREF::mObject->mData; }

        inline T *
        operator -> () { return AUTOREF::mObject->mData; }

        inline
        operator T* () { return AUTOREF::mObject->mData; }
    };
#undef AUTOREF

} // namespace Common

#endif // __AUTOREF_H__
