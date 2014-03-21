/***************************************************************************
* Copyright (C), 2013-2015, Beijing Teleron Telecom Engineering Co.,Ltd
* File name:     cw_public.h
* Author:        renleilei - renleilei@ezlink.us
* Description:   
* Others: 
* Last modified: 2014-02-24 19:58
***************************************************************************/

#ifndef __CW_COMMON_H__
#define __CW_COMMON_H__

#include <linux/if_ether.h>
#include <errno.h>
#include <sys/time.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>     
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>
#include <stdarg.h>

#include "cw_util.h"
#include "cw_protocol.h"

#define		CW_BUFFER_SIZE					65536
#define		CW_ZERO_MEMORY					bzero
#define		CW_COPY_MEMORY(dst, src, len)			bcopy(src, dst, len)
#define		CW_REPEAT_FOREVER				while(1)
	
#define DEFAULT_LOG_SIZE					1000000
	
typedef enum {
    CW_FALSE = 0,
    CW_TRUE = 1
} cw_bool;

typedef enum {
    CW_ENTER_SULKING,
    CW_ENTER_IDLE,
    CW_ENTER_DISCOVERY,
    CW_ENTER_JOIN,
    CW_ENTER_IMAGE_DATA,
    CW_ENTER_CONFIGURE,
    CW_ENTER_DATA_CHECK,
    CW_ENTER_RUN,
    CW_ENTER_RESET,
    CW_QUIT
} cw_state_transition;

extern int g_wtp_socket ;
extern int g_enable_log;

#define cw_log(format...)  __cw_log(__FILE__, __LINE__, format)
#define cw_error(format...)  __cw_error(__FILE__, __LINE__, format)

#define		cw_close_socket(x)		{ shutdown(SHUT_RDWR, x); close(x); }

#define	CW_CREATE_ARRAY(ar_name, ar_size, ar_type, on_err)	{ar_name = (ar_type*) (cw_malloc(sizeof(ar_type) * (ar_size))); if(!(ar_name)) {on_err}}

#define 	CW_CREATE_PROTOCOL_MSG_ARRAY(ar_name, ar_size, on_err) 	{\
											CW_CREATE_ARRAY(ar_name, ar_size, cw_protocol_msg, on_err)\
											int i;\
											for(i=0;i<(ar_size); i++) {\
												(ar_name)[i].msg = NULL;\
												(ar_name)[i].offset = 0; \
											}\
										}


#define	CW_CREATE_OBJECT_SIZE(obj_name, obj_size,on_err)	{obj_name = (cw_malloc(obj_size)); if(!(obj_name)) {on_err}}
#define	CW_CREATE_PROTOCOL_MESSAGE(mess, size, err)		CW_CREATE_OBJECT_SIZE(((mess).msg), (size), err);		\
									CW_ZERO_MEMORY(((mess).msg), (size));						\
									(mess).offset = 0;

#define		CW_FREE_PROTOCOL_MESSAGE(mess)				cw_free(((mess).msg));	\
									(mess).offset = 0;
#define	CW_CREATE_OBJECT(obj_name, obj_type, on_err)	{obj_name = (obj_type*) (cw_malloc(sizeof(obj_type))); if(!(obj_name)) {on_err}}

cw_bool cw_init_socket_client(int *sock, const char *addr) ;
cw_state_transition cw_wtp_send_discovery(void);
cw_bool cw_send_unconnected(int sock, struct sockaddr_in  *addr, const char *buf, int len) ;
cw_bool cw_build_msg_elem(cw_protocol_msg *msg, unsigned int type);
cw_bool  cw_build_msg(cw_protocol_msg **all_msg, int *frag_num, int pmtu, int seq_num, int msg_type, cw_protocol_msg *msg_elems, 
                                                               const int msg_elem_num, cw_protocol_msg *msg_elems_binding, const int msg_elem_binding_num, int is_crypted);
int cw_get_fragment_id(void);
cw_bool cw_thread_mutex_lock(pthread_mutex_t *mutex) ;
void cw_thread_mutex_unlock(pthread_mutex_t *mutex);


cw_bool __cw_error(const char *filename, int line_num, const char *format, ...);



#endif

