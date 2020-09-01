/***************************************************
 * decode.cpp
 * cpp file for Linux / MCM communication
 * objects to receive and decode packets from MCM
 **************************************************/


#include <unistd.h>    //read
#include <stdio.h>    //perror, sprintf
#include "cfile.h"
#include "decode.h"

/*-----------------------------------------------*/
/* CommandPlus class functions                   */
/*-----------------------------------------------*/

/******* Set Command Name ****************************
 * Set CmdName according to Command Number CmdNum   */
void CommandPlus::SetCmdName(void)
{
  switch (CmdNum) {
    case (0) : sprintf(CmdName, "Set Mcm Addr");break;
    case (1) : sprintf(CmdName, "Null Cmd");break;
    case (2) : sprintf(CmdName, "Set Idle Mode");break;
    case (3) : sprintf(CmdName, "Set Scan Mode");break;
    case (4) : sprintf(CmdName, "Set Mean Mode");break;
    case (5) : sprintf(CmdName, "Set Anl Mask");break;
    case (6) : sprintf(CmdName, "Set Dig Mask 16b");break;
    case (7) : sprintf(CmdName, "Set Dig Mask 32b");break;
    case (8) : sprintf(CmdName, "Set Dig Mask 64b");break;
    case (9) : sprintf(CmdName, "Read Anl Mask");break;
    case (10) : sprintf(CmdName, "Read Dig Mask 16b");break;
    case (11) : sprintf(CmdName, "Read Dig Mask 32b");break;
    case (12) : sprintf(CmdName, "Read Dig Mask 64b");break;
    case (13) : sprintf(CmdName, "Read Version");break;
    case (14) : sprintf(CmdName, "Read mode");break;
    case (15) : sprintf(CmdName, "Reboot");break;    
    case (16) : sprintf(CmdName, "FE Control (old)");break;
    case (17) : sprintf(CmdName, "FE Control (new)");break;
    case (18) : sprintf(CmdName, "FE-Box %d Monitor", FEbox);
    case (19) : sprintf(CmdName, "Common Box Monitor");break;
    case (20) : sprintf(CmdName, "Exit");break;
    case (21) : sprintf(CmdName, "File of File");break;
    case (22) : sprintf(CmdName, "Data Monitoring");break;
    case (23) : sprintf(CmdName, "File Data");
  }
}     

/*-----------------------------------------------*/
/* Decode class functions                        */
/*-----------------------------------------------*/

Decode::Decode(void)
{
  int i;
  
  buf_ptr0 = new char[100];
  Version = new char[10];
  Avgfact = new char[4];
  CmdStatus = new char[30];
  *Avgfact = 0;
  NbofChannels = 0;
  for (i=0;i<8;i++) anlmask[i]=0;
}

Decode::~Decode(void)
{
  delete buf_ptr0;
  delete Version;
  delete Avgfact;
  delete CmdStatus;     ////
}

/******* Read Buffer *********************************
 * Read buffer from serial port
 * Read total length, include MCM address           */
/*int Decode::ReadBuffer(ftHandle)
{
  int bytes=0, length=0, i;
  
  for (i=0;i<50;i++) *buf_ptr0 = 0;
  bytes = read(ftHandle, buf_ptr0, 100);
  //perror("WRite");
  if (bytes == -1) printf("format error (cs/siz), noise on rx.\n");
  else if (bytes == 0) printf("No answer, time-out.\n");
  else {
    printf("RX : %d bytes\n", bytes);
    if (bytes >3) length = *(buf_ptr0+1) + (*(buf_ptr0+2)<<8) + 1;
    printf("RX : %d bytes\n", length);
    for (i=0; i<length; i++) printf("%.2x\t", (unsigned char) *(buf_ptr0+i));
    printf("\n");
    //printf("cs : %.2x\n", *(buf_ptr0+length-1));
    DecodeBuffer();
  }
  return bytes;
}*/

/******* First Decode function ***********************
 * Decode MCM response main header
 * Call apropriate next decode functions
 * Set Command status and Command Name              */
void Decode::DecodeBuffer(void)
{
  McmAddr = *buf_ptr0;   
  switch (*(buf_ptr0+4)) {
    case (0) : sprintf(CmdStatus, "MCM Command OK"); break;
    case (1) : sprintf(CmdStatus, "MCM TimeOut"); break;
    case (2) : sprintf(CmdStatus, "Command Check Sum Error"); break;
    case (4) : sprintf(CmdStatus, "Command unknown"); break;
    case (8) : sprintf(CmdStatus, "Packet too large"); break;
    case (16) : sprintf(CmdStatus, "MCM Answer Message truncated"); break;
  }
  if (*(buf_ptr0+5)==1) {
    CmdRx.CmdNum = 1;
    DecodeFirstPacket();
  }
  else {
    DecodeFirstPacket();
    DecodeSecondPacket(buf_ptr0+6+*(buf_ptr0+6));
  }
  CmdRx.SetCmdName();
}

/******* Decode packet function ***********************
 * Decode MCM response first logical packet
 * Set MCM mode
 * Call RecordFirstPack                              */
