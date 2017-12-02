#include <assert.h>
#include <inlow/kernel/refcount.h>

ReferenceCounted::ReferenceCounted()
{
	refcount = 0;
}

ReferenceCounted::~ReferenceCounted()
{
	assert(refcount == 0);
}

void ReferenceCounted::addReference() const
{
	__atomic_add_fetch(&refcount, 1, __ATOMIC_SEQ_CST);
}

void ReferenceCounted::removeReference() const
{
	__atomic_sub_fetch(&refcount, 1, __ATOMIC_SEQ_CST);
	if (!refcount)
	{
		delete this;
	}
}
