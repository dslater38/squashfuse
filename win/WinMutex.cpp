/*
 * Copyright (c) 2019 Daniel R. Slater <dslater38@gmail.com>
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// Fix compile error in windows headers :(
struct IUnknown;
#include <windows.h>
#include <mutex>
#include <pthread.h>

extern "C"
{

	int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t * /* attr */ )
	{
		if (mutex)
		{
			mutex->data = reinterpret_cast<struct device_mutex *>(new std::mutex{});
		}
		return mutex->data != nullptr ? 0 : ENOMEM;
	}

	int pthread_mutex_destroy(pthread_mutex_t *mutex)
	{
		if (mutex && mutex->data)
		{
			std::mutex *ptr = reinterpret_cast<std::mutex *>(mutex->data);
			mutex->data = nullptr;
			delete ptr;
			return 0;
		}
		return EINVAL;
	}

	int pthread_mutexattr_destroy(pthread_mutexattr_t * attr)
	{
		if (attr)
		{
			return 0;
		}
		return EINVAL;
	}
	int pthread_mutexattr_init(pthread_mutexattr_t *attr)
	{
		if (attr)
		{
			return 0;
		}
		return EINVAL;
	}

	int pthread_mutex_lock(pthread_mutex_t *mutex)
	{
		if (mutex && mutex->data)
		{
			std::mutex *ptr = reinterpret_cast<std::mutex *>(mutex->data);
			ptr->lock();
			return 0;
		}
		return EINVAL;
	}

	int pthread_mutex_unlock(pthread_mutex_t *mutex)
	{
		if (mutex && mutex->data)
		{
			std::mutex *ptr = reinterpret_cast<std::mutex *>(mutex->data);
			ptr->unlock();
			return 0;
		}
		return EINVAL;
	}

	int __atomic_add_fetch(int *ptr, int val, int memmodel )
	{
		return (memmodel== __ATOMIC_RELAXED)
				? InterlockedAddNoFence(reinterpret_cast<long *>(ptr), val)
			    : InterlockedAdd(reinterpret_cast<long *>(ptr), val);
	}

	int __atomic_sub_fetch(int *ptr, int val, int memmodel )
	{
		return (memmodel == __ATOMIC_RELAXED)
			? InterlockedAddNoFence(reinterpret_cast<long *>(ptr), -val)
			: InterlockedAdd(reinterpret_cast<long *>(ptr), -val);
	}


}
