/***************************************************************************
* Copyright (C), 2013-2015, Beijing Teleron Telecom Engineering Co.,Ltd
* File name:     ac.c
* Author:        renleilei - renleilei@ezlink.us
* Description:   
* Others: 
* Last modified: 2014-02-24 19:56
***************************************************************************/

#include "cw_common.h"

__inline__ int cw_get_fragment_id(void) 
{
    static int frag_id = 0;
    int r;

    if(!cw_thread_mutex_lock(&g_create_id_mutex)) {
        cw_log("Error Locking a mutex");
    }

    r = frag_id;

    if (frag_id==CW_MAX_FRAGMENT_ID) 
        frag_id=0;
    else 
        frag_id++;

    cw_thread_mutex_lock(&g_create_id_mutex);
    return r;
}

int main (int argc, const char * argv[]) 
{
    /* Daemon mode */

    if (daemon(1, 0) < 0)
        exit(1);

    if (chdir("/tmp") != 0)
        exit(1);

    CWACInit();
    CWCreateConnectionWithHostapdAC();
    CWACEnterMainLoop();
    CWACDestroy();  

    return 0;
}

