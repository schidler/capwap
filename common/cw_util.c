/***************************************************************************
* Copyright (C), 2013-2015, Beijing Teleron Telecom Engineering Co.,Ltd
* File name:     cw_util.c
* Author:        renleilei - renleilei@ezlink.us
* Description:   
* Others: 
* Last modified: 2014-02-25 01:37
***************************************************************************/

#include <stdlib.h>
#include "cw_common.h"

int g_malloc_counter;

__inline__ void cw_random_init_lib(void) 
{
        // set seed
    srand( (unsigned)time( NULL ) );
}
    
__inline__ int cw_random_int_in_range(int min, int max) 
{
    return min + (rand() % (max-min));
}

void* cw_malloc(size_t size)
{
    void * ret = NULL;

    ret = malloc(size);

    if (NULL == ret) {
        cw_log("Error: malloc failed.");
        return NULL;
    }

    g_malloc_counter++;
    
    //if(g_malloc_counter > 100)
        cw_log("Debug: total malloc counter=%d.", g_malloc_counter);

    return ret;
}


void* cw_free(void* ptr)
{
    void * ret = NULL;

    if(NULL != ptr) {
        free(ptr);
        ptr = NULL;
        g_malloc_counter--;
    }


    if(g_malloc_counter < 0)
        g_malloc_counter = 0;

    return ret;
}


