/***************************************************************************
* Copyright (C), 2013-2015, Beijing Teleron Telecom Engineering Co.,Ltd
* File name:     cw_protocol.c
* Author:        renleilei - renleilei@ezlink.us
* Description:   
* Others: 
* Last modified: 2014-02-25 00:45
***************************************************************************/

#include "cw_common.h"
#include "cw_protocol.h"

const int gMaxCAPWAPHeaderSizeBinding = 16;

static const int gMaxDTLSHeaderSize = 25; // see http://crypto.stanford.edu/~nagendra/papers/dtls.pdf
static const int gMaxCAPWAPHeaderSize = 8; // note: this include optional Wireless field

char   g_radio_mac[ETH_ALEN];
pthread_mutex_t    g_radio_mac_mutex;


// stores 8 bits in the message, increments the current offset in bytes
void cw_protocol_store8(cw_protocol_msg *msg, unsigned char val) 
{
    CW_COPY_MEMORY(&((msg->msg)[(msg->offset)]), &(val), 1);
    (msg->offset) += 1;
}

// stores 16 bits in the message, increments the current offset in bytes
void cw_protocol_store16(cw_protocol_msg *msg, unsigned short val) 
{
    val = htons(val);
    CW_COPY_MEMORY(&((msg->msg)[(msg->offset)]), &(val), 2);
    (msg->offset) += 2;
}

// stores 32 bits in the message, increments the current offset in bytes
void cw_protocol_store32(cw_protocol_msg *msg, unsigned int val) 
{
    val = htonl(val);
    CW_COPY_MEMORY(&((msg->msg)[(msg->offset)]), &(val), 4);
    (msg->offset) += 4;
}

// stores len bytes in the message, increments the current offset in bytes.
void cw_protocol_store_raw_bytes(cw_protocol_msg *msg, void *bytes, int len) 
{
    CW_COPY_MEMORY(&((msg->msg)[(msg->offset)]), bytes, len);
    (msg->offset) += len;
}

// stores another message in the message, increments the current offset in bytes.
void cw_protocol_store_message(cw_protocol_msg *msg, cw_protocol_msg *msg_to_store) 
{
    CW_COPY_MEMORY(&((msg->msg)[(msg->offset)]), msg_to_store->msg, msg_to_store->offset);
    (msg->offset) += msg_to_store->offset;
}

// Assembles the Control Header
cw_bool cw_build_control_header(cw_protocol_msg* control_hdr, cw_control_header* val) 
{
    if(control_hdr == NULL || val == NULL) {
        return cw_error("Error: args error.");
    }
	
    CW_CREATE_PROTOCOL_MESSAGE(*control_hdr, 8,	 return cw_error("Error: malloc failed."););
	
    cw_protocol_store32(control_hdr, val->msg_type);
    cw_protocol_store8(control_hdr, val->seq_num);
    cw_protocol_store16(control_hdr, (CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS+(val->msg_elems_len))); // 7 is for the next 8+32+16 bits (= 7 bytes), MessageElementsLength+flags + timestamp 
    cw_protocol_store8(control_hdr, 0); // flags
    //CWProtocolStore32(controlHdrPtr, ((unsigned int)(time(NULL))) ); // timestamp
	
    return CW_TRUE;
}

