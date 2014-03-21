/***************************************************************************
* Copyright (C), 2013-2015, Beijing Teleron Telecom Engineering Co.,Ltd
* File name:     cw_protocol.h
* Author:        renleilei - renleilei@ezlink.us
* Description:   
* Others: 
* Last modified: 2014-02-24 23:30
***************************************************************************/

#include "cw_binding.h"

#ifndef __CW_PROTOCOL_H__
#define __CW_PROTOCOL_H__

#define		CW_CONTROL_PORT						5246
#define		CW_DATA_PORT						5247
#define		CW_MAX_FRAGMENT_ID					65535
#define		CW_PROTOCOL_VERSION					0
#define		CW_MAX_SEQ_NUM						255
#define		CW_PACKET_PLAIN						0
#define		CW_PACKET_CRYPT						1

#define 	CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS			3		//Offset "Seq Num" - "Message Elements"

// <TRANSPORT_HEADER_FIELDS>
// CAPWAP version (currently 0)
#define 	CW_TRANSPORT_HEADER_VERSION_START			0
#define 	CW_TRANSPORT_HEADER_VERSION_LEN				4

// Mauro
#define		CW_TRANSPORT_HEADER_TYPE_START				4
#define		CW_TRANSPORT_HEADER_TYPE_LEN				4

// Radio ID number (for WTPs with multiple radios)
#define 	CW_TRANSPORT_HEADER_RID_START				13
#define 	CW_TRANSPORT_HEADER_RID_LEN				5

// Length of CAPWAP tunnel header in 4 byte words 
#define 	CW_TRANSPORT_HEADER_HLEN_START				8
#define 	CW_TRANSPORT_HEADER_HLEN_LEN				5

// Wireless Binding ID
#define 	CW_TRANSPORT_HEADER_WBID_START				18
#define 	CW_TRANSPORT_HEADER_WBID_LEN				5

// Format of the frame
#define 	CW_TRANSPORT_HEADER_T_START				23
#define 	CW_TRANSPORT_HEADER_T_LEN				1

// Is a fragment?
#define 	CW_TRANSPORT_HEADER_F_START				24
#define 	CW_TRANSPORT_HEADER_F_LEN				1

// Is NOT the last fragment?
#define 	CW_TRANSPORT_HEADER_L_START				25
#define 	CW_TRANSPORT_HEADER_L_LEN				1

// Is the Wireless optional header present?
#define 	CW_TRANSPORT_HEADER_W_START				26
#define 	CW_TRANSPORT_HEADER_W_LEN				1

// Is the Radio MAC Address optional field present?
#define 	CW_TRANSPORT_HEADER_M_START				27
#define 	CW_TRANSPORT_HEADER_M_LEN				1

// Is the message a keep alive?
#define 	CW_TRANSPORT_HEADER_K_START				28
#define 	CW_TRANSPORT_HEADER_K_LEN				1

// Set to 0 in this version of the protocol
#define 	CW_TRANSPORT_HEADER_FLAGS_START				29
#define 	CW_TRANSPORT_HEADER_FLAGS_LEN				3

// ID of the group of fragments
#define 	CW_TRANSPORT_HEADER_FRAGMENT_ID_START			0
#define 	CW_TRANSPORT_HEADER_FRAGMENT_ID_LEN			16

// Position of this fragment in the group 
#define 	CW_TRANSPORT_HEADER_FRAGMENT_OFFSET_START		16
#define 	CW_TRANSPORT_HEADER_FRAGMENT_OFFSET_LEN			13

// Set to 0 in this version of the protocol
#define 	CW_TRANSPORT_HEADER_RESERVED_START			29
#define 	CW_TRANSPORT_HEADER_RESERVED_LEN			3
// </TRANSPORT_HEADER_FIELDS>

// Message Type Values
#define		CW_MSG_TYPE_VALUE_DISCOVERY_REQUEST			1
#define		CW_MSG_TYPE_VALUE_DISCOVERY_RESPONSE			2
#define		CW_MSG_TYPE_VALUE_JOIN_REQUEST				3
#define		CW_MSG_TYPE_VALUE_JOIN_RESPONSE				4
#define		CW_MSG_TYPE_VALUE_CONFIGURE_REQUEST			5
#define		CW_MSG_TYPE_VALUE_CONFIGURE_RESPONSE			6
#define		CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_REQUEST		7
#define		CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_RESPONSE		8
#define		CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST			9
#define		CW_MSG_TYPE_VALUE_WTP_EVENT_RESPONSE			10
#define		CW_MSG_TYPE_VALUE_CHANGE_STATE_EVENT_REQUEST		11
#define		CW_MSG_TYPE_VALUE_CHANGE_STATE_EVENT_RESPONSE		12
#define		CW_MSG_TYPE_VALUE_ECHO_REQUEST				13
#define		CW_MSG_TYPE_VALUE_ECHO_RESPONSE				14
#define		CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST			15
#define		CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE			16
#define		CW_MSG_TYPE_VALUE_RESET_REQUEST				17
#define		CW_MSG_TYPE_VALUE_RESET_RESPONSE			18
#define		CW_MSG_TYPE_VALUE_PRIMARY_DISCOVERY_REQUEST		19
#define		CW_MSG_TYPE_VALUE_PRIMARY_DISCOVERY_RESPONSE		20
#define		CW_MSG_TYPE_VALUE_DATA_TRANSFER_REQUEST			21
#define		CW_MSG_TYPE_VALUE_DATA_TRANSFER_RESPONSE		22
#define		CW_MSG_TYPE_VALUE_CLEAR_CONFIGURATION_REQUEST		23
#define		CW_MSG_TYPE_VALUE_CLEAR_CONFIGURATION_RESPONSE		24
#define		CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_REQUEST		25
#define		CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_RESPONSE	26

