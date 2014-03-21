/***************************************************************************
* Copyright (C), 2013-2015, Beijing Teleron Telecom Engineering Co.,Ltd
* File name:     wtp_discovery.c
* Author:        renleilei - renleilei@ezlink.us
* Description:   
* Others: 
* Last modified: 2014-02-25 00:42
***************************************************************************/

#include "cw_common.h"
#include "cw_protocol.h"
#include "wtp.h"
    
int g_cw_discovery_count;
extern ac_info g_ac_info[CW_MAX_AC_NUM];

#define CW_MAX_DISCOVERIES                       10    
    
#define CW_DISCOVERY_INTERVAL                3   //5;  //seconds
#define CW_MAX_DISCOVERY_INTERVAL     4   //20;  //seconds

#define cw_wtp_found_an_ac()	(((g_ac_info[0].addr.sin_addr.s_addr != 0) && \
                                                         (g_ac_info[0].addr.sin_addr.s_addr != INADDR_BROADCAST)))

cw_bool cw_wtp_get_board_data(cw_wtp_vendors* infos)
{
    unsigned char mac[ETH_ALEN] = {0,1,2,3,4,5};
    
    if(infos == NULL) {
        cw_log("Error: argument 'infos' is null.");
        return CW_FALSE;
    }

    infos->vendor_infos_count = 1;

    CW_CREATE_ARRAY((infos->vendor_infos), infos->vendor_infos_count, cw_wtp_vendor, return cw_error("Error: malloc failed."););
    (infos->vendor_infos)[0].vendor_id = 12345;
    (infos->vendor_infos)[0].type = CW_WTP_LABEL_MAC;
    (infos->vendor_infos)[0].length = ETH_ALEN;
    CW_CREATE_OBJECT_SIZE(( ( (infos->vendor_infos)[0] ).value), (infos->vendor_infos)[1].length, return cw_error("Error: malloc failed."););
    memcpy(((infos->vendor_infos)[0]).value, (void *)mac, sizeof(mac)); // AP MAC Address
    
    return CW_TRUE;
}

 void cw_wtp_free_vendors(cw_wtp_vendors *infos) 
{
	int i;
	
	if(infos == NULL) return;
	
	for(i = 0; i < infos->vendor_infos_count; i++) {
		cw_free((infos->vendor_infos)[i].value);
	}
	
	cw_free(infos->vendor_infos);
}

cw_bool cw_build_discovery_type(cw_protocol_msg *msg)
{
    if(msg == NULL) {
        cw_log("Error: argument 'msg' is null.");
        return CW_FALSE;
    }
    
    CW_CREATE_PROTOCOL_MESSAGE(*msg, 1, return cw_error("Error: malloc failed."););
    cw_protocol_store8(msg, CW_MSG_ELEMENT_DISCOVERY_TYPE_BROADCAST);
    
    
    return cw_build_msg_elem(msg, CW_MSG_ELEMENT_DISCOVERY_TYPE_CW_TYPE);
}

