
#ifndef _UBLOCK_HEADER_
#define _UBLOCK_HEADER_

#include "UBHeader.h"
#include <pthread.h>


class UBLock
{
public:
	UBLock()
	{
		pthread_mutex_init(&m_lock, NULL);
	};

	// 	ҪȷUnlock״̬
	// make sure the lock is in unlock status;
	~UBLock()
	{
		if(pthread_mutex_destroy(&m_lock)<0)
		{
			pthread_mutex_unlock(&m_lock);
			pthread_mutex_destroy(&m_lock);
		};
	};

	inline void Lock()
	{
		pthread_mutex_lock(&m_lock);
	};

	inline bool TryLock()
	{
		return pthread_mutex_trylock(&m_lock)==0;
	};
	
	inline void Unlock()
	{
		pthread_mutex_unlock(&m_lock);
	};
protected:
	pthread_mutex_t m_lock;	
};





#endif


