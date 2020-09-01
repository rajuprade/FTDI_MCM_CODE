/***************************************************
 * shell.cpp
 * cpp file for Linux / MCM communication
 * shell terminal specific classes
 **************************************************/

#include <stdio.h>
#include <unistd.h>  //sleep
#include <stdlib.h>  //strtol
#include <time.h>
#include "cfile.h"
#include "decode.h"
#include "exec.h"
#include "shell.h"
/*-----------------------------------------------*/
/* ShellInput class functions                    */
/*-----------------------------------------------*/

/******* Set Cmd Arguments function *********************
 * Ask arguments corresponding to the selected command
 * Read inputs                                         */
void ShellInput::SetCmdArgument(void)
{
  int i;

  switch (CmdNum) {
    case (0) :
      printf("\nEnter Mcm Addr (dec):\t");
      scanf("%d", (int *)&Argument[0]);
      break;
    case (5) :
      printf("\nEnter Anl Mask (xx xx xx xx xx xx xx xx):\t");
      for (i=0;i<8;i++) scanf("%x", (int *)&Argument[i]);
      break;
    case (6) :
      printf("\nEnter Dig Mask 16 (xx xx):\t");
      for (i=0;i<2;i++) scanf("%x", (int *)&Argument[i]);
      break;
    case (7) :
      printf("\nEnter Dig Mask 32 (xx xx xx xx):\t");
      for (i=0;i<4;i++) scanf("%x", (int *)&Argument[i]);
      break;
    case (8) :
      printf("\nEnter Dig Mask 64 (xx xx xx xx xx xx xx xx):\t");
      for (i=0;i<8;i++) scanf("%x", (int *)&Argument[i]);
      break;
    case (4) :
      printf("\nEnter Avg Factor:\t");
      scanf("%d", (int *)&Argument[0]);
      break;
    case (16) :
      printf("\nEnter Feed data mask (xx):\t");
      scanf("%x", (int *)&Argument[0]);
      break;
    case (17) :
      printf("\nEnter Feed data mask(xx xx):\t");
      for (i=0;i<2;i++) scanf("%x", (int *)&Argument[i]);
      break;
    case (18) :
      printf("\nEnter Feed box number:\t");
      scanf("%d", (int *)&Argument[0]); break;
    case (21) :
      Fofinputs(); break;
    case (22) :
      Datmoninputs(); break;
    case (23) :
      Filedatinputs(); 
             
  }
}

/******* Get Command function *********************
 * Command selection
 * display available commands if wrong entry
 * call SetCmdArgument                           */
void ShellInput::GetCmd(void)
{
  printf("\nEnter Command:\t");
  //fflush(stdout);
  scanf("%d", &CmdNum);
  if ((CmdNum < 0) || (CmdNum > 23)) {
    printf("\n0:Set Mcm Addr\t\t1:Null Cmd\t\t2:Set Idle Mode\t\t3:Set Scan Mode\n");
    printf("4:Set Mean Mode\t\t5:Set Anl Mask\t\t6:Set Dig Mask 16b\t7:Set Dig Mask 32b\n");
    printf("8:Set Dig Mask 64b\t9:Read Anl Mask\t\t10:Read Dig Mask 16b\t11:Read Dig Mask 32\n");
    printf("12:Read Dig Mask 64b\t13:Read Version\t\t14:Read mode\t\t15:Reboot\n");
    printf("16:FE Control (old)\t17:FE Control (new)\t18:FE-Box Monitor\t19:Common Box Monitor\n");
    printf("20:Exit\t\t\t21:File of File\t\t22:Data Monitoring\t23:File Data\n");
  }
  else SetCmdArgument();
}

void ShellInput::Fofinputs(void)
{
  int i;

  printf("\nEnter File to be read:\t");
  scanf("%s", fileread);
  printf("\nEnter File to be written:\t");
  scanf("%s", filewrt);
  printf("\nEnter Delay (option, ms):\t");
  scanf("%d", &delay);
  printf("\nEnter Anl Mask (xx xx xx xx xx xx xx xx):\t");
  for (i=0;i<8;i++) scanf("%x", (int *)&Argument[i]);
}

void ShellInput::Filedatinputs(void)
{
  printf("\nEnter File to be read:\t");
  scanf("%s", fileread);
  printf("\nEnter Delay (ms):\t");
  scanf("%d", &delay);
}

