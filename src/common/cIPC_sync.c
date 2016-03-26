#include "klib/cIPC_sync.h"
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>
#include <errno.h>

Sync_Sem_t* Sync_Sem_Init(unsigned int value)
{
	int ret;
	Sync_Sem_t* psem = malloc(sizeof(Sync_Sem_t));
	
	if(psem == NULL)
		return NULL;
	
	ret = sem_init(psem, 0, value);
	if(ret < 0)
	{
		free(psem);
		psem = NULL;
		return NULL;
	}
	
	return psem;
}
/*******************************************************************************
inline void Sync_Sem_Init(Sync_Sem_t* &pSem, unsigned int value)
{
	pSem = Sync_Sem_Init(value);
}
*******************************************************************************/
int Sync_Sem_Destroy(Sync_Sem_t* psem)
{
	int ret;
	
	ret =	sem_destroy(psem);
	free(psem);
	
	return ret;
}

int Sync_Sem_Wait(Sync_Sem_t* psem)
{
	return sem_wait(psem);
}

int Sync_Sem_TryWait(Sync_Sem_t* psem)
{
	return sem_trywait(psem);
}

static void maketimeout(struct timespec* tsp, unsigned long usecs)
{
	struct timeval tv;
	
	gettimeofday(&tv , NULL);
	                                         
	tsp->tv_sec = tv.tv_sec + usecs/1000000;
	tsp->tv_nsec = (tv.tv_usec + (usecs%1000000))* 1000;
}

int	Sync_Sem_WaitTimeout(Sync_Sem_t* psem, unsigned long usecs)
{
	int ret;
	struct timespec ts;
	
	maketimeout(&ts, usecs);
	while(((ret = sem_timedwait(psem, &ts)) == -1) && errno == EINTR)
		continue;

	return ret;
}

int Sync_Sem_Signal(Sync_Sem_t* psem)
{
	return sem_post(psem);
}

Sync_Mutex_t* Sync_Mutex_Init(void)
{
	int	ret;
	Sync_Mutex_t* pmutex = malloc(sizeof(Sync_Mutex_t));
	
	if(pmutex == NULL)
		return NULL;
	
	ret = pthread_mutex_init(pmutex, NULL);
	if(ret != 0)
	{
		free(pmutex);
		return NULL;
	}
	
	return pmutex;
}

int Sync_Mutex_Destroy(Sync_Mutex_t* pmutex)
{
	int ret;
	
	ret = pthread_mutex_destroy(pmutex);
	free(pmutex);
	return ret;
}

int Sync_Mutex_Lock(Sync_Mutex_t* pmutex)
{
	return pthread_mutex_lock(pmutex);
}

int Sync_Mutex_UnLock(Sync_Mutex_t* pmutex)
{
	return pthread_mutex_unlock(pmutex);
}

int Sync_Mutex_TryLock(Sync_Mutex_t* pmutex)
{
	return pthread_mutex_trylock(pmutex);
}
/*------------------------------------EOF------------------------------------*/