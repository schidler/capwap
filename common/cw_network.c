/***************************************************************************
* Copyright (C), 2013-2015, Beijing Teleron Telecom Engineering Co.,Ltd
* File name:     cw_network.c
* Author:        renleilei - renleilei@ezlink.us
* Description:   
* Others: 
* Last modified: 2014-02-28 01:50
***************************************************************************/

#include "cw_common.h"

/*
 * Assume address is valid
 */
__inline__ int cw_get_addr_size(struct sockaddr_in *addr) {
	
	switch ( ((struct sockaddr*)(addr))->sa_family ) {
		
	#ifdef	IPV6
		/* IPv6 is defined in Stevens' library */
		case AF_INET6:
			return sizeof(struct sockaddr_in6);
			break;
	#endif
		case AF_INET:
		default:
			return sizeof(struct sockaddr_in);
	}
}

/* 
 * Send buf on an unconnected UDP socket. Unsafe means that we don't use DTLS.
 */
cw_bool cw_send_unconnected(int sock, 
				      struct sockaddr_in  *addr,
				      const char *buf,
				      int len) 
{
    if(buf == NULL || addr == NULL) 
        return  cw_error("Error: arg error.");
    
    if(sendto(sock, buf, len, 0, (struct sockaddr*)addr, cw_get_addr_size(addr)) < 0) {
        //if(errno == EINTR) 
            //continue;
        cw_error("Error: sending error-- %s.", strerror(errno));
    }
    return CW_TRUE;
}

/*
 * Send buf on a "connected" UDP socket. Unsafe means that we don't use DTLS.
 */
cw_bool cw_send_connected(int sock, const char *buf, int len) 
{
    if(buf == NULL) 
        return cw_error("Error: arg error.");

    while(send(sock, buf, len, 0) < 0) {
        if(errno == EINTR) 
            continue;
        cw_error("Error: sending error.");
    }
    return CW_TRUE;
}

/* 
 * Receive a datagram on an connected UDP socket (blocking).
 * Unsafe means that we don't use DTLS.
 */
cw_bool cw_rcv_connected(int sock, char *buf, int len, int *read_buf) 
{	
    if(buf == NULL) 
        return cw_error("Error: arg error.");
	
    while((*read_buf = recv(sock, buf, len, 0)) < 0) {
        if(errno == EINTR) 
            continue;
        cw_error("Error: recv error.");
    }
    return CW_TRUE;
}

/*
 * Init network for client.
 */
cw_bool cw_init_socket_client(int *sock, const char *addr) 
{	
    int yes = 1;
#if 0
    struct addrinfo hints, *res;
    char myport[8] = {0};

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; 
    
    sprintf(myport,"%d",CW_CONTROL_PORT);
    /* NULL addr means that we don't want to connect to a 
    * specific address
    */
    
    getaddrinfo(addr, myport, &hints, &res);
#endif 	
    if(sock == NULL) 
        return cw_error("Error: arg is null.");
	
	if(((*sock)=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		return cw_error("Error: create socket failed.");
	}
#if 0
    setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    
    if(bind(*sock, res->ai_addr, res->ai_addrlen) < 0) {
        close(*sock);
        cw_log("failed to bind Client socket\n");
        return CW_FALSE;
    }
	
    cw_log("Binding Client socket with UDP port:%d\n", CW_CONTROL_PORT);

    if(addr != NULL) {
        //char str[64] = {0};
        //inet_ntop(AF_INET, &addr->sin_addr, str, sizeof(str));

        if(connect((*sock), res->ai_addr, sizeof(struct sockaddr)) < 0) {

            cw_log("connect error.");
        }
    }
#endif    
    /* allow sending broadcast packets */
    setsockopt(*sock, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes));
	
    return CW_TRUE;
}

