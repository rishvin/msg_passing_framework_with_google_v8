#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

typedef Buffer<char *> MsgBuffer;
#define MSG_HEADER_SIZE (sizeof(TL))

typedef Buffer<char *> MsgBuffer;

class TL
{
    public:
    int iType;
    unsigned int uLength;
};
class TLV
{
    public:
    TL tl;
    char *pcValue;
};


class MsgOper
{
    public:
    static int Get(char *pcMessage, TLV &tlv);
    static int Create(char *(&pcMsg), unsigned int &uLen, TLV &tlv);
    static void Destroy(char *pcMessage);
};


#endif