cw_bool cw_build_transport_header_binding(cw_protocol_msg* transport_hdr, cw_binding_transport_header *value)
{
	int val = 0;
/* Mauro: non piu' specificato nel campo Wireless Specific Information 
	CW_SET_FIELD32(val,
		     CW_TRANSPORT_HEADER_WIRELESS_ID_START,
		     CW_TRANSPORT_HEADER_WIRELESS_ID_LEN,
		     CW_BINDING_WIRELESSID);
*/
	CW_SET_FIELD32(val,
		     CW_TRANSPORT_HEADER_LENGTH_START,
		     CW_TRANSPORT_HEADER_LENGTH_LEN,
		     CW_BINDING_DATALENGTH);

//	CWDebugLog("#### RSSI in= %d",value->RSSI );
	CW_SET_FIELD32(val,
		     CW_TRANSPORT_HEADER_RSSI_START,
		     CW_TRANSPORT_HEADER_RSSI_LEN,
		     value->rssi);

//	CWDebugLog("#### SNR in= %d",value->SNR );
	CW_SET_FIELD32(val,
		     CW_TRANSPORT_HEADER_SNR_START,
		     CW_TRANSPORT_HEADER_SNR_LEN,
		     value->snr);

/* Mauro: inserisci il byte piu' significativo del sottocampo Data */
	CW_SET_FIELD32(val,
		     CW_TRANSPORT_HEADER_DATARATE_1_START,
		     CW_TRANSPORT_HEADER_DATARATE_1_LEN,
		     (value->data_rate)>>8);

	cw_protocol_store32(transport_hdr, val);
	val = 0;

/* Mauro: inserisci il byte meno significativo del sottocampo Data */
	CW_SET_FIELD32(val,
		     CW_TRANSPORT_HEADER_DATARATE_2_START,
		     CW_TRANSPORT_HEADER_DATARATE_2_LEN,
		     (value->data_rate) & 0x000000FF);

//	CWDebugLog("#### data rate in= %d",value->dataRate );
/*	CW_SET_FIELD32(val,
		     CW_TRANSPORT_HEADER_DATARATE_START,
		     CW_TRANSPORT_HEADER_DATARATE_LEN,
		     value->dataRate);
*/
	CW_SET_FIELD32(val,
		     CW_TRANSPORT_HEADER_PADDING_START,
		     CW_TRANSPORT_HEADER_PADDING_LEN,
		     0);

	cw_protocol_store32(transport_hdr, val);

	return CW_TRUE;
}

