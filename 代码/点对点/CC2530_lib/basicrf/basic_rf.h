//-------------------------------------------------------------------
// Filename: basic_rf.h
// Description:  Basic RF library header file
//-------------------------------------------------------------------
//    The "Basic RF" library contains simple functions for packet transmission and
//    reception with the IEEE 802.15.4 compliant radio devices. The intention of this
//    library is to demonstrate how the radio devices are operated, and not to provide
//    a complete and fully-functional packet protocol. The protocol uses 802.15.4 MAC compliant data
//    and acknowledgment packets, however it contains only a small subset of  the
//    802.15.4 standard:
//    - Association, scanning nor beacons are not implemented
//    - No defined coordinator/device roles (peer-to-peer, all nodes are equal)
//    - Waits for the channel to become ready, but does not check CCA twice
//    (802.15.4 CSMA-CA)
//    - Does not retransmit packets
//
//    INSTRUCTIONS:
//    Startup:
//    1. Create a basicRfCfg_t structure, and initialize the members:
//    2. Call basicRfInit() to initialize the packet protocol.
//
//    Transmission:
//    1. Create a buffer with the payload to send
//    2. Call basicRfSendPacket()
//
//    Reception:
//    1. Check if a packet is ready to be received by highger layer with
//    basicRfPacketIsReady()
//    2. Call basicRfReceive() to receive the packet by higher layer
//
//    FRAME FORMATS:
//    Data packets (without security):
//    [Preambles (4)][SFD (1)][Length (1)][Frame control field (2)]
//    [Sequence number (1)][PAN ID (2)][Dest. address (2)][Source address (2)]
//    [Payload (Length - 2+1+2+2+2)][Frame check sequence (2)]
//
//    Acknowledgment packets:
//    [Preambles (4)][SFD (1)][Length = 5 (1)][Frame control field (2)]
//    [Sequence number (1)][Frame check sequence (2)]
//-------------------------------------------------------------------
#ifndef _BASIC_RF_H_
#define _BASIC_RF_H_

//-------------------------------------------------------------------
// TYPEDEFS
//-------------------------------------------------------------------
typedef struct
{
    unsigned short myAddr;
    unsigned short panId;
    unsigned char channel;
    unsigned char ackRequest;
    #ifdef SECURITY_CCM
        unsigned char *securityKey;
        unsigned char *securityNonce;
    #endif 
} basicRfCfg_t;

//-------------------------------------------------------------------
// GLOBAL FUNCTIONS
//-------------------------------------------------------------------
unsigned char basicRfInit(basicRfCfg_t *pRfConfig);
unsigned char basicRfSendPacket( unsigned short destAddr, unsigned char *pPayload, unsigned char length);
unsigned char basicRfPacketIsReady(void);
char basicRfGetRssi(void);
unsigned char basicRfReceive(unsigned char *pRxData,  unsigned short len,  short *pRssi);
void basicRfReceiveOn(void);
void basicRfReceiveOff(void);

#endif /*_BASIC_RF_H_*/
