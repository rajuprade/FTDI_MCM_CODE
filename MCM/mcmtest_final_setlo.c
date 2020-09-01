/******************************************************************
 * mcmtest.c @Author Raj Uprade
 * C test program for  MCM Communication
 * requires USB to RS485 converter cable by FTDI
 * As per ABR lab requirement implemented the SET L0 commands which takes as input lo1 lo2 and convert respective frequencies into 32 or 64 bit digital mask argument dt.24.2.2012 

******************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include<string.h>
#include <sys/io.h>       
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pthread.h>
#include<fcntl.h>
#include "/home/teleset/Desktop/Hello/ftd2xx.h"
#define BUF_SIZE  0x10
unsigned char Arg[100];
/*--------------Add Check Sum function---------------------------------*/
void add_cksum(char buff[])
{
 int length, i;
 char CS=0;

 length = buff[1] + (buff[2]<<8);
 for(i=1; i<length; i++) CS += *(buff+i);
 CS = ~CS+1;
// length = length+1;
 buff[length] = CS;
 fprintf(stderr, "I m in Add_chksum%.2x\t\n", (unsigned char)(buff[length])); 
}
/*------------ Write & read function----------------------------------*/
void write_read(char *buff, char *buf_ptr0)
{
   long bytes=0;
   int length,i,j;
   char CS=0;
  unsigned char  *pcBufRead = NULL, *testBuf = NULL;
   length = buff[1] + (buff[2]<<8);
//  long sTime=0, nxtTime=0;
  /*  for(i=1; i<length; i++) CS += *(buff+i);
   CS = ~CS+1;
    //length = length+1;
   printf(" Length is %d\n", length);
   buff[length] = CS;*/
// printf(" ********************************************************ADDRESS is %s*****************************************\n",buf_ptr0);
    buff[0]=*buf_ptr0;
    printf("Mcm addr %d\n", buff[0]);
      add_cksum(buff);  
    DWORD dwBytesWritten = 0,dwBytesInQueue = 0 ;
    DWORD dwRxSize = 2;
    FT_STATUS ftStatus;
    FT_HANDLE ftHandle;
    EVENT_HANDLE eh;
    //fcntl(ftHandle, F_SETFL, O_NONBLOCK);
// sTime = time(NULL);
	/* Setup */
                if((ftStatus = FT_OpenEx("FTU6Z938", FT_OPEN_BY_SERIAL_NUMBER, &ftHandle)) != FT_OK)
		// if((ftStatus = FT_OpenEx("FTUN7KZE", FT_OPEN_BY_SERIAL_NUMBER, &ftHandle)) != FT_OK)
 //                   if((ftStatus = FT_OpenEx("FTUN7NUJ", FT_OPEN_BY_SERIAL_NUMBER, &ftHandle)) != FT_OK)
  //                      if((ftStatus = FT_OpenEx("FTUN7IF4", FT_OPEN_BY_SERIAL_NUMBER, &ftHandle)) != FT_OK)

      //      if((ftStatus = FT_OpenEx("FTSERRS1", FT_OPEN_BY_SERIAL_NUMBER, &ftHandle)) != FT_OK)
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
	// Set read timeout of 5sec, write timeout of 1sec
                    ftStatus = FT_SetTimeouts(ftHandle, 1000, 1000);
                       if (ftStatus == FT_OK) {
                                     // FT_SetTimeouts OK
                                        fprintf(stderr," Device SET timeout ok \n");
                                           }
                                            else {
                                                                   // FT_SetTimeouts failed
                                                           fprintf(stderr," Device SET timeout FAiled \n");
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
          
                        usleep(4000);
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
                                             
                      if((ftStatus = FT_Write(ftHandle, (buff+1),length, &dwBytesWritten)) != FT_OK) {
                        printf("Error FT_Write(%d)\n", ftStatus);
                         }
                         else
                          {
                                for(i=0;i<dwBytesWritten;i++)
                                printf("We have successfully written buffer %x\n", (unsigned char)buff[i]);
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
		

	// We Are writing the READ block of code
 /*       pthread_mutex_init(&eh.eMutex, NULL);
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
	pthread_mutex_unlock(&eh.eMutex); */
	
       
                     
                      pcBufRead = (char*)malloc(sizeof(char )*512);
                      testBuf = (char*)malloc(sizeof(char )*512);
                      memset(pcBufRead, 0x00, 512); 
                      memset(testBuf, 0x00,512); 
           
              
                  //  while( (sTime > 0 ) && (abs(nxtTime-sTime) > DEVRESP_TO) )
                  //     {
            	             ftStatus = FT_Read(ftHandle, pcBufRead,dwRxSize, &dwBytesInQueue);
                             if (ftStatus == FT_OK) 
                                 {
                                     //    nxtTime=time(NULL);
				        printf("FT_Read = %d\n", dwBytesInQueue);
                                           if(dwBytesInQueue == 0 ) 
                                               {
                                                      fprintf(stderr,"############### MCM DEVICE TIMEOUT ###########\n\n\n\n\n\n"); 
                                                } 
			        	else
                                          { 
                                           for(j = 0; j < dwBytesInQueue; j++) 
                                         {
					     printf("pcBufWrite[%d] = 0x%02X\n", j, pcBufRead[j]);
				         }
                                      }
			          }
                 
			   else 
                             {
                                      fprintf(stderr,"ERROR FT_TIMEOUT(%d)\n", ftStatus);
				      printf("Device TIMEOUT\n"); 
			        } 
                          
//                         pcBufRead[strlen(pcBufRead)]='\0';
 
//                         fprintf(stderr, " %x \n", pcBufRead[1]);
                        sprintf(testBuf,"%x",pcBufRead[1]);
  //                       fprintf(stderr, "=====  %x %x %s \n", pcBufRead[1], testBuf[0], testBuf);
                         dwRxSize = (DWORD)strtoul( testBuf, NULL, 16); 
    //                     fprintf(stderr, " -----------\n");
      //                      printf(" DWRXSIZE is %d\n",dwRxSize);
                              if (dwRxSize == 0 ) 
                                    goto again; 

                            dwRxSize = dwRxSize-1;
                        /*    printf(" DWRXSIZE is %d\n",dwRxSize);
                           if(dwRxSize == -1 )
                             {
                                    printf("Device TIMEOUT)\n"); 
                              } */
                                 

                             dwBytesInQueue = 0;  
                            ftStatus = FT_Read(ftHandle, pcBufRead,dwRxSize, &dwBytesInQueue);
                            if (ftStatus == FT_OK) 
                           {
				       // printf("FT_Read = %d\n", dwBytesInQueue);
			        	//for(j = 0; j < dwBytesInQueue; j++) 
			        	printf("FT_Read = %d\n",dwRxSize);
                                         for(j = 0; j < dwRxSize; j++) 
                                         {
					     printf("pcBufWrite[%d] = 0x%02X\n", j, pcBufRead[j]);
				         }
			   }
			 else 
                             {
                                      fprintf(stderr,"ERROR FT_TIMEOUT(%d)\n", ftStatus);
				          
			         }
                        again:
                           free(pcBufRead);
                           free(testBuf); 
                              //  disp_answer(pcBufRead);
                               //   free(pcBufRead);
	/* Cleanup */
		FT_Close(ftHandle);
                
        /*       free(pcBufRead);
               free(testBuf);  */
}

/*--------------MCM Commands --------------------------------------*/ 

/* Convert() ---> Convert the Mask bit into Argument for SET Dmask 32 or 64 ******/

Convert(int offset, char mask[8])
{
  int j;
  unsigned char tab1[3], tab2[3];

  for (j=0;j<2;j++) {tab1[j]=mask[j];tab2[j]=mask[j+2];}     //"ffff" mask format
  tab1[2] = tab2[2] = '\n';       //strtol() converts until '\n'
  Arg[0+offset] = strtol((const char *)tab2, NULL, 16);     //3412 order
  Arg[1+offset] = strtol((const char *)tab1, NULL, 16);
}

/* Mask64to Arg convert  mask into Argument */

Mask64toArg(char *buf_ptr0,char mask0[8], char mask1[8], char mask2[8], char mask3[8])
{
  Convert(0, mask0); Convert(2, mask1);
  Convert(4, mask2); Convert(6, mask3);
  char buff[64]={0,0x11,0,0,1,0,9,0,4,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xE1};
  printf("#################################################################\n");
  printf("64 MASK bit%s %s %s %s\n", mask0,mask1,mask2,mask3);
  printf("#################################################################\n");
  printf("64 bit HEX Argument generated %x %x %x %x %x %x %x %x\n",(int)Arg[0],(int)Arg[1],(int)Arg[2],(int)Arg[3],(int)Arg[4],(int)Arg[5],(int)Arg[6],(int)Arg[7]);
  printf("#################################################################\n");
    buff[9] = Arg[0];
    buff[10] =Arg[1];
    buff[11] =Arg[2];
    buff[12] =Arg[3]; 
    buff[13] = Arg[4];
    buff[14] = Arg[5];
    buff[15] = Arg[6];
   buff[16] = Arg[7];
   printf("set_dmask_64b\n");
//  printf(" ********************************************************ADDRESS is %s*****************************************\n",buf_ptr0);
 write_read(buff, buf_ptr0);
}

/* Mask32to Arg convert  mask into Argument */

Mask32toArg(char *buf_ptr0,char mask0[8], char mask1[8])
{
  Convert(0, mask0); Convert(2, mask1);
  char buff[64]={0,0xd,0,0,1,0,5,0,3,0,0,0,0,0};
   printf("#################################################################\n");
   printf(" 32 MASK bit %s %s\n",mask0,mask1);
   printf("#################################################################\n");
   printf("32 HEX bit Argument generated %x %x %x %x \n",(int)Arg[0],(int)Arg[1],(int)Arg[2],(int)Arg[3]);
    printf("#################################################################\n");
    buff[9] = Arg[0];
    buff[10] =Arg[1];
    buff[11] =Arg[2];
    buff[12] =Arg[3]; 
 printf("set_dmask_32b\n");
 //printf(" ********************************************************ADDRESS is %s*****************************************\n",buf_ptr0);
write_read(buff, buf_ptr0);
}

/* SET_LO Command */
void SET_LO(char *buf_ptr0 )
    {
    int lo1, lo2;
    printf("ENTER <LO1>  <LO2>\n");
    scanf("%d %d",&lo1,&lo2);
  
  /*  switch( num )
    {
	case 1: printf("Enter the Frequency 1\n");
                scanf("%d",&lo1);
                lo2 = lo1 ;
		break;
	case 2: printf("Enter the Frequency 1 and frequency 2\n");
                scanf("%d %d",&lo1,&lo2);
                break;
        case 3 :printf("we have not entered the frequency\n");
                 break; 
    }*/
    fprintf( stdout,"lo1= %d, lo2= %d\n", lo1, lo2 );

   
 if(set_newlo(buf_ptr0, lo1, lo2 ))
        printf("Error in setting lo1 lo2.\n");
        else
        {
         printf("\n\n");
         printf("#################################################################\n");
         printf("Successfully SET THE LO1 FREQUENCY=>%d  LO2 FREQUENCY=>%d\n",lo1,lo2);    
         printf("#################################################################\n");                                                                   
       }
    }

/* Actaul set_lo function */

int set_newlo(char *buf_ptr0, int lo1, int lo2 )
{
    int lofreq,vco_no,tmp1,stp,frq,bitset,tmp2,temp=0,lofreq1,lofreq2;
    char mask[5][8],mask1[5][8],mode[80],maska[2][8],mask1a[2][8];
    char permission,ans[10],maskp[2][8],mask1p[2][8];
    char masksp[6][8] = {"00080","08080","01000","09000","0600D","0E01D"};
    int mnum,nnum,mdec,ndec,bx,acum,acum1,acum2,acum3,acuma,acump,yigsc,cnt=2,maxfrq;
   
    int MASK1=61440;
    int MASK2=4095;

	lofreq1 = lo1;
	lofreq2 = lo2;
	maxfrq = (lofreq2 >= lofreq1) ? lofreq2 : lofreq1;


    if (lofreq1 < 100 || lofreq1 > 1795 || lofreq2 < 100 || lofreq1 > 1795)
	{
        error (4);
        return 1;
	}

    if ( 100<=lofreq1 && lofreq1<110 || 190<lofreq1 && lofreq1<220 )
	 {
        do    {
		   printf("LOFREQ1 can not be guaranteed.\n");
		   printf("Do you wish to continue?(y/n)");
		   scanf("\n%c",&permission);
	      }
            while(permission !='y' && permission !='n');
        if (permission == 'n')
            {
            printf("Abnormal exit...\n");
            return 1;
            }
	}
	if( 100<=lofreq2 && lofreq2<110 || 190<lofreq2 && lofreq2<220)
            {
        do    {
		   printf("LOFREQ2 can not be guaranteed.\n");
		   printf("Do you wish to continue?(y/n)");
		   scanf("\n%c",&permission);
	      }
            while(permission !='y' && permission !='n');
        if (permission == 'n')
            {
            printf("Abnormal exit...\n");
            return 1;
            }
	}

	if( (lofreq1 != lofreq2) && ((lofreq1 <= 600 && lofreq2 <= 600) ||
				     (lofreq1 >= 605 && lofreq2 >= 605))  )
	{
		printf("\nSingal VCO cannot support two diffrent Frequencies ");
		error(4);
		return(1);
	}

   if(((lofreq1>=355) && (((float)lofreq1/5-(int)(lofreq1/5)) != 0))
   || ((lofreq2>=355) && (((float)lofreq2/5-(int)(lofreq2/5)) != 0)))
	{
	    printf("For VCO 3 and VCO 4, lofreq only in steps of 5MHz.\n");
	    error(4);
	    return(1);
	}

lofreq = lofreq1;
while(cnt--)
{

	if(lofreq >= 100 && lofreq <= 200) {vco_no = 1;stp = 1;}
	if(lofreq >= 201 && lofreq <= 354) {vco_no = 2;stp = 1;}
	if(lofreq >= 355 && lofreq <= 600) {vco_no = 3;stp = 5;}
	if(lofreq >= 601 && lofreq <= 1800){vco_no = 4;stp = 5;}
	yigsc = 25;

    if(vco_no == 1 || vco_no == 4)
        {
	sprintf(mask[4], "6005");
	sprintf(mask1[4],"E015");
        }
    if(vco_no == 2 || vco_no == 3)
        {
	sprintf(mask[4], "6002");
	sprintf(mask1[4],"E012");
        }

    if((vco_no == 1) && ( lofreq1 <= lofreq2))
        {
	sprintf(mask[2], "0168");
	sprintf(mask1[2],"8168");
	sprintf(mask[3], "1001");
	sprintf(mask1[3],"9001");
        }
    if((vco_no == 1) && ( lofreq1 > lofreq2))
        {
        sprintf(mask[2], "01a8");
        sprintf(mask1[2],"81a8");
        sprintf(mask[3], "1001");
        sprintf(mask1[3],"9001");
        }
    
    if((vco_no == 2) && (lofreq1 <= lofreq2)) 
        {
	sprintf(mask[2], "0146");
	sprintf(mask1[2],"8146");
	sprintf(mask[3], "1002");
	sprintf(mask1[3],"9002");
        }
     if((vco_no == 2) && (lofreq1 > lofreq2))  
        {
        sprintf(mask[2], "0186");
        sprintf(mask1[2],"8186");
        sprintf(mask[3], "1002");
        sprintf(mask1[3],"9002");
        }
    
    if((vco_no == 3) && (lofreq1 <= lofreq2))
        {
	sprintf(mask[2], "0245");
	sprintf(mask1[2],"8245");
	sprintf(mask[3], "1004");
	sprintf(mask1[3],"9004");
        }
    if((vco_no == 3) && (lofreq1 > lofreq2)) 
        {
        sprintf(mask[2], "0285");
        sprintf(mask1[2],"8285");
        sprintf(mask[3], "1004");
        sprintf(mask1[3],"9004");
        }
   
    if((vco_no == 4) && (lofreq1 <= lofreq2)) 
        {
	sprintf(mask[2], "0258");
	sprintf(mask1[2],"8258");
	sprintf(mask[3], "1008");
	sprintf(mask1[3],"9008");
        }
  if((vco_no == 4) && (lofreq1 > lofreq2))  
        {
        sprintf(mask[2], "0298");
        sprintf(mask1[2],"8298");
        sprintf(mask[3], "1008");
        sprintf(mask1[3],"9008");
        }
 

     if((lofreq1 == lofreq2) && (vco_no != 4))
	strcpy(mode,"SYN1A->CH1LO,SYN1B->CH2LO");
     if((lofreq1 == lofreq2) && (vco_no == 4))
	strcpy(mode,"SYN2B->CH1LO,SYN2A->CH2LO");
     if(lofreq1 < lofreq2)
	strcpy(mode,"SYN1A->CH1LO,SYN2A->CH2LO");
     if(lofreq1 > lofreq2)
	strcpy(mode,"SYN2B->CH1LO,SYN1B->CH2LO");


    frq = lofreq/stp;
    mdec=frq-10*((int)(frq/10));
    mnum=15-mdec;
    ndec=((int)(frq/10));
    nnum=255-ndec+1;
    if(mdec >= ndec)
        {
	    printf("ERROR:%d MHz LO is Technically not feasible\n",lofreq);
        return 1;
        }


    bx=nnum*16;
    acum=bx+mnum;
     
	if(vco_no == 4 ) {
                acum = 4095 - acum;
		sprintf(mask[0], "4%03X",acum);
		sprintf(mask1[0],"C%03X",acum);
         } else {
                acum1 = acum & MASK1;
                acum2 = acum ^ MASK2;
                acum3 = acum1 | acum2;
                printf("04%03X 04%03X\n \n", acum, acum3);
		sprintf(mask[0], "04%03X",acum3);
		sprintf(mask1[0],"0C%03X",acum3);

         }


    bx=255*16;
    acum=bx+15;

    if(vco_no == 4)
    {
	bitset = (int)(lofreq-200)/yigsc;
/*	if(sc == 1) bitset--;
	if(sc == 3) bitset++; */
	temp = 0;
	if(lofreq <= 1070) temp = 192;
	if(lofreq >= 1075 && lofreq <= 1100) temp = 64;
	if(lofreq > 1100 && lofreq <= 1200) temp = 128;
	acum = bitset + 3840 + temp;
	acuma = bitset + 1 + 3840 + temp;
	acump = bitset - 1 + 3840 + temp;
    }

    sprintf(mask[1], "2%03X",acum);
    sprintf(mask1[1],"A%03X",acum);
    if(vco_no == 4)
    {
    sprintf(maska[1], "2%03X",acuma);
    sprintf(mask1a[1],"A%03X",acuma);
    sprintf(maskp[1], "2%03X",acump);
    sprintf(mask1p[1],"A%03X",acump);
    }



    if((lofreq1 == lofreq2) && (cnt == 1))
       {
        if(vco_no == 4) *buf_ptr0= 2;
        else  *buf_ptr0= 3;
        Mask64toArg(buf_ptr0,masksp[0], masksp[1], masksp[2], masksp[3]);
        Mask32toArg(buf_ptr0,masksp[4],masksp[5]);
    }
    else
    {

    if(vco_no == 4) *buf_ptr0= 3;
         else  *buf_ptr0= 2;
       
       
     Mask32toArg(buf_ptr0,masksp[2],masksp[3]);
      Mask64toArg(buf_ptr0,mask[0], mask1[0], mask[1], mask1[1]);

    if(vco_no == 4)       
            *buf_ptr0= 3;
     else *buf_ptr0= 2;
    

    } 
  Mask64toArg(buf_ptr0,mask[2], mask1[2], mask[3], mask1[3]);
  Mask32toArg(buf_ptr0,mask[4], mask1[4]);
 }
    if(cnt == 1) lofreq = lofreq2;
    return 0;
   
}
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
   int i;
    unsigned char Argument[64];
//  char buff[64]={0,0xd,0,0,1,0,5,0,3,0xff,0xff,0xff,0xff,0}; // after statically declaring 64 bytes of memory..segmentation falut problem was solved ...
char buff[64]={0,0xd,0,0,1,0,5,0,3,0,0,0,0,0}; 
 printf("\nEnter Dig Mask 32 (xx xx xx xx):\t");
      for (i=0;i<4;i++) 
  {
    scanf("%x", (int *)&Argument[i]);
 //   buff[9+i] = Argument[i];
 }
    buff[9] = Argument[1];
    buff[10] =Argument[0];
    buff[11] =Argument[3];
    buff[12] =Argument[2]; 
 printf("set_dmask_32b\n");
 write_read(buff, buf_ptr0);
}

void set_dmask_16b(char *buf_ptr0)
{
   int i;
    unsigned char Argument[64];
   char buff[]={0,0xb,0,0,1,0,3,0,2,0xff,0xff,0};
   printf("\nEnter Dig Mask 16 (xx xx):\t");
  for (i=0;i<2;i++)
  {
    scanf("%x", (int *)&Argument[i]);
 //   buff[9+i] = Argument[i];
  } 
 buff[9] = Argument[1];
 buff[10] = Argument[0];

 printf("set_dmask_16b\n");
 write_read(buff, buf_ptr0);
} 


/* Set_dmask_16 modification tried for ABR lab as they were asking for no space between xx xx.
void set_dmask_16b(char *buf_ptr0)
{
   int i;
    unsigned char Argument[64],arg1[20],arg2[20],*p,tempbuf;
   char buff[]={0,0xb,0,0,1,0,3,0,2,0xff,0xff,0};
  printf("\nEnter Dig Mask 16 (xxxx)=>");
//   for (i=0;i<2;i++)
  //{
    //scanf("%x", (int *)&Argument[i]);
 //   buff[9+i] = Argument[i];
  }   
// fgets(Argument, sizeof(Argument), stdin);
   scanf("%x", &tempbuf);
  sprintf(Argument,"%x", tempbuf);
  
//  printf("\nlength is %d #%s#\n\n",strlen(buff),  buff);
  memcpy(arg1,Argument,2);
  arg1[2]='\0';
  printf("\nlength is %d #%s#\n\n",strlen(arg1), arg1);
  p=Argument + 2;
   memmove(Argument,p,sizeof(Argument));
   strcpy(arg2,Argument);
   printf("\nlength is %d #%s#\n\n",strlen(arg2), arg2);

 buff[9] = arg2;
 buff[10] = arg1; 
 
// buff[9] = Argument[1];
// buff[10]= Argument[0];

 printf("set_dmask_16b\n");
 write_read(buff, buf_ptr0);
} */

void set_an_mask(char *buf_ptr0)
{
    int i;
   //char buff[]={0,0x11,0,0,1,0,9,0,1,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
     unsigned char Argument[64];
    char buff[64]={0,0x11,0,0,1,0,9,0,1,0,0,0,0,0,0,0,0,0};
     printf("\nEnter Anl Mask (xx xx xx xx xx xx xx xx):\t");
      for (i=0;i<8;i++)
      {
          scanf("%x", (int *)&Argument[i]);
          buff[9+i] = Argument[i];
      }
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


void FE_control_old(char *buf_ptr0)
{
   int i;
    unsigned char Argument[64];
  char buff[32]={0,0xa,0,0,3,0,2,0,0,0,0};
    printf("\nEnter Feed data mask (xx):\t");
       scanf("%x", (int *)&Argument[0]);
        buff[8] = Argument[0];
        buff[9] = Argument[0]>>8;
 printf("feed_mcm_cmd\n");
 write_read(buff, buf_ptr0);
}

void feed_data_monitoring(char *buf_ptr0)
{
 char buff[32]={0,0xa,0,0,1,0,2,0,0,1,0xf2};

 printf("feed_data_monitoring\n");
 write_read(buff, buf_ptr0);
}
void FE_control_new(char *buf_ptr0)
{
    int i;
    unsigned char Argument[64];
    char buff[32]={0,0xa,0,0,6,0,2,0,0,0,0};
    printf("\nEnter Feed data mask(xx xx):\t");
      for (i=0;i<2;i++) 
    {
      scanf("%x", (int *)&Argument[i]);
      buff[8+i] = Argument[i];
     }
 printf("feed_control_modified\n");
 write_read(buff, buf_ptr0);
}
void fe_box_mon(char *buf_ptr0)
{
     int i;
     unsigned char Argument[64];
    char buff[32]={0,0xa,0,0,7,0,2,0,0,0,0xED };
    printf("\nEnter Feed box number:\t");
   scanf("%d", (int *)&Argument[0]); 
    buff[9] = Argument[0]-1;
 printf("fe_box_mon\n");
 write_read(buff, buf_ptr0);
}
void common_box_mon(char *buf_ptr0)
{ 
    char buff[32]={0,9,0,0,7,0,1,0,1,0xEE };
    printf("common_box_mon\n");
    write_read(buff, buf_ptr0);
}

void set_dmask_64b(char *buf_ptr0)
{
    int i;
    unsigned char Argument[64];
    char buff[64]={0,0x11,0,0,1,0,9,0,4,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xE1}; // after statically declaring 64 bytes of memory..segmentation falut problem was solved ...
     printf("\nEnter Dig Mask 64 (xx xx xx xx xx xx xx xx):\t");
      for (i=0;i<8;i++) 
    {
       scanf("%x", (int *)&Argument[i]);
     // buff[9+i] = Argument[i];
    }
buff[9] = Argument[1];
buff[10] = Argument[0];
buff[11] = Argument[3];
buff[12] = Argument[2];
buff[13] = Argument[5];
buff[14] = Argument[4];
buff[15] = Argument[7];
buff[16] = Argument[6];
 printf("set_dmask_64b\n");
 write_read(buff, buf_ptr0);
}

void read_dmask_64b(char *buf_ptr0)
{
  char buff[64]={0,9,0,0,2,0,1,0,5,0xEF} ;//after statically declaring 64 bytes of memory..segmentation falut problem was solved ...

 printf("read_dmask_64b\n");
 write_read(buff, buf_ptr0);
}
void set_mean_mode(char *buf_ptr0)
{
    int i;
    unsigned char Argument[64];
   char buff[]={0,0xb,0,0,1,0,3,0,0,2,0,0};
  printf("\nEnter Avg Factor:\t");
  scanf("%d", (int *)&Argument[0]);
 buff[10] = Argument[0];
  printf("set_mean_mode\n");
 write_read(buff,buf_ptr0);
}
void set_mcm_addr(char *buf_ptr0)
{
   printf("\nEnter Mcm Address:\t");
   fflush(stdout);
   scanf("%x", (int *)buf_ptr0);      
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
  case 12 :FE_control_old(buf_ptr0);break;
  case 13 : feed_data_monitoring(buf_ptr0);break;
  case 14 : FE_control_new(buf_ptr0);break;
  case 15 : fe_box_mon(buf_ptr0);break;
  case 16 : common_box_mon(buf_ptr0);break;
  case 17 : set_dmask_64b(buf_ptr0);break;
  case 18 : read_dmask_64b(buf_ptr0);break;
  case 19 : set_mean_mode(buf_ptr0); break;
  case 20 : set_mcm_addr(buf_ptr0);break;
  case 21 : SET_LO(buf_ptr0);break;
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
 
 //for (i=0;i<3;i++) {
//for(;;){

  int ret=-1;

  ret = system("rmmod ftdi_sio");
  if(ret)
    {
       system("rmmod usbserial");
       printf("\n No need to remove Default drivers loaded\n");
    }
    else
     {
       printf("Removed the default drivers\n");
     }

  printf("\nEnter Mcm Address:\t");
   fflush(stdout);
 scanf("%x", (int *)buf_ptr0);      
 printf("\nEnter Command from the list:\n\n");
  for(;;){ 
 printf(" 0 : nul_cmd\n");
 printf(" 1 : set_idle_mode\n");
  printf("  2 : set_scan_mode\n");
  printf("  3 : set_an_mask\n");
  printf("  4 : set_dmask_16b\n");
   printf(" 5 : set_dmask_32b\n");
   printf(" 6 : read_an_mask\n");
   printf(" 7 : read_dmask_16b\n");
  printf("  8 : read_dmask_32b\n");
  printf("  9 : read_version\n");
   printf(" 10 : read_mode\n");
   printf("  11 : reboot\n");
   printf("  12 :FE_control_old\n");
    printf(" 13 : feed_data_monitoring\n");
  printf(" 14 : FE_control_new\n");
    printf(" 15 : fe_box_mon\n");
   printf("  16 : common_box_mon\n");
    printf(" 17 : set_dmask_64b\n");
    printf(" 18 : read_dmask_64b\n");
   printf("  19 : set_mean_mode\n");
   printf("  20 : set_mcm_address\n");
   printf("  21 : SET_LO\n");

 fflush(stdout);
 scanf("%d", &cmd);      
  exec_cmd(cmd, buf_ptr0);
  printf("\n\n");
 printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ TIME TO ENTER NEW COMMAND $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
 }
 free(buf_ptr0);
 return 1;
}


