/******************************************************************
 * mcmtest.c
 * C test program for  MCM Communication
 * requires USB to RS485 converter cable by FTDI
* Today found out that checksum was not going out with the address and data byte.
* changed the program, then chksum was there withh the outgoing commands
* still MCM was not sending response
* Charu figure out that with serial port idle line is in high state
* But with this USb-RS 485 cable idle line is in low state so a PULL UP register is connected with Orange cable
* Now commands are going to the MCM and MCM is sending response.
* For the read part I have to write some event based mechanism to read the incoming characters. Date: 16/08/2011

*** Written the event based READ also sumtimes it's working...but sumtimes It is failing.....

**** Now READ event is working for NUll command..I am getting RESPONSE for NULL command...Dt .17.08.2011

DwRxSize plays an important role here If I make it dwRxSize < 15 , except NULL commandI get response from all commands.

I have to determine the size of dwRxSize ..so that program can be dynamic....

Manually tried reading the address and Packet bytes from the incoming packets....but it didn't work as per desired...Date 18.8.2011

Tried SetbitMode also for Large Read..but it is not working...
also tried While(ftStatus != ft_OK ) but it is also not working...Dt..23.08.11


Now I am doing two read operation and it is working fine....

just finding it bit difficult to copy data from a char pointer to a char pointer...
******************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include<string.h>
#include <sys/io.h>       
#include <sys/types.h>
#include <sys/stat.h>
#include "/home/teleset/Desktop/Hello/ftd2xx.h"
#define BUF_SIZE  0x10
/*------------ display function-----------------------------------------*/

void disp_answer(char *buf_ptr)
{
 int i, length;


 length = *(buf_ptr+1) + (*(buf_ptr+2)<<8) + 1; //addr mcm
 for (i=0; i<length; i++) printf("%.2x\t", *(buf_ptr+i));
 printf("\n");
}

/*------------ checksum function-------------------------------------*/
char cksum(char *buff)
{
 int length, i;
 char CS=0;

 length = *(buff) + (*(buff+1)<<8);
 for(i=0; i<length; i++) CS += *(buff+i);
 return CS;
}