// IEEE 802.11 Binding Type
#define		CW_MSG_TYPE_VALUE_WLAN_CONFIGURATION_REQUEST		3398913
#define		CW_MSG_TYPE_VALUE_WLAN_CONFIGURATION_RESPONSE		3398914

// Message Elements Type Values
#define 	CW_MSG_ELEMENT_AC_DESCRIPTOR_CW_TYPE			1
#define 	CW_MSG_ELEMENT_AC_IPV4_LIST_CW_TYPE			2
#define 	CW_MSG_ELEMENT_AC_IPV6_LIST_CW_TYPE			3
#define 	CW_MSG_ELEMENT_AC_NAME_CW_TYPE				4
#define 	CW_MSG_ELEMENT_AC_NAME_INDEX_CW_TYPE			5
#define		CW_MSG_ELEMENT_TIMESTAMP_CW_TYPE			6
#define		CW_MSG_ELEMENT_ADD_MAC_ACL_CW_TYPE			7
#define		CW_MSG_ELEMENT_ADD_STATION_CW_TYPE			8
#define		CW_MSG_ELEMENT_ADD_STATIC_MAC_ACL_CW_TYPE		9
#define 	CW_MSG_ELEMENT_CW_CONTROL_IPV4_ADDRESS_CW_TYPE		10
#define 	CW_MSG_ELEMENT_CW_CONTROL_IPV6_ADDRESS_CW_TYPE		11
#define		CW_MSG_ELEMENT_CW_TIMERS_CW_TYPE			12
#define		CW_MSG_ELEMENT_DATA_TRANSFER_DATA_CW_TYPE		13
#define		CW_MSG_ELEMENT_DATA_TRANSFER_MODE_CW_TYPE		14
#define 	CW_MSG_ELEMENT_CW_DECRYPT_ER_REPORT_CW_TYPE		15
#define 	CW_MSG_ELEMENT_CW_DECRYPT_ER_REPORT_PERIOD_CW_TYPE	16
#define 	CW_MSG_ELEMENT_DELETE_MAC_ACL_CW_TYPE			17
#define 	CW_MSG_ELEMENT_DELETE_STATION_CW_TYPE			18
#define 	CW_MSG_ELEMENT_DELETE_STATIC_MAC_ACL_CW_TYPE		19
#define 	CW_MSG_ELEMENT_DISCOVERY_TYPE_CW_TYPE			20
#define 	CW_MSG_ELEMENT_DUPLICATE_IPV4_ADDRESS_CW_TYPE		21
#define 	CW_MSG_ELEMENT_DUPLICATE_IPV6_ADDRESS_CW_TYPE		22
#define 	CW_MSG_ELEMENT_IDLE_TIMEOUT_CW_TYPE			23
#define 	CW_MSG_ELEMENT_IMAGE_DATA_CW_TYPE			24
#define 	CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE			25
#define 	CW_MSG_ELEMENT_IMAGE_INFO_CW_TYPE			26
#define 	CW_MSG_ELEMENT_INITIATED_DOWNLOAD_CW_TYPE		27
#define 	CW_MSG_ELEMENT_LOCATION_DATA_CW_TYPE			28
#define 	CW_MSG_ELEMENT_MAX_MSG_LEN_CW_TYPE			29
#define 	CW_MSG_ELEMENT_WTP_IPV4_ADDRESS_CW_TYPE			30
#define 	CW_MSG_ELEMENT_RADIO_ADMIN_STATE_CW_TYPE		31
#define 	CW_MSG_ELEMENT_RADIO_OPERAT_STATE_CW_TYPE		32
#define 	CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE			33
#define 	CW_MSG_ELEMENT_RETURNED_MSG_ELEM_CW_TYPE		34
#define 	CW_MSG_ELEMENT_SESSION_ID_CW_TYPE			35
#define 	CW_MSG_ELEMENT_STATISTICS_TIMER_CW_TYPE			36
#define 	CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_BW_CW_TYPE		37
#define 	CW_MSG_ELEMENT_WTP_BOARD_DATA_CW_TYPE			38
#define 	CW_MSG_ELEMENT_WTP_DESCRIPTOR_CW_TYPE			39
#define 	CW_MSG_ELEMENT_WTP_FALLBACK_CW_TYPE			40
#define 	CW_MSG_ELEMENT_WTP_FRAME_TUNNEL_MODE_CW_TYPE		41
#define 	CW_MSG_ELEMENT_WTP_MAC_TYPE_CW_TYPE			44
#define 	CW_MSG_ELEMENT_WTP_NAME_CW_TYPE				45
#define 	CW_MSG_ELEMENT_WTP_OPERAT_STATISTICS_CW_TYPE		46
#define 	CW_MSG_ELEMENT_WTP_RADIO_STATISTICS_CW_TYPE		47
#define 	CW_MSG_ELEMENT_WTP_REBOOT_STATISTICS_CW_TYPE		48
#define 	CW_MSG_ELEMENT_WTP_STATIC_IP_CW_TYPE			49
/*Update 2009:
		Message type to return a payload together with the 
		configuration update response*/
