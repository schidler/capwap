/***************************************************************************
* Copyright (C), 2013-2015, Beijing Teleron Telecom Engineering Co.,Ltd
* File name:     cw_log.c
* Author:        renleilei - renleilei@ezlink.us
* Description:   
* Others: 
* Last modified: 2014-02-24 22:58
***************************************************************************/

#include <stdarg.h>
#include "cw_common.h"

extern int g_enable_log;
#define LOG_BUF_SIZE  512

__inline__ void __cw_log(const char *filename, int line_num, const char *format, ...)
{
    va_list args;
    char log_buf[LOG_BUF_SIZE] = {0};
	
    if(g_enable_log) {
        va_start(args, format);
        vsnprintf(log_buf, LOG_BUF_SIZE, format, args);
        va_end(args);
    
        printf("%s:%d | %s\n", filename, line_num, log_buf);
    }
}

cw_bool __cw_error(const char *filename, int line_num, const char *format, ...)
{
    va_list args;
    char log_buf[LOG_BUF_SIZE] = {0};
	
    if(g_enable_log) {
        va_start(args, format);
        vsnprintf(log_buf, LOG_BUF_SIZE, format, args);
        va_end(args);
    
        printf("%s:%d | %s\n", filename, line_num, log_buf);
    }

    return CW_FALSE;
}


