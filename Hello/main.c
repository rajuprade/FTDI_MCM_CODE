/*
	Simple example to open a maximum of 4 devices - write some data then read it back.
	Shows one method of using list devices also.
	Assumes the devices have a loopback connector on them and they also have a serial number

	To build use the following gcc statement 
	(assuming you have the d2xx library in the /usr/local/lib directory).
	gcc -o simple main.c -L. -lftd2xx -Wl,-rpath /usr/local/lib
*/

#include <stdio.h>
#include "/home/teleset/Desktop/Hello/ftd2xx.h"

#define BUF_SIZE 0x10

#define MAX_DEVICES		5

int main()
{
	char 	cBufWrite[BUF_SIZE];
	char * 	pcBufRead = NULL;
	char * 	pcBufLD[MAX_DEVICES + 1];
	char 	cBufLD[MAX_DEVICES][64];
	DWORD	dwRxSize = 0;
	DWORD 	dwBytesWritten, dwBytesRead;
	FT_STATUS	ftStatus;
	FT_HANDLE	ftHandle;
        FT_DEVICE_LIST_INFO_NODE *devInfo;
        DWORD numDevs;
	int	iNumDevs = 0;
	int	i, j;
	int	iDevicesOpen;


        char *buf_ptr0;
         char buff[]={0,8,0,0,0,0,0,0,0};

      buf_ptr0 = malloc(100);
      printf("\nEnter Mcm Address:\t");
      fflush(stdout);
      scanf("%x", (int *)buf_ptr0);

		
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
		if((ftStatus = FT_Write(ftHandle, buf_ptr0, 1, &dwBytesWritten)) != FT_OK) {
			printf("Error FT_Write(%d)\n", ftStatus);
			return 1;
		}
                else
                    {
                      fprintf(stderr,"Successfully written [%d]  bytes on the device\n",dwBytesWritten);
                       printf("%.2x\t", (unsigned char) *(buf_ptr0)); 
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

                      if((ftStatus = FT_Write(ftHandle, buff,7, &dwBytesWritten)) != FT_OK) {
                        printf("Error FT_Write(%d)\n", ftStatus);
                        return 1;
                }
                 else
                    {
                      fprintf(stderr,"Successfully written [%d]  bytes on the device\n",dwBytesWritten);
                      for (i=0; i<7; i++) printf("%.2x\t", (unsigned char) buff[i]);
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
	//	sleep(1);
		
		/* Read */
		dwRxSize = 0;			
		while ((dwRxSize < BUF_SIZE) && (ftStatus == FT_OK)) {
			ftStatus = FT_GetQueueStatus(ftHandle, &dwRxSize);
		}
		if(ftStatus == FT_OK) {
			pcBufRead = (char*)realloc(pcBufRead, dwRxSize);
			memset(pcBufRead, 0xFF, dwRxSize);
			for(j = 0; j < dwRxSize; j++) {
				printf("pcBufWrite[%d] = 0x%02X\n", j, pcBufRead[j]);
			}
			if((ftStatus = FT_Read(ftHandle, pcBufRead, dwRxSize, &dwBytesRead)) != FT_OK) {
				printf("Error FT_Read(%d)\n", ftStatus);
			}
			else {
				printf("FT_Read = %d\n", dwBytesRead);
				for(j = 0; j < dwBytesRead; j++) {
					printf("pcBufWrite[%d] = 0x%02X\n", j, pcBufRead[j]);
				}
			}
		}
		else {
			printf("Error FT_GetQueueStatus(%d)\n", ftStatus);	
		}


	/* Cleanup */
		FT_Close(ftHandle);

	return 0;
}