void Decode::DecodeFirstPacket(void)
{
  int siz;
  
  siz = *(buf_ptr0+6) + (*(buf_ptr0+7)<<8);
  if (siz == 4) Mode = IDLE_MODE;
  else if (*(buf_ptr0+10) == 1) {
    Mode = SCAN_MODE;
    RecordFirstPack(buf_ptr0+11, siz-13);
  }
  else if (*(buf_ptr0+10) == 2) {
    Mode = MEAN_MODE;
    *Avgfact = *(buf_ptr0+11);
    RecordFirstPack(buf_ptr0+12, siz-14);
  }
  else if (*(buf_ptr0+10) == 3) {
    Mode = LIMITS_MODE;
    RecordFirstPack(buf_ptr0+11, siz-13);
  }
}

/******* RecordFirstPack function ***********************
 * Record 1st logical packet data in object variables  */
void Decode::RecordFirstPack(char *ptr, int recordsiz)
{
  int i;

  for (i=0;i<8;i++) anlmask[i] = *(ptr+i);
  for (i=0;i<recordsiz;i++) scandata[i] = *(ptr+i+8);
  NbofChannels = recordsiz;
}

/******* Decode packet function ***********************
 * Decode MCM response second logical packet
 * Call apropriate function or
 * set CmdNum & record data                          */
void Decode::DecodeSecondPacket(char *SecPackStart)
{
  int i, siz;

  siz = *SecPackStart + (*(SecPackStart+1)<<8);
  switch (*(SecPackStart+2)) {
    case (1) : SetProcess(SecPackStart); break;
    case (2) : ReadProcess(SecPackStart); break;
    case (3) :
      CmdRx.CmdNum = 16;
      for (i=0;i<2;i++) CmdRx.Argument[i] = *(SecPackStart+3+i);
      break;
    case (5) : CmdRx.CmdNum = 15; break;
    case (6) :
      CmdRx.CmdNum = 17;
      for (i=0;i<2;i++) CmdRx.Argument[i] = *(SecPackStart+3+i);
      break;
    case (7) : BoxProcess(SecPackStart, siz); break;
    case (8) : sprintf(CmdRx.CmdName, "Power Down"); break;
  }
}

/******* Set process function ***********************
 * Decode MCM response second logical packet
 * for Set commands                                */
void Decode::SetProcess(char* SecPackStart)
{
  int i;

  switch (*(SecPackStart+3)) {
    case (0) :
      if (*(SecPackStart+4) == 0) CmdRx.CmdNum = 2; 
      else if (*(SecPackStart+4) == 1) CmdRx.CmdNum = 3; 
      else if (*(SecPackStart+4) == 2) CmdRx.CmdNum = 4; 
      break;
    case (1) :
      CmdRx.CmdNum = 5; 
      for (i=0;i<8;i++) CmdRx.Argument[i] = *(SecPackStart+4+i);
      break;
    case (2) :
      CmdRx.CmdNum = 6; 
      for (i=0;i<2;i++) CmdRx.Argument[i] = *(SecPackStart+4+i);
      break;
    case (3) :
      CmdRx.CmdNum = 7; 
      for (i=0;i<4;i++) CmdRx.Argument[i] = *(SecPackStart+4+i);
      break;
    case (4) :
      CmdRx.CmdNum = 8;
      for (i=0;i<8;i++) CmdRx.Argument[i] = *(SecPackStart+4+i);
      break;
  }
}

/******* Read process function ***********************
 * Decode MCM response second logical packet
 * for Read commands                                */
void Decode::ReadProcess(char* SecPackStart)
{
  int i;
  char tmp1, tmp2;

  switch (*(SecPackStart+3)) {
    case (0) :
      CmdRx.CmdNum = 9;
      for (i=0;i<8;i++) CmdRx.Argument[i] = *(SecPackStart+4+i);
      // redondant with RecordFirstPack() except in Idle mode
      break;
    case (1) :
      CmdRx.CmdNum = 10;
      for (i=0;i<2;i++) CmdRx.Argument[i] = *(SecPackStart+4+i);
      break;
    case (2) :
      CmdRx.CmdNum = 11;
      for (i=0;i<4;i++) CmdRx.Argument[i] = *(SecPackStart+4+i);
      break;
    case (3) :
      CmdRx.CmdNum = 13;
      tmp1 = (*(SecPackStart+4) & 0xf) + 0x30;
      tmp2 = ((*(SecPackStart+4)>>4) & 0xf) + 0x30;
      sprintf(Version, "%c.%c", tmp2, tmp1);
      break;
    case (4) :
      CmdRx.CmdNum = 14;
      if (*(SecPackStart+4) == 0) Mode = IDLE_MODE;
      else if (*(SecPackStart+4) == 1) Mode = SCAN_MODE;
      else if (*(SecPackStart+4) == 2) Mode = MEAN_MODE;
      else Mode = LIMITS_MODE;
      break;
    case (5) :
      CmdRx.CmdNum = 12;
      for (i=0;i<8;i++) CmdRx.Argument[i] = *(SecPackStart+4+i);
      break;
  }
}

/******* Box process function ***********************
 * Decode MCM response second logical packet
 * for Box commands                                */
void Decode::BoxProcess(char *SecPackStart, int siz)
{
  int i;

  if (*(SecPackStart+3)) {
    CmdRx.CmdNum = 19;
    NbofChannels = siz-4;
    for (i=0;i<NbofChannels;i++) CmdRx.Argument[i] = *(SecPackStart+4+i);
  }
  else {
    CmdRx.CmdNum = 18;
    CmdRx.FEbox = *(SecPackStart+4);
    NbofChannels = siz-5;
    for (i=0;i<NbofChannels;i++) CmdRx.Argument[i] = *(SecPackStart+5+i);
  }
}

