/***************************************************
 * main.cpp
 * main program for Linux / MCM communication
 * basic program without interface
 **************************************************/


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include<string.h>
#include "cfile.h"
#include "exec.h"
#include "decode.h" 
#include "shell.h"      

int main()
{
  Connection connX;
  ComExec comeX;
  RxToShell rx_print;
  ShellInput inp;
  FileofFile *fofprocess;
  DataMonitoring *datmonprocess;
  FileData *filedataprocess;

   connX.Connect();
  //  connX.Connect("/dev/ttyUSB0");
  printf("Type Command -1 for the list of available commands\n");
  while (1) {
    inp.GetCmd();
    if (inp.CmdNum == 0) comeX.set_mcm_addr(inp.Argument);
 /*   else if (inp.CmdNum == 20) {printf("Exit Command\n"); break;}
    else if (inp.CmdNum == 21) {
      fofprocess = new FileofFile(inp.fileread, inp.filewrt, inp.delay, inp.Argument, connX.ftHandle);
      if (!fofprocess->Fof()) printf("Wrong Command File\n"); 
      delete fofprocess; 
    }
    else if (inp.CmdNum == 22) {
      datmonprocess = new DataMonitoring(inp.filewrt, inp.delay, inp.Argument, connX.ftHandle, inp.McmAddr, inp.iter);
      datmonprocess->DataMon();
      delete datmonprocess;
    }
    else if (inp.CmdNum == 23) {
      filedataprocess = new FileData(inp.fileread, inp.delay, connX.ftHandle);
      if (!filedataprocess->FData()) printf("Wrong Command File\n");
      delete filedataprocess;
    } */
    else if ((inp.CmdNum > 0) && (inp.CmdNum < 20)) {
      if (comeX.WriteBuffer(connX.ftHandle, inp) < 0)
        printf("Writting error\n");
   //   else if (rx_print.ReadBuffer(connX.ftHandle) < 1) 
     //      printf("Reading error\n");
      else rx_print.PrintRes();
    }
  }
  connX.Disconnect();

  return 1;
}

