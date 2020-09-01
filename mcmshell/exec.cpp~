/***************************************************
 * exec.cpp
 * cpp file for Linux / MCM communication
 * objects to prepare and send command packets to MCM
 **************************************************/


#include <stdio.h>    //perror, printf
#include <unistd.h>  

#include "cfile.h"
#include "exec.h"



/*-----------------------------------------------*/
/* Connection class functions                    */
/*-----------------------------------------------*/
Connection::Connection()
{
    ftStatus =-1;
}

void Connection::Connect()
{
          if((ftStatus = FT_OpenEx((void *)"FTU6Z938", FT_OPEN_BY_SERIAL_NUMBER, &ftHandle)) != FT_OK)
                  {
			/* 
				This can fail if the ftdi_sio driver is loaded
		 		use lsmod to check this and rmmod ftdi_sio to remove
				also rmmod usbserial
		 	*/
			printf("Error FT_OpenEx(%d)\n", ftStatus);
			//return ERROR;
		  }
                else
                   {
                      fprintf(stderr," Device opened successfully\n");
                   }   
	
            FT_Purge(ftHandle, FT_PURGE_RX | FT_PURGE_TX);  // clean the Tx and RX buffers...

}

void Connection::Disconnect()
{
   //close();
  //perror("CLose");
}


/*-----------------------------------------------*/
/* ComExec class functions                    */
/*-----------------------------------------------*/
ComExec::ComExec(void)
{
  buf_ptr0 = new char[100];
  McmAddr = 0;
}

ComExec::~ComExec(void)
{
  delete buf_ptr0;
}

/******* Add Check Sum function *********************
 * Add CS at the end of the buffer
 * CS does no include MCM Address                  */ 
void ComExec::AddCksum(char *buff)
{
  int length, i;
  char cs = 0;

  length = *(buff+1) + (*(buff+2)<<8);
  for(i=0; i<(length-1); i++) cs += *(buff+i+1);
  cs = ~cs+1;
  buff[length] = cs;
}

/******* Make buffer function ***********************
 * Prepare Cmd buffer ccording to Cmd Number
 * call sub functions for argument setting
 * set MCM address in the buffer
 * add CS                                          */ 
void ComExec::MakeBuffer(Command Cmd)
{
  switch(Cmd.CmdNum) {
    case 1 : {
      char buff[]={0,8,0,0,0,0,0,0,0};
      InitBuff(buff); break;
    }
    case 2 : {
      char buff[]={0,0xa,0,0,1,0,2,0,0,0,0};
      InitBuff(buff); break;
    }
    case 3 : {
      char buff[]={0,0xa,0,0,1,0,2,0,0,1,0};
      InitBuff(buff); break;
    }
    case 4 : set_mean_mode(Cmd.Argument);break;
    case 5 : set_anl_mask(Cmd.Argument);break;
    case 6 : set_dig_mask16(Cmd.Argument);break;
    case 7 : set_dig_mask32(Cmd.Argument);break;
    case 8 : set_dig_mask64(Cmd.Argument);break;
    case 9 : {
      char buff[]={0,9,0,0,2,0,1,0,0,0};
      InitBuff(buff); break;
    }
    case 10 : {
      char buff[]={0,9,0,0,2,0,1,0,1,0};
      InitBuff(buff); break;
    }
    case 11 : {
      char buff[]={0,9,0,0,2,0,1,0,2,0};
      InitBuff(buff); break;
    }
    case 12 : {
      char buff[]={0,9,0,0,2,0,1,0,5,0};
      InitBuff(buff); break;
    }
    case 13 : {
      char buff[]={0,9,0,0,2,0,1,0,3,0};
      InitBuff(buff); break;
    }
    case 14 : {
      char buff[]={0,9,0,0,2,0,1,0,4,0};
      InitBuff(buff); break;
    }
    case 15 : {
      char buff[]={0,8,0,0,5,0,0,0,0};
      InitBuff(buff); break;
    }
    case 16 : feed_cntrl_old(Cmd.Argument);break;
    case 17 : feed_cntrl_new(Cmd.Argument);break;
    case 18 : fe_box_mon(Cmd.Argument);break;
    case 19 : {
      char buff[]={0,9,0,0,7,0,1,0,1,0};
      InitBuff(buff); break;
    }
  } 
  *buf_ptr0 = McmAddr;
  AddCksum(buf_ptr0); 
}

/******* Init buffer function ***********************
 * Make buffer sub function
 * convenient for buffer implementation            */ 
void ComExec::InitBuff(char buff[])
{
  int i, length;
  length = buff[1] + (buff[2]<<8);
  for (i=0; i<length; i++) *(buf_ptr0+i) = buff[i];
}

/******* Write buffer function ***********************
 * Call Make buffer function
 * Write buffer to Serial Port                      */ 