#define 	CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE		57
#define 	CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE_WITH_PAYLOAD		58
#define 	CW_MSG_ELEMENT_MTU_DISCOVERY_PADDING_CW_TYPE		59

//Vendor Specific data
#define 	CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_AC_PRIORITY		128
#define 	CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_AC_PRIORITY_LEN	1
#define 	CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_AC_PRIORITY_DEFAULT	3

// IEEE 802.11 Message Element
#define 	CW_MSG_ELEMENT_IEEE80211_ADD_WLAN_CW_TYPE						1024
#define 	CW_MSG_ELEMENT_IEEE80211_DELETE_WLAN_CW_TYPE					1027
#define 	CW_MSG_ELEMENT_IEEE80211_MULTI_DOMAIN_CAPABILITY_CW_TYPE		1032
#define 	CW_MSG_ELEMENT_IEEE80211_SUPPORTED_RATES_CW_TYPE				1040
#define 	CW_MSG_ELEMENT_IEEE80211_WTP_RADIO_INFORMATION_CW_TYPE			1048


#define CW_SET_FIELD32(src,start,len,val)					src |= ((~(0xFFFFFFFF << (len))) & (val)) << (32 - start - (len))
#define CW_GET_FIELD32(src,start,len)					((~(0xFFFFFFFF<<(len))) & (src >> (32 - start - (len))))


typedef struct {
	char *msg;
	int offset;
	int data; 
}cw_protocol_msg;

typedef struct {
	int vendor_id;
	enum {
		CW_WTP_MODEL_NUMBER	= 0,
		CW_WTP_SERIAL_NUMBER	= 1,
		CW_BOARD_ID		= 2,
		CW_BOARD_REVISION	= 3,

		CW_WTP_HARDWARE_VERSION	= 4,
		CW_WTP_SOFTWARE_VERSION	= 5,
		CW_BOOT_VERSION		= 6,
		CW_WTP_LABEL_MAC		= 7
	} type;
	int length;
	void *value;
} cw_wtp_vendor;

typedef struct  {
	int vendor_infos_count;
	cw_wtp_vendor *vendor_infos;
} cw_wtp_vendors;

typedef struct {
	enum {
		CW_MSG_ELEMENT_DISCOVERY_TYPE_BROADCAST = 0,
		CW_MSG_ELEMENT_DISCOVERY_TYPE_CONFIGURED = 1
	} type;
	cw_wtp_vendors wtp_board_data;
	//CWWTPDescriptor WTPDescriptor;
	//CWframeTunnelMode frameTunnelMode;
	//CWMACType MACType;

} CWDiscoveryRequestValues;

typedef struct {
	int payload_type;
	int type;
	int is_fragment;
	int last;
	int fragment_id;
	int fragment_offset;
	int keep_alive;
	cw_binding_transport_header *binding;
} cw_protocol_transport_header;

typedef struct {
	unsigned int msg_type;
	unsigned char seq_num;
	unsigned short msg_elems_len;
//	unsigned int timestamp;
} cw_control_header;

typedef struct {
    cw_protocol_msg* frame;
    cw_binding_transport_header* bindingValues;
} cw_binding_data_list_elem;

void cw_protocol_store8(cw_protocol_msg *msg, unsigned char val) ;
void cw_protocol_store16(cw_protocol_msg *msg, unsigned short val) ;
void cw_protocol_store32(cw_protocol_msg *msg, unsigned int val);
void cw_protocol_store_raw_bytes(cw_protocol_msg *msg, void *bytes, int len);
void cw_protocol_store_raw_bytes(cw_protocol_msg *msg, void *bytes, int len) ;
void cw_protocol_store_message(cw_protocol_msg *msg, cw_protocol_msg *msg_to_store);



#endif

