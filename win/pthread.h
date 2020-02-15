#ifndef PTHREAD_H_INCLUDED
#define PTHREAD_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


	int __atomic_add_fetch(int *ptr, int val, int memmodel);
	int __atomic_sub_fetch(int *ptr, int val, int memmodel);

/* Dummy definitions for these values. They are not used
   by the Windows polyfill implementation. Instead, the Windows InterlockedXXX methods
   always use acquire_release memory access. */
#define __ATOMIC_RELAXED 1
#define __ATOMIC_ACQ_REL 2

	typedef struct pthread_mutex
	{
		struct device_mutex *data;
	}pthread_mutex_t;

	typedef struct pthread_mutexattr
	{
		struct device_attr *data;
	}pthread_mutexattr_t;

	/* polyfils for Windows - these are implemented using a std::mutex 
	   The polyfill ignores the pthread_mutexattr_t parameter */
	int pthread_mutex_init(pthread_mutex_t *mutex,
		const pthread_mutexattr_t *attr);
	int pthread_mutex_destroy(pthread_mutex_t *mutex);

	int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
	int pthread_mutexattr_init(pthread_mutexattr_t *attr);

	int pthread_mutex_lock(pthread_mutex_t *mutex);

	int pthread_mutex_unlock(pthread_mutex_t *mutex);

#ifdef __cplusplus
}
#endif

#endif // PTHREAD_H_INCLUDED