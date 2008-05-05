#include <sys/types.h>
#include <sys/dir.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/file.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>

#include "epos_cpc.h"
#include "pdebug.h"

#undef ASL_DEBUG

/* Different macros for watching on read, write or both on a CAN channel. */
/* Also stdin is watched for a user input.                                */

#define SELECT    FD_ZERO(&readfds); \
                  FD_SET(0, &readfds); \
                  FD_SET(cpcfd, &readfds); \
                  tv.tv_sec = 0; \
                  tv.tv_usec = 500000; \
                  nfds = select(cpcfd+1, &readfds, NULL, NULL, &tv);

#define SELECT_WR FD_ZERO(&writefds); \
                  FD_SET(0, &readfds); \
                  FD_SET(cpcfd, &writefds); \
                  tv.tv_sec = 0; \
                  tv.tv_usec = 500000; \
                  nfds = select(cpcfd+1, NULL, &writefds, NULL, &tv);

unsigned char btr0, btr1;
int handle;
char interface[32];
int zchn, nfds, cpcfd;
CPC_INIT_PARAMS_T *CPCInitParamsPtr;
fd_set readfds, writefds;
struct timeval tv;

/* Init CAN Hardware */
void can_init() {
  /* ... add your code here ... */
  btr0=0x00;
  btr1=0x14;

  /* Using Interface cpc_usb0 */
  strcpy(interface, "/dev/usb/cpc_usb0");
  // strcpy(interface, "/dev/cpc_usb0");

  /*Open the CAN*/
  if ((handle = CPC_OpenChannel(interface)) < 0) {
      fprintf(stderr, "ERROR: %s\n", CPC_DecodeErrorMsg(handle));
      exit(1);
  }
  PDEBUG("%s is CAN interface -> handle %d\n", interface, handle);

  /* ############################# Init Parameters ######################*/

  /*Define Handlers*/
  CPC_AddHandler(handle, can_read_message_handler);
  /*CPC_AddHandler(handle, get_steering_msg_handler);
  CPC_AddHandler(handle, get_yaw_msg_handler);*/


  /* This sets up the parameters used to initialize the CAN controller */
  PDEBUG("Initializing CAN-Controller ... ");

  CPCInitParamsPtr = CPC_GetInitParamsPtr(handle);
  CPCInitParamsPtr->canparams.cc_type                      = SJA1000;
  CPCInitParamsPtr->canparams.cc_params.sja1000.btr0       = btr0;
  CPCInitParamsPtr->canparams.cc_params.sja1000.btr1       = btr1;
  CPCInitParamsPtr->canparams.cc_params.sja1000.outp_contr = 0xda;
  CPCInitParamsPtr->canparams.cc_params.sja1000.acc_code0  = 0xff;
  CPCInitParamsPtr->canparams.cc_params.sja1000.acc_code1  = 0xff;
  CPCInitParamsPtr->canparams.cc_params.sja1000.acc_code2  = 0xff;
  CPCInitParamsPtr->canparams.cc_params.sja1000.acc_code3  = 0xff;
  CPCInitParamsPtr->canparams.cc_params.sja1000.acc_mask0  = 0xff;
  CPCInitParamsPtr->canparams.cc_params.sja1000.acc_mask1  = 0xff;
  CPCInitParamsPtr->canparams.cc_params.sja1000.acc_mask2  = 0xff;
  CPCInitParamsPtr->canparams.cc_params.sja1000.acc_mask3  = 0xff;
  CPCInitParamsPtr->canparams.cc_params.sja1000.mode       = 0;

  CPC_CANInit(handle, 0);
  PDEBUG("Done!\n\n");

  cpcfd = CPC_GetFdByHandle(handle);

  PDEBUG("Switch ON transimssion of CAN messages from CPC to PC\n");

  /* switch on transmission of CAN messages from CPC to PC */
  CPC_Control(handle, CONTR_CAN_Message | CONTR_CONT_ON);
  PDEBUG("InitHW finished...\n");
}

void can_send_message(int can_id, char *msg) {
  static CPC_CAN_MSG_T cmsg = {0x00L, 0, {0, 0, 0, 0, 0, 0, 0, 0}};
  int i;

  cmsg.id=can_id;                               //put the can id of the device in cmsg
  cmsg.length=8;                                //put the lenght of the message in cmsg
  for(i = 0; i < cmsg.length; i++) {
    cmsg.msg[i] = (unsigned char)msg[i];        //put the message in cmsg
  }

  SELECT_WR                                     //enable write en read
  if (FD_ISSET(cpcfd, &writefds)) {             //if chanel open and writable (???)
    CPC_SendMsg(handle, 0, &cmsg);              //send the message of lenght 8 to the device of id can_id
  }
}

void can_read_message() {
  /* task for reading can */
  SELECT                                        //enabel read (???)

  if (nfds > 0) {
  if (FD_ISSET(cpcfd, &readfds)) {              //check, if messages have been received
      do{;}                                     //wait
      while(CPC_Handle(handle));                //until OK is send
    }
  }
  else if(nfds == -1)                           //if error
    perror(interface);                          //print error
}
