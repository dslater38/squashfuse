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
#ifdef _MSC_VER

#include <Windows.h>
#include "../mutex.h"

int init_mutex(Mutex *pMtx)
{
	int success = -1;
	if (pMtx != NULL)
	{
		pMtx->impl = (CRITICAL_SECTION *)malloc(sizeof(CRITICAL_SECTION));
		if (pMtx->impl != NULL)
		{
			InitializeCriticalSectionAndSpinCount((CRITICAL_SECTION *)(pMtx->impl), 1000);
			success = LOCK_SUCCESS;
		}
	}
	return success;
}

int lock(Mutex *pMtx)
{
	if (pMtx)
	{
		EnterCriticalSection((CRITICAL_SECTION *)(pMtx->impl));
		return LOCK_SUCCESS;
	}
	return LOCK_FAILURE;
}

int unlock(Mutex *pMtx)
{
	if (pMtx)
	{
		LeaveCriticalSection((CRITICAL_SECTION *)(pMtx->impl));
		return LOCK_SUCCESS;
	}
	return LOCK_FAILURE;
}

int delete_mutex(Mutex *pMtx)
{
	if (pMtx && pMtx->impl)
	{
		DeleteCriticalSection((CRITICAL_SECTION *)(pMtx->impl));
		pMtx->impl = NULL;
		return LOCK_SUCCESS;
	}
	return LOCK_FAILURE;
}

#endif
