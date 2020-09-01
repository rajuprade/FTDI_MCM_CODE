/*******************************************************
 * cfile.cpp
 * cpp file for Linux / MCM communication
 * common constant definitions
 * basic objects and objects for file based commmands
 ******************************************************/

#include <stdio.h>
#include <unistd.h>  //sleep
#include <stdlib.h>  //strtol
#include <time.h>
#include <string.h>

#include "cfile.h"
#include "decode.h"
#include "exec.h"


/*-----------------------------------------------*/
/* CommandList class functions                   */
/*-----------------------------------------------*/
CommandList::CommandList(void)
{
  NextCmd = 0;
}

CommandList::~CommandList(void)
{
  if (NextCmd) delete NextCmd;
}

/*-----------------------------------------------*/
/* Filebase class functions                      */
/*-----------------------------------------------*/
Filebase::Filebase(char *filename)
{
  fd = fopen(filename, "a+");
}

Filebase::~Filebase(void)
{
  fclose(fd);
}

/******* function for 'File Of File' Command **************
 * read command inputs from fd
 * call Convert
 * prepare corresponding CommandList                     */
int Filebase::ReadCmdParam1(CommandList **CurrentCmdptr)
{
  CommandList *FirstCmd;
  char param[6];
  time_t t1, t2;

  FirstCmd = *CurrentCmdptr;
  fgets(param, 6, fd);
  if (param[0] != ':') return 0; 
  time(&t1);
  while (param[0] != 'q') {
    if (param[0]==':') {
      (*CurrentCmdptr)->CmdNum = 0;    //set mcm addr
      (*CurrentCmdptr)->Argument[0] = strtol(&param[1], NULL, 16);
      (*CurrentCmdptr)->NextCmd = new CommandList;
      *CurrentCmdptr = (*CurrentCmdptr)->NextCmd;
      fgets(param, 6, fd);
    }          
    (*CurrentCmdptr)->CmdNum = 6;     //set dig mask 16bits
    Convert(0, &((*CurrentCmdptr)->Argument[0]), &((*CurrentCmdptr)->Argument[1]), param);
    //printf(" %x %x\n", (*CurrentCmdptr)->Argument[0], (*CurrentCmdptr)->Argument[1]);
    (*CurrentCmdptr)->NextCmd = new CommandList;
    *CurrentCmdptr = (*CurrentCmdptr)->NextCmd;
    fgets(param, 6, fd);
    time(&t2);
    if ((t2-t1)>2) return 0;
  }
  *CurrentCmdptr =  FirstCmd;
  return 1;
}

/******* function for 'File Of File' and 'File Data' Commands **********
 * convert file line into 2 char numbers
 * for 2 parameters, starting from offset inside the string            */
void Filebase::Convert(int offset, unsigned char *conv1, unsigned char *conv2, char line[21])
{
  int j;
  unsigned char tab1[3], tab2[3];

  for (j=0;j<2;j++) {tab1[j]=line[j+offset];tab2[j]=line[j+offset+2];}
  tab1[2] = tab2[2] = '\n';       //strtol() converts until '\n'
  *conv2 = strtol((const char *)tab1, NULL, 16);   //3412 order in previous mcmprn prog
  *conv1 = strtol((const char *)tab2, NULL, 16);
}

/******* function for 'File Of File' Command **************
 * write header in fd with passed parameters              */
void Filebase::WriteHeader1(int delay, unsigned char *AnlMask)
{
  time_t tim;
  int i;

  time(&tim);
  fprintf(fd, "\n-------------------------- File of File MCM command -------------------------\n");
  fprintf(fd, "%sdelay %d\nAnalog Mask:", ctime(&tim), delay);
  for (i=0;i<8;i++) fprintf(fd, " %.2x", (unsigned char)AnlMask[i]);
  fprintf(fd, "\n\nDig Mask\t MCM Addr\t Channels values (in decimal)\n");
}

/******* function for 'File Of File' Command **************
 * write passed parameters in fd as measured values      */
