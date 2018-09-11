#include "jrtplib3/rtpsession.h"
#include "jrtplib3/rtpsessionparams.h"
#include "jrtplib3/rtpudpv4transmitter.h"
#include "jrtplib3/rtpipv4address.h"
#include "jrtplib3/rtptimeutilities.h"
#include "jrtplib3/rtppacket.h"
#include "stdlib.h"

#include "transfer.h"
#include "debug.h"
using namespace jrtplib;

NALU_HEADER *nalu_hdr;
FU_INDICATOR *fu_ind;
FU_HEADER *fu_hdr;
char sendbuf[1500];
char *nalu_payload;
NALU_t *n;

RTPSession session;
RTPSessionParams sessionparams;
RTPUDPv4TransmissionParams transparams;

/**
 * @brief Get the startcodeprefix len object for 4. such as 00 00 00 01.
 * 
 * @param nalu 
 * @return int 
 */
static int get_startcodeprefix_len(uint8_t *nalu)
{
    if(nalu[2]==1)return 3;
    return 4;
}

void close_transfer()
{
    session.BYEDestroy(RTPTime(10.0), "Time's up", 9);
}

int init_param_for_jrtplib(uint8_t *dst_ip, uint16_t local_port, uint16_t remote_port, int fps)
{
    int ret = 0;
    sessionparams.SetOwnTimestampUnit(1.0 / 900);
    transparams.SetPortbase(local_port);

    ret = session.Create(sessionparams, &transparams);
    RTPIPv4Address addr(dst_ip, remote_port);
    ret = session.AddDestination(addr);
    session.SetDefaultPayloadType(96);
    session.SetDefaultMark(false);
    session.SetDefaultTimestampIncrement(900/fps);
    n = (NALU_t *)calloc(1, sizeof(NALU_t));
    return ret;
}

int transfer_one_nalu_by_rtp(uint8_t *nalu, int len, uint32_t inc)
{
    int ret;
    n->startcodeprefix_len = get_startcodeprefix_len(nalu);
    n->buf = nalu + n->startcodeprefix_len;
    n->forbidden_bit = n->buf[0] & 0x80;
    n->nal_reference_idc = n->buf[0] & 0x60;
    n->nal_unit_type = (n->buf[0]) & 0x1f;
    n->len = len - n->startcodeprefix_len;
    debug_print("nalu type = %u.",n->nal_unit_type);
    if (n->len <= MAX_RTP_PKT_LENGTH)
    {
        nalu_hdr = (NALU_HEADER *)&sendbuf[0];
        nalu_hdr->F = n->forbidden_bit;
        nalu_hdr->NRI = n->nal_reference_idc >> 5;
        nalu_hdr->TYPE = n->nal_unit_type;

        nalu_payload = &sendbuf[1];
        memcpy(nalu_payload, n->buf + 1, n->len - 1);

        if (n->nal_unit_type == 1 || n->nal_unit_type == 5)
        {
            ret = session.SendPacket((void *)sendbuf, n->len, 96, true, inc);
        }
        else
        {
            ret = session.SendPacket((void *)sendbuf, n->len, 96, true, 0);
        }
    }
    else if (n->len > MAX_RTP_PKT_LENGTH)
    {
        int k = 0, l = 0, t = 0;
        k = n->len / MAX_RTP_PKT_LENGTH;
        l = n->len % MAX_RTP_PKT_LENGTH;
        while (t <= k)
        {
            if (!t)
            {
                memset(sendbuf, 0, 1500);
                fu_ind = (FU_INDICATOR *)&sendbuf[0];
                fu_ind->F = n->forbidden_bit;
                fu_ind->NRI = n->nal_reference_idc >> 5;
                fu_ind->TYPE = 28;
                fu_hdr = (FU_HEADER *)&sendbuf[1];
                fu_hdr->E = 0;
                fu_hdr->R = 0;
                fu_hdr->S = 1;
                fu_hdr->TYPE = n->nal_unit_type;

                nalu_payload = &sendbuf[2];
                memcpy(nalu_payload, n->buf + 1, MAX_RTP_PKT_LENGTH);
                ret = session.SendPacket((void *)sendbuf, MAX_RTP_PKT_LENGTH + 2, 96, false, 0);
                t++;
            }
            else if (k == t)
            {
                fu_ind->F = n->forbidden_bit;
                fu_ind->NRI = n->nal_reference_idc >> 5;
                fu_ind->TYPE = 28;
                fu_hdr = (FU_HEADER *)&sendbuf[1];
                fu_hdr->R = 0;
                fu_hdr->S = 0;
                fu_hdr->TYPE = n->nal_unit_type;
                fu_hdr->E = 1;
                nalu_payload = &sendbuf[2];
                memcpy(nalu_payload, n->buf + t * MAX_RTP_PKT_LENGTH + 1, l - 1);
                ret = session.SendPacket((void *)sendbuf, l + 1, 96, true, inc);
                t++;
            }
            else if (t < k && 0 != t)
            {
                fu_ind = (FU_INDICATOR *)&sendbuf[0];
                fu_ind->F = n->forbidden_bit;
                fu_ind->NRI = n->nal_reference_idc >> 5;
                fu_ind->TYPE = 28;
                fu_hdr = (FU_HEADER *)&sendbuf[1];
                fu_hdr->R = 0;
                fu_hdr->S = 0;
                fu_hdr->E = 0;
                fu_hdr->TYPE = n->nal_unit_type;

                nalu_payload = &sendbuf[2];
                memcpy(nalu_payload, n->buf + t * MAX_RTP_PKT_LENGTH + 1, MAX_RTP_PKT_LENGTH);
                ret = session.SendPacket((void *)sendbuf, MAX_RTP_PKT_LENGTH + 2, 96, false, 0);
                t++;
            }
        }
    }
    return ret;
}