/*--------------Add Check Sum function---------------------------------*/
void add_cksum(char buff[])
{
 int length, i;
 char CS=0;

 length = buff[1] + (buff[2]<<8);
 for(i=1; i<length; i++) CS += *(buff+i);
 CS = ~CS+1;
 length = length+1;
 buff[length] = CS;
 fprintf(stderr, "I m in Add_chksum%.2x\t\n", (unsigned char)(buff[length])); 
}
/*------------ Write & read function----------------------------------*/
void write_read(char *buff, char *buf_ptr0)
{
   long bytes=0;
   int length,i,j;
   char CS=0;
   char  *pcBufRead = NULL, *testBuf = NULL;
   length = buff[1] + (buff[2]<<8);

  for(i=1; i<length; i++) CS += *(buff+i);
   CS = ~CS+1;
    //length = length+1;
   printf(" Length is %d\n", length);
   buff[length] = CS;
   buff[0]=*buf_ptr0;
    printf("Mcm addr %d\n", buff[0]);
      //add_cksum(buff);  
    DWORD dwBytesWritten,dwBytesInQueue = 0 ;
    DWORD dwRxSize = 2;
    FT_STATUS	ftStatus;
    FT_HANDLE	ftHandle;
    EVENT_HANDLE eh;
   
	/* Setup */
		if((ftStatus = FT_OpenEx("FTU6Z938", FT_OPEN_BY_SERIAL_NUMBER, &ftHandle)) != FT_OK)
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
		if((ftStatus = FT_Write(ftHandle, buff, 1, &dwBytesWritten)) != FT_OK) {
			printf("Error FT_Write(%d)\n", ftStatus);
			}
           
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
                                             
                      if((ftStatus = FT_Write(ftHandle, (buff+1),length, &dwBytesWritten)) != FT_OK) {
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
	
    //  	usleep(500); // Some delay is necessary because hardware can't write succesively...in a short period of time...
		
	// We Are writing the READ block of code
         pthread_mutex_init(&eh.eMutex, NULL);
	pthread_cond_init(&eh.eCondVar, NULL);

       ftStatus = FT_SetFlowControl(ftHandle, FT_FLOW_NONE, 0, 0);
	if(ftStatus != FT_OK) 
          {
		printf("Failed to set flow control\n");	
	  }

	ftStatus = FT_SetEventNotification(ftHandle, FT_EVENT_RXCHAR, (PVOID)&eh);
	if(ftStatus != FT_OK) {
		printf("Failed to set events\n");
		//return 1;
	}
	
	pthread_mutex_lock(&eh.eMutex);
	pthread_cond_wait(&eh.eCondVar, &eh.eMutex);
	pthread_mutex_unlock(&eh.eMutex);
	
                      pcBufRead = (char*)malloc(sizeof(char )*128);
                      testBuf = (char*)malloc(sizeof(char )*128);
                      memset(pcBufRead, 0x00, 128); 
                                      	
	              	ftStatus = FT_Read(ftHandle, pcBufRead,dwRxSize, &dwBytesInQueue);
                            if (ftStatus == FT_OK) 
                           {
				        printf("FT_Read = %d\n", dwBytesInQueue);
			        	for(j = 0; j < dwBytesInQueue; j++) 
                                         {
					     printf("pcBufWrite[%d] = 0x%02X\n", j, pcBufRead[j]);
				         }
			   }
			 else 
                             {
                                      fprintf(stderr,"ERROR FT_TIMEOUT(%d)\n", ftStatus);
				          
			       }
                         pcBufRead[strlen(pcBufRead)]='\0';
 
                         fprintf(stderr, " %x \n", pcBufRead[1]);
/*
                         printf(">>>>>>>>>> pcBufWrite= %s \n", pcBufRead);
                         printf("pcBufWrite= 0x%02X\n", pcBufRead[1]);
                          dwRxSize =0;
                         char *buffer; unsigned int countsize;
                         buffer = (unsigned  *) malloc (32);
                         memset(buffer, 0x00, 31);
                         sscanf(pcBufRead,"%*X %X",&countsize);
                         printf("--------%x \n",countsize);
*/
                         
                       /*
                       //  buffer = "a";
                           pcBufRead++;
                           //buffer = *pcBufRead;
                            // buffer[0] = pcBufRead[1];
                              //strcpy(buffer,(pcBufRead+1));
                   //   memcpy(buffer,(pcBufRead+1),1);
                            printf("Buffer = 0x%02X\n", *(pcBufRead));
                       //  printf(" String is %s\n", buffer);
                        // printf("Buffer = 0x%02X\n", *(buffer));
                          //dwRxSize = (DWORD)strtoul(pcBufRead, NULL, 16); */

                         fprintf(stderr, " -----------\n");
                         sprintf(testBuf,"%x",pcBufRead[1]);
                         fprintf(stderr, "=====  %x %x %s \n", pcBufRead[1], testBuf[0], testBuf);
                         dwRxSize = (DWORD)strtoul( testBuf, NULL, 16); 
                         fprintf(stderr, " -----------\n");
                            printf(" DWRXSIZE is %d\n",dwRxSize);
 
/*
                          dwRxSize = (DWORD)strtoul(buffer, NULL, 16);
                          dwRxSize = dwRxSize-1;
                            printf(" DWRXSIZE is %d\n",dwRxSize);
*/
/*
                            ftStatus = FT_Read(ftHandle, pcBufRead,dwRxSize, &dwBytesInQueue);
                            if (ftStatus == FT_OK) 
                           {
				        printf("FT_Read = %d\n", dwBytesInQueue);
			        	for(j = 0; j < dwBytesInQueue; j++) 
                                         {
					     printf("pcBufWrite[%d] = 0x%02X\n", j, pcBufRead[j]);
				         }
			   }
			 else 
                             {
                                      fprintf(stderr,"ERROR FT_TIMEOUT(%d)\n", ftStatus);
				          
			         }
                                //   disp_answer(pcBufRead);
*/
	/* Cleanup */
		FT_Close(ftHandle);
                
               free(pcBufRead);
    
}

/*--------------MCM Commands --------------------------------------*/ 
void read_mode(char *buf_ptr0)
{
 char buff[]={0,9,0,0,2,0,1,0,4,0};

 printf("read_mode\n");
 write_read(buff, buf_ptr0);
}

void read_version(char *buf_ptr0)
{
 char buff[]={0,9,0,0,2,0,1,0,3,0};

 printf("read_version\n");
 write_read(buff, buf_ptr0);
}

void read_dmask_32b(char *buf_ptr0)
{
 char buff[]={0,9,0,0,2,0,1,0,2,0};

 printf("read_dmask_32b\n");
 write_read(buff, buf_ptr0);
}

void read_dmask_16b(char *buf_ptr0)
{
 char buff[]={0,9,0,0,2,0,1,0,1,0};

 printf("read_dmask_16b\n");
 write_read(buff, buf_ptr0);
}

void read_an_mask(char *buf_ptr0)
{
 char buff[]={0,9,0,0,2,0,1,0,0,0};

 printf("read_an_mask\n");
 write_read(buff, buf_ptr0);
}

void set_dmask_32b(char *buf_ptr0)
{
 char buff[]={0,0xd,0,0,1,0,5,0,3,0xff,0xff,0xff,0xff,0};

 printf("set_dmask_32b\n");
 write_read(buff, buf_ptr0);
}

void set_dmask_16b(char *buf_ptr0)
{
 char buff[]={0,0xb,0,0,1,0,3,0,2,0xff,0xff,0};

 printf("set_dmask_16b\n");
 write_read(buff, buf_ptr0);
}

void set_an_mask(char *buf_ptr0)
{
 char buff[]={0,0x11,0,0,1,0,9,0,1,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0};

 printf("set_an_mask\n");
 write_read(buff, buf_ptr0);
}

void set_scan_mode(char *buf_ptr0)
{
 char buff[]={0,0xa,0,0,1,0,2,0,0,1,0};

 printf("set_scan_mode\n");
 write_read(buff, buf_ptr0);
}

void set_idle_mode(char *buf_ptr0)
{
 char buff[]={0,0xa,0,0,1,0,2,0,0,0,0};

 printf("set_idle_mode\n");
 write_read(buff, buf_ptr0);
}

void nul_cmd(char *buf_ptr0)
{
   char buff[]={0,8,0,0,0,0,0,0,0};
  
 printf("nul_cmd\n");
 write_read(buff, buf_ptr0);
}

void reboot(char *buf_ptr0)
{
 char buff[]={0,8,0,0,5,0,0,0,0};

 printf("reboot\n");
 write_read(buff, buf_ptr0);
}                    

/*------------ execute cmd function------------------------------------*/
void exec_cmd(int cmd, char *buf_ptr0)
{                                     
 switch(cmd)
 {
  case 0 : nul_cmd(buf_ptr0);break;
  case 1 : set_idle_mode(buf_ptr0);break;
  case 2 : set_scan_mode(buf_ptr0);break;
  case 3 : set_an_mask(buf_ptr0);break;
  case 4 : set_dmask_16b(buf_ptr0);break;
  case 5 : set_dmask_32b(buf_ptr0);break;
  case 6 : read_an_mask(buf_ptr0);break;
  case 7 : read_dmask_16b(buf_ptr0);break;
  case 8 : read_dmask_32b(buf_ptr0);break;
  case 9 : read_version(buf_ptr0);break;
  case 10 : read_mode(buf_ptr0);break;
  case 11 : reboot(buf_ptr0);break;
  default : break;
 }
}


int main(int argc, char *argv[])
{
 char *buf_ptr0;
 int i,cmd=0;

 buf_ptr0 = malloc(100);
 //ioperm(SERIAL_PORT, 8, 1);
 //perror("\nioperm :");
 
 for (i=0;i<3;i++) {
 printf("\nEnter Mcm Address:\t");
 fflush(stdout);
 scanf("%x", (int *)buf_ptr0);      
 printf("\nEnter Command:\t");
 fflush(stdout);
 scanf("%d", &cmd);      
  
 exec_cmd(cmd, buf_ptr0);
 printf("*****************************************\n");
 }
// free(buf_ptr0);
 return 1;
}