int ComExec::WriteBuffer( FT_HANDLE ftHandle, Command Cmd)
{
  int bytes=0, length, i;
  DWORD dwBytesWritten = 0,dwBytesInQueue = 0 ;            
  MakeBuffer(Cmd);
  length = *(buf_ptr0+1) + (*(buf_ptr0+2)<<8);
  printf("TX : Cmd %d\n", Cmd.CmdNum);
  for (i=0; i<length; i++) printf("%.2x\t", (unsigned char) *(buf_ptr0+i));
  printf("\n");
 


                ftStatus = FT_SetRts(ftHandle);
                if (ftStatus == FT_OK) 
                       {
                         // FT_SetRts OK
                          fprintf(stderr,"FT_SetRTS(%d)\n", ftStatus);
                       }
                  else 
                      {
                                // FT_SetRts failed
                                 fprintf(stderr,"ERROR FT_SetRTS(%d)\n", ftStatus);
                       }
       
		ftStatus = FT_SetBaudRate(ftHandle, 9600); 
                if (ftStatus == FT_OK) 
                   {
                     // FT_SetBaudRate OK
                     fprintf(stderr,"FT_SetBaudRate(%d)\n", ftStatus);
                   }
                   else 
                   {
                        // FT_SetBaudRate Failed
                        fprintf(stderr,"Error FT_SetBaudRate(%d)\n", ftStatus);

                   }
       
                 // Set 8 data bits, 1 stop bit and no parity
                ftStatus = FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1,FT_PARITY_MARK);
                if (ftStatus == FT_OK) 
                    {
                      // FT_SetDataCharacteristics OK
                      fprintf(stderr,"SET FT_SetDataCharacteristics(%d)\n", ftStatus);
                    }
                     else 
                        {
                            // FT_SetDataCharacteristics Failed
                            fprintf(stderr,"Error FT_SetDataCharacteristics(%d)\n", ftStatus);

                         }
                                   
		/* Write */
		if((ftStatus = FT_Write(ftHandle, buf_ptr0, 1, &dwBytesWritten)) != FT_OK) {
			printf("Error FT_Write(%d)\n", ftStatus);
			}
          
                        usleep(10000);
                     dwBytesWritten = 0; 
                    ftStatus = FT_SetBaudRate(ftHandle, 9600);

                   ftStatus = FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1,FT_PARITY_SPACE);
                  if (ftStatus == FT_OK)
                    {
                      // FT_SetDataCharacteristics OK
                         fprintf(stderr,"SET FT_SetDataCharacteristics(%d)\n", ftStatus);
                    }
                    else {
                            // FT_SetDataCharacteristics Failed
                          fprintf(stderr,"Error FT_SetDataCharacteristics(%d)\n", ftStatus);
                      
                         }
                                             
                      if((ftStatus = FT_Write(ftHandle, (buf_ptr0+1),length, &dwBytesWritten)) != FT_OK) {
                        printf("Error FT_Write(%d)\n", ftStatus);
                       
                }
                             
             ftStatus = FT_ClrRts(ftHandle);
               if (ftStatus == FT_OK)
                 {
                         // FT_SetRts OK
                         fprintf(stderr,"FT_ClearRTS(%d)\n", ftStatus);
                 }
                     else {
                          // FT_SetRts failed
                         fprintf(stderr,"ERROR FT_ClearRTS(%d)\n", ftStatus);
                          }
                          //
	
     	usleep(10000); // Some delay is necessary because hardware can't write succesively...in a short period of time...

  return dwBytesWritten;
}

/******* Set MCM address function ********************
 * Set MCM Address in object parameter
 * this command doesn not sent any message          */ 
void ComExec::set_mcm_addr(unsigned char Arg[])
{
  McmAddr = Arg[0];
  *buf_ptr0 = McmAddr;
  printf("adr mcm set %x\n", McmAddr);
}

void ComExec::set_dig_mask64(unsigned char Arg[])
{
 int i;
 char buff[]={0,0x11,0,0,1,0,9,0,4,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0};

 for(i=0; i<8; i++) buff[9+i] = Arg[i];
 InitBuff(buff);
}

void ComExec::set_dig_mask32(unsigned char Arg[])
{
 int i;
 char buff[]={0,0xd,0,0,1,0,5,0,3,0xff,0xff,0xff,0xff,0};     

 for(i=0; i<4; i++) buff[9+i] = Arg[i];
 InitBuff(buff);
}

void ComExec::set_dig_mask16(unsigned char Arg[])
{
  int i;
  char buff[]={0,0xb,0,0,1,0,3,0,2,0xff,0xff,0};

  for(i=0; i<2; i++) buff[9+i] = Arg[i];
  InitBuff(buff);
}

void ComExec::set_anl_mask(unsigned char Arg[])
{
  int i;
  char buff[]={0,0x11,0,0,1,0,9,0,1,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0};

  for(i=0; i<8; i++)
   buff[9+i] = Arg[i];
  InitBuff(buff);
}

void ComExec::set_mean_mode(unsigned char Arg[])
{
 char buff[]={0,0xb,0,0,1,0,3,0,0,2,0,0};

 buff[10] = Arg[0];
 InitBuff(buff);
}


void ComExec::feed_cntrl_old(unsigned char Arg[])
{
  char buff[]={0,0xa,0,0,3,0,2,0,0,0,0};

  buff[8] = Arg[0]; buff[9] = Arg[0]>>8;//buff[9]=0;
  InitBuff(buff);
}

void ComExec::feed_cntrl_new(unsigned char Arg[])
{
  int i;
  char buff[]={0,0xa,0,0,6,0,2,0,0,0,0};

  for(i=0; i<2; i++) buff[8+i] = Arg[i];
  InitBuff(buff);
}

void ComExec::fe_box_mon(unsigned char Arg[])
{
  char buff[]={0,0xa,0,0,7,0,2,0,0,0,0};

  buff[9] = Arg[0]-1;
  InitBuff(buff);
}


