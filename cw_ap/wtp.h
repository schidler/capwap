/***************************************************************************
* Copyright (C), 2013-2015, Beijing Teleron Telecom Engineering Co.,Ltd
* File name:     wtp.h
* Author:        renleilei - renleilei@ezlink.us
* Description:   
* Others: 
* Last modified: 2014-02-24 19:57
***************************************************************************/

#ifndef __WTP_H__
#define __WTP_H__
#include <arpa/inet.h>
#include "cw_common.h"

#define CW_MAX_AC_NUM                                   8


typedef struct  __ac_info {
    char name[32];
    struct sockaddr_in addr;
    char *domain_name;
}ac_info;

unsigned int cw_get_seq_num(void);
int cw_get_fragment_id(void);


#endif

