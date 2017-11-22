#ifndef KERNEL_KTHREAD_H
#define KERNEL_KTHREAD_H

typedef bool kthread_mutex_t;
#define KTHREAD_MUTEX_INITIALIZER false

int kthread_mutex_lock(kthread_mutex_t* mutex);
int kthread_mutex_unlock(kthread_mutex_t* mutex);

class AutoLock
{
	public:
			AutoLock(kthread_mutex_t* mutex)
			{
				this->mutex = mutex;
				kthread_mutex_lock(mutex);
			}

			~AutoLock()
			{
				kthread_mutex_unlock(mutex);
			}

	private:
			kthread_mutex_t* mutex;
};

#endif
