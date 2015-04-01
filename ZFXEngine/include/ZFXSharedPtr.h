#ifndef _ZFX_SHARED_PTR_H_
#define _ZFX_SHARED_PTR_H_

#include "ZFXPrerequisites.h"
#include <algorithm>
#include <functional>

namespace ZFX
{
	template<class T>
	class SharedPtr
	{
	public:
		SharedPtr() : mPtr(NULL), mUseCount(NULL)
		{
		}

		SharedPtr(T *ptr) :
			mPtr(ptr),
			mUseCount(ptr == NULL ? 0 : new uint32(1))
		{
		}

		SharedPtr(const SharedPtr& ptr) :
			mPtr(NULL), mUseCount(NULL)
		{
			mPtr = ptr.mPtr;
			mUseCount = ptr.mUseCount;
			if (mUseCount)
			{
				++(*mUseCount);
			}
		}

		~SharedPtr()
		{
			Release();
		}

		SharedPtr& operator=(SharedPtr const& ptr)
		{
			if (mPtr == ptr.mPtr)
				return *this;
			SharedPtr<T> temp(ptr);
			Swap(temp);
			return *this;
		}

		inline T& operator*() const
		{
			assert(mPtr);
			return *mPtr;
		}

		inline T* operator->() const
		{
			assert(mPtr);
			return mPtr;
		}

		inline T* Get() const
		{
			return mPtr;
		}

		void Assign(T* ptr)
		{
			assert(!mPtr && !mUseCount);

			mUseCount = new uint32(1);
			mPtr = ptr;
		}

		inline void Clear()
		{
			if (mPtr)
			{
				Release();
				mPtr = NULL;
				mUseCount = NULL;
			}
		}

		inline bool Unique() const
		{
			assert(mUseCount);
			return *mUseCount == 1;
		}

		inline bool IsNULL() const
		{
			return mPtr == NULL;
		}

		void Swap(SharedPtr<T> & ptr)
		{
			std::swap(mPtr, ptr.mPtr);
			std::swap(mUseCount, ptr.mUseCount);
		}

		uint32 UseCount() const
		{
			assert(mUseCount);
			return *mUseCount;
		}

	protected:
		T* mPtr;
		uint32* mUseCount;

		inline void Release()
		{
			bool destroy = false;
			if (mUseCount)
			{
				if (--(*mUseCount) == 0)
				{
					destroy = true;
				}
			}
			if (destroy)
			{
				assert(mPtr);
				delete mPtr;
				delete mUseCount;
			}
		}

	};

	template<class A, class B> inline bool operator==(SharedPtr<A> const& a, SharedPtr<B> const& b)
	{
		return a.Get() == b.Get();
	}

	template<class A, class B> inline bool operator!=(SharedPtr<A> const& a, SharedPtr<B> const& b)
	{
		return a.Get() != b.Get();
	}

	template<class T> inline bool operator<(SharedPtr<T> const& a, SharedPtr<T> const& b)
	{
		return std::less<T*>()(a.Get(), b.Get());
	}
}

#endif