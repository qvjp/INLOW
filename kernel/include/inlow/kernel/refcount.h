#ifndef KERNEL_REFCOUNT_H
#define KERNEL_REFCOUNT_H

#include <inlow/kernel/kernel.h>

class ReferenceCounted
{
	public:
			ReferenceCounted();
			virtual ~ReferenceCounted();
			void addReference() const;
			void removeReference() const;
	private:
			mutable size_t refcount;
};

template <typename T>
class Reference
{
public:
	Reference() : object(nullptr)
	{
	}
	Reference(T* obj) : object(obj)
	{
		if (object)
		{
			object->addReference();
		}
	}

	Reference(const Reference& ref) : Reference((T*) ref)
	{
	}

	template <typename T2, typename = decltype(Reference<T>((T2*) nullptr))>
	Reference(const Reference<T2>& ref) : Reference((T2*) ref)
	{
	}

	template <typename T2>
	explicit operator Reference<T2>() const
	{
		return Reference<T2>((T2*) object);
	}

	~Reference()
	{
		if (object)
		{
			object->removeReference();
		}
	}

	Reference& operator=(const Reference& ref)
	{
		return operator=<T>(ref);
	}

	template <typename T2>
	Reference& operator=(const Reference<T2>& ref)
	{
		if (object == (T2*) ref)
			return *this;

		if (object)
		{
			object->removeReference();
		}

		object = (T2*) ref;
		if (object)
		{
			object->addReference();
		}

		return *this;
	}

	template <typename T2>
	bool operator==(const Reference<T2>& ref)
	{
		return object == (T2*) ref;
	}
	operator bool() const
	{
		return object;
	}

	explicit operator T*() const
	{
		return object;
	}

	T& operator*() const
	{
		return *object;
	}

	T* operator->() const
	{
		return object;
	}

private:
	T* object;

};


#endif
