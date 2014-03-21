/***************************************************************************
* Copyright (C), 2013-2015, Beijing Teleron Telecom Engineering Co.,Ltd
* File name:     wtp.c
* Author:        renleilei - renleilei@ezlink.us
* Description:   
* Others: 
* Last modified: 2014-02-24 19:51
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cw_common.h"
#include "cw_protocol.h"
#include "cw_util.h"
#include "wtp.h"

int g_enable_log = CW_TRUE;
ac_info g_ac_info[CW_MAX_AC_NUM];
int g_wtp_socket = -1;

__inline__ int cw_get_fragment_id(void)
{
    static int frag_id = 0;
    return frag_id++;
}

__inline__ unsigned int cw_get_seq_num(void) 
{
    static unsigned int seq_num = 0;
	
    if (seq_num == CW_MAX_SEQ_NUM)  {
        seq_num = 0;
    }else{ 
        seq_num++;
    }
    
    return seq_num;
}

cw_state_transition cw_wtp_enter_discovery() 
{
    int state = CW_ENTER_DISCOVERY;
    cw_log("######### Discovery State #########");

    /* reset Discovery state */
    cw_close_socket(g_wtp_socket);
    if(!cw_init_socket_client(&g_wtp_socket, NULL)) {
    	return CW_QUIT;
    }

    state = cw_wtp_send_discovery();

    

    return state;
}

cw_state_transition cw_wtp_enter_sulking()
{
    cw_log("######### Sulking State #########");

    return CW_ENTER_IDLE;
}
    
cw_state_transition cw_wtp_enter_join()
{
    cw_log("######### Join State #########");
    
    return CW_ENTER_CONFIGURE;
}
    
cw_state_transition cw_wtp_enter_image_data()
{
    cw_log("######### Image Data State #########");

    return CW_ENTER_RESET;
}
    
cw_state_transition cw_wtp_enter_configure()
{
    cw_log("######### Configure State #########");

    return CW_ENTER_DATA_CHECK;
}
    
cw_state_transition cw_wtp_enter_datacheck()
{
    cw_log("######### Data Check State #########");

    return CW_ENTER_RUN;
}
    
cw_state_transition cw_wtp_enter_run()
{
    cw_log("######### Run State #########");

    return CW_ENTER_RUN;
}

cw_state_transition cw_wtp_enter_idle()
{
    cw_log("######### Idle State #########");

    return CW_ENTER_DISCOVERY;
}

void cw_wtp_destroy()
{
    cw_log("######### WTP Destroy #########");

    return ;
}

int main (int argc, const char **argv)
{

    cw_state_transition next_state = CW_ENTER_DISCOVERY;

    /* start CAPWAP state machine */	
    CW_REPEAT_FOREVER {
        switch(next_state) {
            case CW_ENTER_DISCOVERY:
                next_state = cw_wtp_enter_discovery();
                break;
            case CW_ENTER_IDLE:
                next_state = cw_wtp_enter_idle();
                break;
            case CW_ENTER_SULKING:
                next_state = cw_wtp_enter_sulking();
                break;
            case CW_ENTER_JOIN:
                next_state = cw_wtp_enter_join();
                break;
            case CW_ENTER_IMAGE_DATA:
                next_state = cw_wtp_enter_image_data();
                break;	
            case CW_ENTER_CONFIGURE:
                next_state = cw_wtp_enter_configure();
                break;	
            case CW_ENTER_DATA_CHECK:
                next_state = cw_wtp_enter_datacheck();
                break;	
            case CW_ENTER_RUN:
                next_state = cw_wtp_enter_run();
                break;
            case CW_ENTER_RESET:
                next_state = CW_ENTER_DISCOVERY;
                break;
            case CW_QUIT:
                cw_wtp_destroy();
                return 0;
        }
    }

    return 0;
}


