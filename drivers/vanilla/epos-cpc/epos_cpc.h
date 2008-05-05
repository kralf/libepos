#ifndef _EPOS_CPC_H
#define _EPOS_CPC_H

#include "cpclib.h"

void can_init(void);
void can_send_message(int can_id, char *msg);
void can_read_message(void);
void can_read_message_handler(int handle, const CPC_MSG_T *cpcmsg);

#endif
