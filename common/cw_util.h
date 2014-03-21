/***************************************************************************
* Copyright (C), 2013-2015, Beijing Teleron Telecom Engineering Co.,Ltd
* File name:     cw_util.h
* Author:        renleilei - renleilei@ezlink.us
* Description:   
* Others: 
* Last modified: 2014-02-25 01:38
***************************************************************************/

void __cw_log(const char *filename, int line_num, const char *format, ...);
void* cw_malloc(size_t size);
void* cw_free(void* ptr);
int cw_random_int_in_range(int min, int max);




