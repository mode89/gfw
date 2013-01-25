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
    class AutoRefObject
    {
    public:
        AutoRefObject()
            : mCounter(0)
        {}

    protected:
        int mCounter;

        template < class ObjectClass > friend class AutoRef;
    };

    // Represents reference to an objects of class ObjectClass
    template < class ObjectClass >
    class AutoRef
    {
    public:
        inline ObjectClass * GetPointer() { return mObject; }

        inline bool IsAttached() { return (mObject == NULL) ? 0 : 1; }

        AutoRef(ObjectClass * object)
            : mObject(object)
        {
            if (mObject != NULL)
            {
                AtomicIncrement(mObject->mCounter);
            }
        }

        inline AutoRef(const AutoRef & ref)
            : mObject(ref.mObject)
        {
            AtomicIncrement(mObject->mCounter);
        }

        ~AutoRef()
        {
            if (mObject != NULL)
            {
                AtomicDecrement(mObject->mCounter);
                if (mObject->mCounter == 0)
                {
					mObject->Release();
                    mObject = NULL;
                }
            }
        }

        inline ObjectClass * operator-> () const { return mObject; }

        inline const AutoRef & operator= (const AutoRef & ref)
        {
            if (this != &ref)
            {
                mObject = ref.mObject;
                AtomicIncrement(mObject->mCounter);
            }

            return *this;
        }

        template < class CastClass >
        inline AutoRef<CastClass> StaticCast() const
        {
            return static_cast<CastClass*>(mObject);
        }

    private:
        ObjectClass * mObject;
    };

} // namespace Common

#endif // __AUTOREF_H__