// Assembles the Transport Header
cw_bool cw_build_transport_header(cw_protocol_msg* transport_hdr, cw_protocol_transport_header* value) 
{
    char radio_mac_present = 0;
    unsigned int val = 0;
    int i;

    for(i = 0; i < 6; i++) {
        //printf(":::: %02X\n",g_radio_mac[i]);
        if( g_radio_mac[i] != 0 ) {
            radio_mac_present = 8;
            break;
        }
    }
	
    if(transport_hdr == NULL || value == NULL)  {
        return cw_error("Error: args error.");
    }
	
    if(value->binding != NULL) {
        CW_CREATE_PROTOCOL_MESSAGE(*transport_hdr,  16 + radio_mac_present, return cw_error("Error: malloc failed."););
    } else {
        CW_CREATE_PROTOCOL_MESSAGE(*transport_hdr,8 + radio_mac_present, return cw_error("Error: malloc failed."););
    }	 
    // meaningful bytes of the header (no wirless header and MAC address)
    CW_SET_FIELD32(val, 
    	     CW_TRANSPORT_HEADER_VERSION_START,
    	     CW_TRANSPORT_HEADER_VERSION_LEN,
    	     CW_PROTOCOL_VERSION); // current version of CAPWAP


    CW_SET_FIELD32(val,
    	     CW_TRANSPORT_HEADER_TYPE_START,
    	     CW_TRANSPORT_HEADER_TYPE_LEN,
    	     (value->payload_type == CW_PACKET_PLAIN) ? 0 : 1);

    if(radio_mac_present) {
    	if(value->binding != NULL){
    		CW_SET_FIELD32(val,
    				 CW_TRANSPORT_HEADER_HLEN_START,
    				 CW_TRANSPORT_HEADER_HLEN_LEN,
    				 CW_BINDING_HLEN + 2);
    	}else {
    		CW_SET_FIELD32(val,
    				 CW_TRANSPORT_HEADER_HLEN_START,
    				 CW_TRANSPORT_HEADER_HLEN_LEN,
    				 2 + 2);
        }
    }else if(value->binding != NULL) {
    		CW_SET_FIELD32(val,
    				 CW_TRANSPORT_HEADER_HLEN_START,
    				 CW_TRANSPORT_HEADER_HLEN_LEN,
    				 CW_BINDING_HLEN);
    }else {
    		CW_SET_FIELD32(val,
    				 CW_TRANSPORT_HEADER_HLEN_START,
    				 CW_TRANSPORT_HEADER_HLEN_LEN,
    				 2);
    }
        
    CW_SET_FIELD32(val,
    	     CW_TRANSPORT_HEADER_RID_START,
    	     CW_TRANSPORT_HEADER_RID_LEN,
    	     0); // only one radio per WTP?

    CW_SET_FIELD32(val,
    	     CW_TRANSPORT_HEADER_WBID_START,
    	     CW_TRANSPORT_HEADER_WBID_LEN,
    	     1); // Wireless Binding ID




    if(value->binding != NULL)
    	CW_SET_FIELD32(val,
    	     CW_TRANSPORT_HEADER_T_START,
    	     CW_TRANSPORT_HEADER_T_LEN,
    	     1);

    else if(value->type==1)
    	CW_SET_FIELD32(val,
    	     CW_TRANSPORT_HEADER_T_START,
    	     CW_TRANSPORT_HEADER_T_LEN,
    	     1);
    else 
    	CW_SET_FIELD32(val,
    	     CW_TRANSPORT_HEADER_T_START,
    	     CW_TRANSPORT_HEADER_T_LEN,
    	     0);



    CW_SET_FIELD32(val,
    	     CW_TRANSPORT_HEADER_F_START,
    	     CW_TRANSPORT_HEADER_F_LEN,
    	     value->is_fragment); // is fragment

    CW_SET_FIELD32(val,
    	     CW_TRANSPORT_HEADER_L_START,
    	     CW_TRANSPORT_HEADER_L_LEN,
    	     value->last); // last fragment

    if(value->binding != NULL)
    	CW_SET_FIELD32(val,
    		     CW_TRANSPORT_HEADER_W_START,
    		     CW_TRANSPORT_HEADER_W_LEN,
    		     1); //wireless header
    else 
    	CW_SET_FIELD32(val,
    		     CW_TRANSPORT_HEADER_W_START,
    		     CW_TRANSPORT_HEADER_W_LEN,
    		     0);

    if(radio_mac_present)
    	CW_SET_FIELD32(val,
    			 CW_TRANSPORT_HEADER_M_START,
    			 CW_TRANSPORT_HEADER_M_LEN,
    			 1); //radio MAC address
    else
    	CW_SET_FIELD32(val,
    			 CW_TRANSPORT_HEADER_M_START,
    			 CW_TRANSPORT_HEADER_M_LEN,
    			 0); // no radio MAC address
    CW_SET_FIELD32(val,
    	     CW_TRANSPORT_HEADER_K_START,
    	     CW_TRANSPORT_HEADER_K_LEN,
    	     0); // Keep alive flag

    CW_SET_FIELD32(val,
    	     CW_TRANSPORT_HEADER_FLAGS_START,
    	     CW_TRANSPORT_HEADER_FLAGS_LEN,
    	     0); // required

    cw_protocol_store32(transport_hdr, val);
    // end of first 32 bits

    val = 0;

    CW_SET_FIELD32(val,
    	     CW_TRANSPORT_HEADER_FRAGMENT_ID_START,
    	     CW_TRANSPORT_HEADER_FRAGMENT_ID_LEN,
    	     value->fragment_id); // fragment ID

    CW_SET_FIELD32(val,
    	     CW_TRANSPORT_HEADER_FRAGMENT_OFFSET_START,
    	     CW_TRANSPORT_HEADER_FRAGMENT_OFFSET_LEN,
    	     value->fragment_offset); // fragment offset

    CW_SET_FIELD32(val,
    	     CW_TRANSPORT_HEADER_RESERVED_START,
    	     CW_TRANSPORT_HEADER_RESERVED_LEN,
    	     0); // required

    cw_protocol_store32(transport_hdr, val);
    // end of second 32 bits

    if(radio_mac_present){
        cw_protocol_store8(transport_hdr,6);
    	
        cw_thread_mutex_lock(&g_radio_mac_mutex);
        cw_protocol_store_raw_bytes(transport_hdr, g_radio_mac, 6);
        cw_thread_mutex_unlock(&g_radio_mac_mutex);
    	
        cw_protocol_store8(transport_hdr,0);
    }


    if(value->binding != NULL){
        if (!cw_build_transport_header_binding(transport_hdr, value->binding))
            return CW_FALSE;
    }

    return CW_TRUE;
}