void ShellInput::Datmoninputs(void)
{
  int i;

  printf("\nEnter File to be written:\t");
  scanf("%s", filewrt);
  printf("\nEnter Delay (ms):\t");
  scanf("%d", &delay);
  printf("\nEnter MCM Address (dec):\t");
  scanf("%d", (int *)&McmAddr);
  printf("\nEnter Anl Mask (xx xx xx xx xx xx xx xx):\t");
  for (i=0;i<8;i++) scanf("%x", (int *)&Argument[i]);
  printf("\nEnter nb of iterations:\t");
  scanf("%d", &iter);
}


/*-----------------------------------------------*/
/* RxToShell class functions                     */
/*-----------------------------------------------*/

/******* Print MCM Response function **********************
 * print relevant MCM response data in the Terminal Shell */
void RxToShell::PrintRes(void)
{
  int i;

  printf("\nMCM Address : %d\n", McmAddr);
  printf("Command Received : %s\n", CmdRx.CmdName);
  printf("Command Status : %s\n", CmdStatus);

  switch (Mode) {
    case IDLE_MODE : printf("Mode : Idle Mode\n"); break;
    case SCAN_MODE : printf("Mode : Scan Mode\n"); break;
    case MEAN_MODE : printf("Mode : Mean Mode\n"); break;
    case LIMITS_MODE : printf("Mode : Limits Mode\n");
  }

  if (Mode != IDLE_MODE) {
    printf("Anl Mask :\t");
    for (i=0;i<8;i++) printf("%x\t", anlmask[i]);
    printf("\n");
    printf("Scan data :\t");
    for (i=0;i<NbofChannels;i++) printf("%d\t", scandata[i]);
    printf("\n");
  }
  if (Mode == MEAN_MODE) printf("averaging factor : %d\n", (int)*Avgfact);
  if (CmdRx.CmdNum == 13) printf("Version : %s\n", Version);
  else if ((CmdRx.CmdNum == 6) || (CmdRx.CmdNum == 10) || (CmdRx.CmdNum == 17)) {
    printf("Digital Mask :\t");
    for (i=0;i<2;i++) printf("%x\t", CmdRx.Argument[i]);
    printf("\n");
  }
  else if (CmdRx.CmdNum == 16) printf("Feed Mask : %x\n", CmdRx.Argument[0]);
  else if ((CmdRx.CmdNum == 7) || (CmdRx.CmdNum == 11)) {
    printf("Digital Mask :\t");
    for (i=0;i<4;i++) printf("%x\t", CmdRx.Argument[i]);
    printf("\n");
  }
  else if ((CmdRx.CmdNum == 8) || (CmdRx.CmdNum == 12)) {
    printf("Digital Mask :\t");
    for (i=0;i<8;i++) printf("%x\t", CmdRx.Argument[i]);
    printf("\n");
  }
  else if ((CmdRx.CmdNum == 18) || (CmdRx.CmdNum == 19)) {
    printf("FE scan data :\t");
    for (i=0;i<NbofChannels;i++) printf("%x\t", CmdRx.Argument[i]);
    printf("\n");
  }
}


/*-----------------------------------------------*/
/* FileofFile class functions for shell prgrm    */
/*-----------------------------------------------*/

FileofFile::FileofFile(char *filerd, char *filewrt, int delay0, unsigned char AnlMask0[8], FT_HANDLE ftHandle)
{
  int i;
  
  filecmd = new Filebase(filerd);
  fileres = new Filebase(filewrt);
  delay = delay0;
  fileres->WriteHeader1(delay, AnlMask0);
  for (i=0;i<8;i++) AnlMask[i] = AnlMask0[i];
}

FileofFile::~FileofFile(void)
{
  delete filecmd;
  delete fileres;
}

/******* OneTxRx function ******************************
 * execute 1 Command (WR, RD), wait delay (s)       
void FileofFile::OneTxRx(Command Cmd)
{
  Fofcom.WriteBuffer(ftHandle, Cmd);
  Fofdec.ReadBuffer(ftHandle);
  usleep(delay*1000);
}       
*/
/******* File Of File function************************
 * execute the File Of File set of basic commands   */
