#include "common.hpp"

int MsgOper::Get(char *pcMessage, TLV &tlv)
{
    TLV *pTlv = (TLV *)(pcMessage);
    tlv.tl.iType   = pTlv->tl.iType;
    tlv.tl.uLength = pTlv->tl.uLength;
    tlv.pcValue    = (char *)(&pTlv->pcValue);
    return 0;
}

int MsgOper::Create(char *(&pcMsg), unsigned int &uMsgLen, TLV &tlv)
{
    int iType = tlv.tl.iType;
    unsigned int uLength = tlv.tl.uLength;

    char *pMsg = new char[MSG_HEADER_SIZE + uLength];
    memset(pMsg, 0 ,(MSG_HEADER_SIZE + uLength));
    TLV *pTLV = (TLV *)pMsg;
    pTLV->tl.iType   = iType;
    pTLV->tl.uLength = uLength;
    memcpy(&pTLV->pcValue, tlv.pcValue, uLength);
    pcMsg = pMsg;
    uMsgLen  = MSG_HEADER_SIZE + uLength;
}

void MsgOper::Destroy(char *pcMessage)
{
    delete pcMessage;
    pcMessage = NULL;
    
}

