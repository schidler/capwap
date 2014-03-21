/***************************************************************************
* Copyright (C), 2013-2015, Beijing Teleron Telecom Engineering Co.,Ltd
* File name:     cw_thread.c
* Author:        renleilei - renleilei@ezlink.us
* Description:   
* Others: 
* Last modified: 2014-02-27 22:06
***************************************************************************/

#include <pthread.h>
#include "cw_common.h"

// locks a mutex among threads at the specified address (blocking)
cw_bool cw_thread_mutex_lock(pthread_mutex_t *mutex) 
{
    if(mutex == NULL) return cw_error("Error: arg error.");
    if(pthread_mutex_lock( mutex ) != 0) {
        return cw_error("Can't lock thread mutex");
    }
/*
    fprintf(stdout, "Mutex %p locked by %p.\n", theMutex, pthread_self());
    fflush(stdout);
*/
    return CW_TRUE;
}

// unlocks a mutex among threads at the specified address
void cw_thread_mutex_unlock(pthread_mutex_t *mutex) 
{
	if(mutex == NULL) return;
	pthread_mutex_unlock( mutex );
/*
	fprintf(stdout, "Mutex %p UNlocked by %p.\n", theMutex, pthread_self());
	fflush(stdout);
*/
}


