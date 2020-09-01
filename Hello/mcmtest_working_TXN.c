/******************************************************************
 * mcmtest.c
 * C test program for  MCM Communication
 * requires USB to RS485 converter cable by FTDI
* Today found out that checksum was not going out with the address and data byte.
* changed the program, then chksum was there withh the outgoing commands
* still MCM was not sending response
* Charu figure out that with serial port idle line is in high state
* But with this USb-RS 485 cable idle line is in low state so a PULL off register is connected with Orange cable
* Now commands are going to the MCM and MCM is sending response.
* For the read part I have to write some event based mechanism to read the incoming characters. Date: 16/08/2011
******************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/io.h>       
#include <sys/types.h>
#include <sys/stat.h>
#include "/home/teleset/Desktop/Hello/ftd2xx.h"
#define BUF_SIZE 0x10
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
   char * 	pcBufRead = NULL;
   length = buff[1] + (buff[2]<<8);
   for(i=1; i<length; i++) CS += *(buff+i);
   CS = ~CS+1;
    //length = length+1;
   printf(" Length is %d\n", length);
    buff[length] = CS;
   buff[0]=*buf_ptr0;
    printf("Mcm addr %d\n", buff[0]);
   //  add_cksum(buff);  
      	DWORD	dwRxSize = 0;
	DWORD 	dwBytesWritten, dwBytesRead;
	FT_STATUS	ftStatus;
	FT_HANDLE	ftHandle;
        FT_DEVICE_LIST_INFO_NODE *devInfo;
        DWORD numDevs;
	int	iNumDevs = 0;
//	int	i, j;
	int	iDevicesOpen;

	/* Setup */
		if((ftStatus = FT_OpenEx("FTU6Z938", FT_OPEN_BY_SERIAL_NUMBER, &ftHandle)) != FT_OK)
                  {
			/* 
				This can fail if the ftdi_sio driver is loaded
		 		use lsmod to check this and rmmod ftdi_sio to remove
				also rmmod usbserial
		 	*/
			printf("Error FT_OpenEx(%d)\n", ftStatus);
			return 1;
		  }
                else
                   {
                      fprintf(stderr," Device opened successfully\n");
                   }   
	

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
              /*  else
                    {
                      fprintf(stderr,"Successfully written the address bytes [%d]  bytes on the device\n",dwBytesWritten);
                       fprintf(stderr, "%.2x\t\n", (unsigned char) *(buf_ptr0)); 
                    } */
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
                     /*   for (i=0; i<(length); i++) {
                          fprintf(stderr,"We are writing the buff %.2x  and value of I is [%d]\t\n", (unsigned char) buff[i+1],i);} */
                      if((ftStatus = FT_Write(ftHandle, (buff+1),length, &dwBytesWritten)) != FT_OK) {
                        printf("Error FT_Write(%d)\n", ftStatus);
                       
                }
           /*      else
                    {
                         fprintf(stderr,"Successfully written the Data bytes [%d]  bytes on the device\n",dwBytesWritten);
                            for (i=1; i<(length); i++) {
                          fprintf(stderr,"%.2x\t\n", (unsigned char) buff[i]);}
                          fprintf(stderr,"%.2x\t\n", (unsigned char) buff[length]);
                    } */

                      
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
	//	sleep(1);
		
		/* Read */
		dwRxSize = 0;			
		while ((dwRxSize < BUF_SIZE) && (ftStatus == FT_OK)) 
                {
			ftStatus = FT_GetQueueStatus(ftHandle, &dwRxSize);
		}
               FT_SetTimeouts(ftHandle,5000,0);
		if(ftStatus == FT_OK) 
                  {
		  	pcBufRead = (char*)realloc(pcBufRead, dwRxSize);
			memset(pcBufRead, 0xFF, dwRxSize);
			for(j = 0; j < dwRxSize; j++) {
				printf("pcBufWrite[%d] = 0x%02X\n", j, pcBufRead[j]);
			}
                          FT_SetTimeouts(ftHandle,5000,0);
			ftStatus = FT_Read(ftHandle, pcBufRead, dwRxSize, &dwBytesRead);
                            if (ftStatus == FT_OK) 
                           {
				        printf("FT_Read = %d\n", dwBytesRead);
			        	for(j = 0; j < dwBytesRead; j++) 
                                         {
					     printf("pcBufWrite[%d] = 0x%02X\n", j, pcBufRead[j]);
				         }
			   }
			 else 
                             {
                                      fprintf(stderr,"ERROR FT_TIMEOUT(%d)\n", ftStatus);
				          
			         }
	        	}
		else
                        {
			      printf("Error FT_GetQueueStatus(%d)\n", ftStatus);	
		        }


	/* Cleanup */
		FT_Close(ftHandle);

		


   // disp_answer(buf_ptr0);
     
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
   char buff[]={0,8,0,0,0,0,0,0};
  
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
 free(buf_ptr0);
 return 1;
}