// Assemble a Message Element creating the appropriate header and storing the message.
cw_bool cw_build_msg_elem(cw_protocol_msg *msg, unsigned int type) 
{
    cw_protocol_msg all_msg;
	
    if(msg == NULL) 
        return cw_error("Error: argument msg is NULL.");
	
    CW_CREATE_PROTOCOL_MESSAGE(all_msg, 6+(msg->offset), return cw_error("Error: malloc failed."););

    // store header
    cw_protocol_store16(&all_msg, type);
    cw_protocol_store16(&all_msg, msg->offset); // size of the body

    // store body
    cw_protocol_store_message(&all_msg, msg);

    CW_FREE_PROTOCOL_MESSAGE(*msg);

    msg->msg = all_msg.msg;
    msg->offset = all_msg.offset;

    return CW_TRUE;
}

// Assemble a CAPWAP Control Packet, with the given Message Elements, Sequence Number and Message Type. Create Transport and Control Headers.
// completeMsgPtr is an array of fragments (can be of size 1 if the packet doesn't need fragmentation
cw_bool  cw_build_msg(cw_protocol_msg **all_msg, int *frag_num, int pmtu, int seq_num, int msg_type, cw_protocol_msg *msg_elems, 
        const int msg_elem_num, cw_protocol_msg *msg_elems_binding, const int msg_elem_binding_num, int is_crypted) 
{
        
    cw_protocol_msg transport_hdr, control_hdr, msg;
    int msg_elems_len = 0;
    int i;
	
    cw_protocol_transport_header transport;
    cw_control_header control;
	
    if(all_msg == NULL || frag_num == NULL || (msg_elems == NULL && msg_elem_num > 0) 
        || (msg_elems_binding == NULL && msg_elem_binding_num > 0)) {
        return cw_error("Error: args error.");
    }
	
        //Calculate the whole size of the Msg Elements	
    for(i = 0; i < msg_elem_num; i++) {
        msg_elems_len += msg_elems[i].offset;
    }
    
    for(i = 0; i < msg_elem_binding_num; i++) {
        msg_elems_len += msg_elems_binding[i].offset;
    }

    //Assemble Control Header
    control.msg_type = msg_type;
    control.msg_elems_len = msg_elems_len;
    control.seq_num = seq_num;
	
    if(!(cw_build_control_header(&control_hdr, &control))) {
        CW_FREE_PROTOCOL_MESSAGE(control_hdr);
        for(i = 0; i < msg_elem_num; i++) { 
            CW_FREE_PROTOCOL_MESSAGE(msg_elems[i]);
        }
        cw_free(msg_elems);
        for(i = 0; i < msg_elem_binding_num; i++) { 
            CW_FREE_PROTOCOL_MESSAGE(msg_elems_binding[i]);
        }
        cw_free(msg_elems_binding);
        return CW_FALSE; // will be handled by the caller
    }
	
    // assemble the message putting all the data consecutively
    CW_CREATE_PROTOCOL_MESSAGE(msg, control_hdr.offset + msg_elems_len, return cw_error("Error: malloc failed."););
	
    cw_protocol_store_message(&msg, &control_hdr);
    for(i = 0; i < msg_elem_num; i++) { // store in the request all the Message Elements
        cw_protocol_store_message(&msg, &(msg_elems[i]));
    }
    for(i = 0; i < msg_elem_binding_num; i++) { // store in the request all the Message Elements
        cw_protocol_store_message(&msg, &(msg_elems_binding[i]));
    }

    //Free memory not needed anymore
    CW_FREE_PROTOCOL_MESSAGE(control_hdr);
    for(i = 0; i < msg_elem_num; i++) { 
        CW_FREE_PROTOCOL_MESSAGE(msg_elems[i]);
    }
    cw_free(msg_elems);
    for(i = 0; i < msg_elem_binding_num; i++) { 
        CW_FREE_PROTOCOL_MESSAGE(msg_elems_binding[i]);
    }
    cw_free(msg_elems_binding);
	
//	cw_log("PMTU: %d", PMTU);
	
    // handle fragmentation
    pmtu = pmtu - gMaxDTLSHeaderSize - gMaxCAPWAPHeaderSize;
	
    if(pmtu > 0) {
        pmtu = (pmtu/8)*8; // CAPWAP fragments are made of groups of 8 bytes
        if(pmtu == 0) 
            goto cw_dont_fragment;
		
        //cw_log("Aligned PMTU: %d", PMTU);
        *frag_num = msg.offset / pmtu;
        if((msg.offset % pmtu) != 0) {
            (*frag_num)++;
        }
            //cw_log("Fragments #: %d", *fragmentsNumPtr);
    } else {
	cw_dont_fragment:
		*frag_num = 1;
    }
	
    transport.binding = NULL;
		
    if(*frag_num == 1) {
        cw_log("1 Fragment");

        CW_CREATE_OBJECT(*all_msg, cw_protocol_msg, return cw_error("Error: malloc failed."););
		
        transport.is_fragment = transport.last = transport.fragment_offset = transport.fragment_id = 0;
        transport.payload_type = is_crypted;
//		transportVal.last = 1;

        // Assemble Message Elements
        if(!(cw_build_transport_header(&transport_hdr, &transport))) {
            CW_FREE_PROTOCOL_MESSAGE(msg);
            CW_FREE_PROTOCOL_MESSAGE(transport_hdr);
            cw_free(all_msg);
            return CW_FALSE; // will be handled by the caller
        } 
	
        // assemble the message putting all the data consecutively
        CW_CREATE_PROTOCOL_MESSAGE(((*all_msg)[0]), transport_hdr.offset + msg.offset, return cw_error("Error: malloc failed."););
		
        cw_protocol_store_message(&((*all_msg)[0]), &transport_hdr);
        cw_protocol_store_message(&((*all_msg)[0]), &msg);
		
        CW_FREE_PROTOCOL_MESSAGE(transport_hdr);
        CW_FREE_PROTOCOL_MESSAGE(msg);
    } else {
        int frag_id = cw_get_fragment_id();
        int total_size = msg.offset;
        //CWDebugLog("%d Fragments", *fragmentsNumPtr);

        CW_CREATE_PROTOCOL_MSG_ARRAY(*all_msg, *frag_num, return cw_error("Error: malloc failed."););
        msg.offset = 0;
	
        for(i = 0; i < *frag_num; i++) { // for each fragment to assemble
            int frag_size;
			
            transport.is_fragment = 1;
            transport.fragment_offset = msg.offset / 8;
            transport.fragment_id = frag_id;
            transport.payload_type = is_crypted;

            if(i < ((*frag_num)-1)) { // not last fragment
                frag_size = pmtu;
                transport.last = 0;
            } else { // last fragment
                frag_size = total_size - (((*frag_num)-1) * pmtu);
                transport.last = 1;
            }

            //CWDebugLog("Fragment #:%d, offset:%d, bytes stored:%d/%d", i, transportVal.fragment_offset, fragSize, totalSize);
			
            // Assemble Transport Header for this fragment
            if(!(cw_build_transport_header(&transport_hdr, &transport))) {
                CW_FREE_PROTOCOL_MESSAGE(msg);
                cw_free(all_msg);
                return CW_FALSE; // will be handled by the caller
            }

            CW_CREATE_PROTOCOL_MESSAGE(((*all_msg)[i]), transport_hdr.offset + frag_size, return cw_error("Error: malloc failed."););
			
            cw_protocol_store_message(&((*all_msg)[i]), &transport_hdr);
            cw_protocol_store_raw_bytes(&((*all_msg)[i]), &((msg.msg)[msg.offset]), frag_size);
            msg.offset += frag_size;
			
            CW_FREE_PROTOCOL_MESSAGE(transport_hdr);
        }
        CW_FREE_PROTOCOL_MESSAGE(msg);
    }
	
    return CW_TRUE;
}


