#include <stdio.h>
#include <stdlib.h>
#include "ltpublic.h"
#include "ltmsg.h"


main()
{
    ltMsgHead *psMsgHead;
    psMsgHead = ltMsgInit(990118,102,123456,1024);
    ltMsgAdd_s(&psMsgHead,"aaaa","This is a Record");
    ltMsgAdd_l(&psMsgHead,"llll",1);
    ltMsgPrintMsg(psMsgHead);
    ltMsgHton(psMsgHead);
    printf(" lCOde====%d  Bytes==%d MsgId==%d \n",psMsgHead->lCode,
            psMsgHead->lBytes,psMsgHead->lMsgId);
    ltMsgNtoh(psMsgHead);
    psMsgHead->cOrder = 3;
    ltMsgPrintMsg(psMsgHead);
}