int FileofFile::Fof(void)
{
  int j;
  CommandList *FirstCmd, *CurrentCmd;
  Command NewCmd;

  FirstCmd = new CommandList;
  CurrentCmd = FirstCmd;
  if (!filecmd->ReadCmdParam1(&CurrentCmd)) return 0;
  CurrentCmd = FirstCmd;
  do {
    if (CurrentCmd->CmdNum == 0) {
      Fofcom.set_mcm_addr(CurrentCmd->Argument);    //set mcm addr
      CurrentCmd = CurrentCmd->NextCmd;
      NewCmd.CmdNum = 5;                            //set Anl mask
      for (j=0;j<8;j++) NewCmd.Argument[j] = AnlMask[j];
      //OneTxRx(NewCmd);
      NewCmd.CmdNum = 3;                            //set Scan Mode
     // OneTxRx(NewCmd); 
    }
   // OneTxRx(*CurrentCmd);                           //set dig mask 16b
    NewCmd.CmdNum = 1;                              //null Cmd
 //   OneTxRx(NewCmd);
    fileres->WriteVal1(CurrentCmd->Argument, Fofdec.McmAddr, Fofdec.scandata, Fofdec.NbofChannels);
    CurrentCmd = CurrentCmd->NextCmd;
  }
  while (CurrentCmd->NextCmd != 0);
  delete FirstCmd;   
  return 1;
}

/*-----------------------------------------------*/
/* DataMonitoring class functions for shell pgrm */
/*-----------------------------------------------*/

DataMonitoring::DataMonitoring(char *filewrt, int delay0, unsigned char AnlMask[8], FT_HANDLE	ftHandle, unsigned char McmAddr, int iter0)
{
  int j;
  Command NewCmd;
  delay = delay0;
  DataMoncom.set_mcm_addr(&McmAddr);    //set mcm addr
  NewCmd.CmdNum = 15;                   //Reboot, to avoid truncating
  //OneTxRx(NewCmd);
  NewCmd.CmdNum = 5;                            //set Anl mask
  for (j=0;j<8;j++) NewCmd.Argument[j] = AnlMask[j];
 // OneTxRx(NewCmd);
  NewCmd.CmdNum = 3;                            //set Scan Mode
  //OneTxRx(NewCmd);

  iter = iter0;
  fileres = new Filebase(filewrt);
  fileres->WriteHeader2(delay, AnlMask, McmAddr);
}

DataMonitoring::~DataMonitoring(void)
{
  delete fileres;
}

/******* OneTxRx function ******************************
 * execute 1 Command (WR, RD), wait delay (s)         
void DataMonitoring::OneTxRx(Command Cmd)
{
  DataMoncom.WriteBuffer(ftHandle, Cmd);
  DataMondec.ReadBuffer(ftHandle);
  usleep(delay*1000);
} */

/******* Data Monitoring function************************
 * execute the Data Monitoring set of basic commands   */
void DataMonitoring::DataMon(void)
{
  int j;
  Command NewCmd;

  for (j=0;j<iter;j++) {
    NewCmd.CmdNum = 1;                              //null Cmd
   // OneTxRx(NewCmd);
    fileres->WriteVal2(DataMondec.scandata, DataMondec.NbofChannels);
  }
}


/*-----------------------------------------------*/
/* FileData class functions for shell prgrm    */
/*-----------------------------------------------*/

FileData::FileData(char *filerd, int delay0,FT_HANDLE	ftHandle)
{
   filecmd = new Filebase(filerd);
  delay = delay0;      
 
}

FileData::~FileData(void)
{
  delete filecmd;
}

/******* OneTxRx function ******************************
 * execute 1 Command (WR, RD), wait delay (s)        
void FileData::OneTxRx(Command Cmd)
{
  FDatcom.WriteBuffer(ftHandle, Cmd);
  FDatdec.ReadBuffer(ftHandle);
  usleep(delay*1000);
}*/

/******* File Data function************************
 * execute the File Data set of basic commands   */
int FileData::FData(void)
{
  CommandList *FirstCmd, *CurrentCmd;

  FirstCmd = new CommandList;
  CurrentCmd = FirstCmd;
  if (!filecmd->ReadCmdParam2(&CurrentCmd)) return 0;
  CurrentCmd = FirstCmd;
  do {
    if (CurrentCmd->CmdNum == 0) {
      FDatcom.set_mcm_addr(CurrentCmd->Argument);    //set mcm addr
      CurrentCmd = CurrentCmd->NextCmd;
    }
   // OneTxRx(*CurrentCmd);      //set dig mask 16b/32b/64b
    CurrentCmd = CurrentCmd->NextCmd;
  }  
  while (CurrentCmd->NextCmd != 0);
  delete FirstCmd;
  return 1;
}



