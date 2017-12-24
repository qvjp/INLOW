#ifndef KERNEL_DYNARRAY_H
#define KERNEL_DYNARRAY_H

#include <assert.h>
#include <stdlib.h>
#include <inlow/kernel/kernel.h>

template <typename T, typename TSize = size_t>
class DynamicArray
{
	public:
		DynamicArray()
		{
			allocatedSize = 0;
			buffer = nullptr;
		}

		~DynamicArray()
		{
			for (TSize i = 0; i < allocatedSize; i++)
			{
				buffer[i].~T();
			}
			free(buffer);
		}

		TSize add(const T& obj)
		{
			TSize i;
			for (i = 0; i < allocatedSize; i++)
			{
				if (!buffer[i])
				{
					buffer[i] = obj;
					return i;
				}
			}
			TSize newSize;
			if (__builtin_add_overflow(allocatedSize, 1, &newSize))
			{
				return (TSize) -1;
			}
			if (!resize(newSize))
			{
				return (TSize) -1;
			}
			buffer[i] = obj;
			return i;
		}
		TSize next(TSize index)
		{
			for (TSize i = index + 1; i < allocatedSize; i++)
			{
				if (buffer[i])
				{
					return i;
				}
			}
			return (TSize) -1;
		}
		void remove(TSize index)
		{
			buffer[index] = T();
		}
		bool resize(TSize size)
		{
			assert(size > allocatedSize);
			T* newBuffer = (T*) reallocarray(buffer, (size_t)size, sizeof(T));
			if (!newBuffer)
			{
				return false;
			}
			buffer = newBuffer;
			for (TSize i = allocatedSize; i < size; i++)
			{
				new (&buffer[i]) T();
			}
			allocatedSize = size;
			return true;
		}

		T& operator[](TSize index)
		{
			assert(index >= 0 && index < allocatedSize);
			return buffer[index];
		}
	private:
		TSize allocatedSize;
		T* buffer;
};

#endif