cw_bool cw_build_board_data(cw_protocol_msg *msg) 
{ 
    cw_wtp_vendors  infos;
    int i, size = 0;

    if(msg == NULL) {
        cw_log("Error: argument 'msg' is null.");
        return CW_FALSE;
    }
    
    // get infos
    if(!cw_wtp_get_board_data(&infos)) {
        return CW_FALSE;
    }
	
    //Calculate msg elem size
    size += sizeof(infos.vendor_infos_count);
    for(i = 0; i < infos.vendor_infos_count; i++) {
        size += (sizeof(infos.vendor_infos[i].vendor_id)  + sizeof(infos.vendor_infos[i].type) + 
            sizeof(infos.vendor_infos[i].vendor_id) + ((infos.vendor_infos)[i]).length);
    }
	
    // create message
    CW_CREATE_PROTOCOL_MESSAGE(*msg, size, return cw_error("Error: malloc failed."););

    cw_protocol_store32(msg, ((infos.vendor_infos)[0].vendor_id));
    for(i = 0; i < infos.vendor_infos_count; i++) {
        cw_protocol_store16(msg, ((infos.vendor_infos)[i].type));
        cw_protocol_store16(msg, ((infos.vendor_infos)[i].length));
    	
        /*if((infos.vendor_infos)[i].length == 4) {
            *((infos.vendor_infos)[i].value) = htonl(*((infos.vendor_infos)[i].value));
        }*/

        cw_protocol_store_raw_bytes(msg, (void*) ((infos.vendor_infos)[i].value), (infos.vendor_infos)[i].length);

        if(CW_WTP_LABEL_MAC == (infos.vendor_infos)[i].type) {
            char mac_str[32] = {0};
            unsigned char mac[ETH_ALEN] = {0};
            memcpy(mac, (infos.vendor_infos)[i].value, sizeof(mac));
            snprintf(mac_str, sizeof(mac_str), "%02d:%02d:%02d:%02d:%02d:%02d", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
                
            cw_log("Board Data: %d - %d - %d - mac=%s", (infos.vendor_infos)[i].vendor_id, (infos.vendor_infos)[i].type, 
                    (infos.vendor_infos)[i].length, mac_str);
        } else {
            cw_log("Board Data: %d - %d - %d ", (infos.vendor_infos)[i].vendor_id, (infos.vendor_infos)[i].type, (infos.vendor_infos)[i].length);
        }
        
    }

    cw_wtp_free_vendors(&infos);

    return cw_build_msg_elem(msg, CW_MSG_ELEMENT_WTP_BOARD_DATA_CW_TYPE);
}

cw_bool cw_build_discovery_request(cw_protocol_msg **msg, int seq_num)
{
    cw_protocol_msg *elems= NULL;
    const int elem_count = 2;
    int i = -1;
    int frag_num;
    
    if(msg == NULL) {
        cw_log("Error: argument 'msg' is null.");
        return CW_FALSE;
    }

    CW_CREATE_PROTOCOL_MSG_ARRAY(elems, elem_count, return cw_error("Error: malloc failed."););

    if(!cw_build_discovery_type(&elems[++i]) ||!cw_build_board_data(&elems[++i])) {
        int k;
        for(k = 0; k <= i; k++) { 
            CW_FREE_PROTOCOL_MESSAGE(elems[i]);
        }
        cw_free(elems);
        /* error will be handled by the caller */
        return CW_FALSE;
    } 
	
    return cw_build_msg(msg, 
    			 &frag_num,
    			 0,
    			 seq_num,
    			 CW_MSG_TYPE_VALUE_DISCOVERY_REQUEST,
    			 elems,
    			 elem_count,
    			 NULL,
    			 0,
    			 CW_PACKET_PLAIN);

}

/*
* Wrapper for select
 */
cw_bool cw_network_timed_poll_read(int sock, struct timeval *timeout) 
{
    int ret;	
    fd_set fset;
	
    if(timeout == NULL) {
        return cw_error("Error: arg error.");
    }
	
    FD_ZERO(&fset);
    FD_SET(sock, &fset);

    if((ret = select(sock+1, &fset, NULL, NULL, timeout)) == 0) {
        cw_log("Select Time Expired");
        return cw_error("Error: TIME_EXPIRED.");
    } else if (ret < 0) {		
        cw_log("Select Error");
        
        if(errno == EINTR){				
            cw_log("Select Interrupted by signal");
            return cw_error("Error: select() INTERRUPTED");
        }
    }

    return CW_TRUE;
}

/*
 * Gets a datagram from network that should be a Discovery Response.
 */
cw_bool cw_rcv_discovery_response() {
#if 0
	char buf[CW_BUFFER_SIZE];
	int i;
	struct in_addr addr;
	ac_info *ac;
	int seq_num;
	int read_bytes;
	
	/* receive the datagram */
	if(!CWErr(CWNetworkReceiveUnsafe(g_wtp_socket,
					 buf,
					 CW_BUFFER_SIZE-1,
					 0,
					 &addr,
					 &read_bytes))) {
		return CW_FALSE;
	}
	
        CW_CREATE_OBJECT(ac, ac_info, return cw_error("Error: malloc failed."););
	
	/* check if it is a valid Discovery Response */
	if(!CWErr(CWParseDiscoveryResponseMessage(buf, read_bytes, &seq_num, ac))) {

		cw_free(ac);
		return cw_error("Received something different from a\
				     Discovery Response while in Discovery State");
	}

	addr.s_addr = ac.ip.s_addr;

	cw_log("WTP Receives Discovery Response");

	/* check if the sequence number we got is correct */
	for(i = 0; i < gCWACCount; i++) {
	
		if(gCWACList[i].seqNum == seqNum) {
		
			CWUseSockNtop(&addr,
				      CWLog("Discovery Response from:%s", str););
			/* we received response from this address */
			gCWACList[i].received = CW_TRUE;
	
			return CW_TRUE;
		}
	}
#endif	
	return cw_error("Sequence Number of Response doesn't macth Request");
}

/* 
 * Wait DiscoveryInterval time while receiving Discovery Responses.
 */
cw_bool cw_read_response(void) 
{
    cw_bool  result = CW_FALSE;
#if 0	
    struct timeval timeout, before, after, delta, new_timeout;
	
    timeout.tv_sec = new_timeout.tv_sec = CW_DISCOVERY_INTERVAL;
    timeout.tv_usec = new_timeout.tv_usec = 0;
	
    gettimeofday(&before, NULL);

    CW_REPEAT_FOREVER {
        /* check if something is available to read until newTimeout */
        if(cw_network_timed_poll_read(g_wtp_socket, &new_timeout)) { 
            /* success
            * if there was no error, raise a "success error", so we can easily handle
            * all the cases in the switch
        */
            cw_log("select success.");
            cw_rcv_discovery_response();
        } else {

        }

        switch(CWErrorGetLastErrorCode()) {
            case CW_ERROR_TIME_EXPIRED:
                goto cw_time_over;
            break;
				
            case CW_ERROR_SUCCESS:
                result = CWReceiveDiscoveryResponse();
            case CW_ERROR_INTERRUPTED: 
				/*
				 * something to read OR interrupted by the system
				 * wait for the remaining time (NetworkPoll will be recalled with the remaining time)
				 */
				gettimeofday(&after, NULL);

				CWTimevalSubtract(&delta, &after, &before);
				if(CWTimevalSubtract(&newTimeout, &timeout, &delta) == 1) { 
					/* negative delta: time is over */
					goto cw_time_over;
				}
				break;
			default:
				CWErrorHandleLast();
				goto cw_error;
				break;	
		}
	}
	cw_time_over:
		/* time is over */
		CWDebugLog("Timer expired during receive");	
	cw_error:
#endif        
		return result;
}

cw_state_transition cw_wtp_send_discovery(void)
{
    int seq_num;
    int ret = 0;
    
    //sleep(cw_random_int_in_range(CW_DISCOVERY_INTERVAL, CW_MAX_DISCOVERY_INTERVAL));

    g_cw_discovery_count = 0;;

    CW_REPEAT_FOREVER {
        cw_protocol_msg *msg = NULL;
        
        /* we get no responses for a very long time */
        if(g_cw_discovery_count >= CW_MAX_DISCOVERIES) {
            cw_log("Notice: reach max discovery count.");
            g_cw_discovery_count = 0;
            return CW_ENTER_SULKING;
        }
        
        if(cw_wtp_found_an_ac()) 
            break;
		
        seq_num = cw_get_seq_num();

        if(! cw_build_discovery_request(&msg, seq_num)) {
            cw_log("build discovery request packet failed..");
            return CW_ENTER_SULKING;
        }
        
        g_ac_info[0].addr.sin_family = AF_INET;
        g_ac_info[0].addr.sin_addr.s_addr = INADDR_BROADCAST;
        g_ac_info[0].addr.sin_port = htons(CW_CONTROL_PORT);
        
        ret = cw_send_unconnected(g_wtp_socket,
        					 &(g_ac_info[0].addr),
        					 (*msg).msg,
        					 (*msg).offset); 
        /* 
         * log eventual error and continue
         * CWUseSockNtop(&(gACInfoPtr->preferredAddress),
         * 		 CWLog("WTP sends Discovery Request to: %s", str););
         */
        				
        CW_FREE_PROTOCOL_MESSAGE(*msg);
        cw_free(msg);

        g_cw_discovery_count++;

        /* wait for Responses */
        if(cw_read_response() && cw_wtp_found_an_ac()) {
            /* we read at least one valid Discovery Response */
            break;
        }
		
        cw_log("WTP Discovery-To-Discovery (%d)", g_cw_discovery_count);

        //sleep(cw_random_int_in_range(CW_DISCOVERY_INTERVAL, CW_MAX_DISCOVERY_INTERVAL));
    }
    
    return CW_ENTER_JOIN;
}