void Filebase::WriteVal1(unsigned char digmask[2], char Mcmaddr, unsigned char Arg[], int NbofChannels)
{
  int i;

  fprintf(fd, "%.2x %.2x\t\t %.2x\t\t", (unsigned char)digmask[0], (unsigned char)digmask[1], Mcmaddr);
  for (i=0;i<NbofChannels;i++) fprintf(fd, "%.2d ", Arg[i]);
  fprintf(fd, "\n");
}

/******* function for 'Data Monitoring' Command ************
 * write header in fd with passed parameters              */
void Filebase::WriteHeader2(int delay, unsigned char *AnlMask, char McmAddr)
{
  time_t tim;
  int i;

  time(&tim);
  fprintf(fd, "\n--------------------- Data Monitoring MCM command --------------------\n");
  fprintf(fd, "%sdelay %d\nMCM Address %.2x\nAnalog Mask:", ctime(&tim), delay, McmAddr);
  for (i=0;i<8;i++) fprintf(fd, " %.2x", (unsigned char)AnlMask[i]);
  fprintf(fd, "\n\nChannels values (Volts):\n\n");
}

/******* function for 'Data Monitoring' Command ***********
 * write passed parameters in fd as measured values      */
void Filebase::WriteVal2(unsigned char Arg[], int NbofChannels)
{
  int i;
  float j;

  for (i=0;i<NbofChannels;i++) {
      j = -5 +0.001*(float)Arg[i]*39.0625;
      fprintf(fd, "%.3f ", j);
      //fprintf(fd, "%.2d ", Arg[i]);
  }
  fprintf(fd, "\n\n");
}

/******* function for 'File Data' Command **************
 * read command inputs from fd
 * prepare corresponding CommandList                     */
int Filebase::ReadCmdParam2(CommandList **CurrentCmdptr)
{
  CommandList *FirstCmd; 
  char param[150];
  time_t t1, t2;

  FirstCmd = *CurrentCmdptr;
  do fgets(param, 150, fd);
  while (param[0] == '*');
  if (param[0] != ':') return 0; 
  time(&t1);
  
  while (param[0] != 'q') {
    if (param[0]==':') {
      (*CurrentCmdptr)->CmdNum = 0;    //set mcm addr
      (*CurrentCmdptr)->Argument[0] = strtol(&param[1], NULL, 16);
    }
    else if (strlen(param)==5) {
      (*CurrentCmdptr)->CmdNum = 6;     //set dig mask 16bits
      Convert(0, &((*CurrentCmdptr)->Argument[0]), &((*CurrentCmdptr)->Argument[1]), param);
    }
    else if (strlen(param)==10) {
      (*CurrentCmdptr)->CmdNum = 7;     //set dig mask 32bits
      Convert(0, &((*CurrentCmdptr)->Argument[0]), &((*CurrentCmdptr)->Argument[1]), param);
      Convert(5, &((*CurrentCmdptr)->Argument[2]), &((*CurrentCmdptr)->Argument[3]), param);
    }
    else if (strlen(param)==20) {
      (*CurrentCmdptr)->CmdNum = 8;     //set dig mask 64bits
      Convert(0, &((*CurrentCmdptr)->Argument[0]), &((*CurrentCmdptr)->Argument[1]), param);
      Convert(5, &((*CurrentCmdptr)->Argument[2]), &((*CurrentCmdptr)->Argument[3]), param);
      Convert(10, &((*CurrentCmdptr)->Argument[4]), &((*CurrentCmdptr)->Argument[5]), param);
      Convert(15, &((*CurrentCmdptr)->Argument[6]), &((*CurrentCmdptr)->Argument[7]), param);
    }
    else {return 0;}
    (*CurrentCmdptr)->NextCmd = new CommandList; 
    *CurrentCmdptr = (*CurrentCmdptr)->NextCmd;
    fgets(param, 21, fd);
    time(&t2); if ((t2-t1)>2) return 0;
  }
  *CurrentCmdptr = FirstCmd;
  return 1;
}

