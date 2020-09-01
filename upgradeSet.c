/*-----------------------------------------------------------------*/
/* AAGE PICHE modification
   creates LOA.SET and LOP.SET for frequencies > 600 MHz */
/*-----------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define COMMENT   '*'
#define LINE_LEN  120

int set_para[32];
float post_gain[2];
int bblo[2];
char PratrCode(int );

/*-----------------------------------------------------------------*/
    void filter_comment(char *x)
    {
    char *p;
    p=strchr(x, COMMENT);
    if(p!=NULL)
        *p='\0';
    }

/*-----------------------------------------------------------------*/
int get_int(char *SetLine)
{
       char *temp1;
       char str[10], str_dest[10];
       int lo,len;

       temp1=strtok(SetLine, ".");
       strcpy(str, temp1);
       if(strlen(str) != 2) return 0;	
       temp1=strtok(NULL, "\n");
       strcat(str, temp1);
       if(strlen(str) >= 9) return atoi(strncpy(str_dest, str,8));
       else {
       	lo=atoi(str);
       	while(lo < 9999999) lo*=10;
/*       fprintf(stderr, "Integer lo %s\n", str);	*/
       
        return lo;
       }
}

/*-------------------------------------------------------------------*/
short bitDO[8];
void getbitDO(short word)
{
  int i, mask, t_bit;
  extern short bitDO[];

   for(i=7;i >= 0;)
   {
     mask = 1<<i;
     t_bit =  word & mask;            /* and operator */
     bitDO[i] =  ( t_bit == 0?0:1 ) ;
     i--;
   }
#ifdef JDEBUG
    for(i=0;i<7;i++) bitDO[i] == 0?fprintf(stderr,"0"):fprintf(stderr,"1");
    fprintf(stderr, "\n");
#endif

}

/*----------------------------------------------------*/
    int main (void)
    {
    FILE *f1;
    char SetLine[LINE_LEN];
    int i;

    f1=fopen("/home/observer/set/user/mcm4set/setupnew.txt","r");
    if(f1==NULL)
        {
	printf("\nCan not open 'setupnewtxt'. Abnormal exit...\n");
        return 20;
        }
    i=0;
    while(!feof(f1))
        {
        fgets(SetLine, LINE_LEN-1, f1);
        filter_comment(SetLine);
        if(is_empty(SetLine))
            continue;
	if(i==7 ) {
		post_gain[0]=atof(SetLine);
        } else  if(i==9) {
		post_gain[1]=atof(SetLine);
        }
/*
	if(i==20 || i==21 ) 
        	bblo[i-20]=get_int(SetLine);
  		printf("The %d th lo is %d\n",i-20,bblo[i-20]);	*/
	else 	{
	set_para[i]=atoi(SetLine);
        }

        i++;
    }

/*       printf("Post gain CH1 and CH2 resp : %f %f\n",post_gain[0],post_gain[1]);*/

    if(set_fe())
        printf("Error in creating 'fe.set'...\n");
        else
        printf("Successfully created 'fe.set'.\n");
    if(set_newlo())
        printf("Error in creating 'lo.set'...\n");
        else
        printf("Successfully created 'lo.set'.\n");
/*    if(set_if())*/
    if(NewIF())
        printf("Error in creating 'if.set'...\n");
        else
        printf("Successfully created 'if.set'.\n");

/* set bb and bblo disabled 7 Nov 2016 JPK 
    if(set_bb())
        printf("Error in creating 'bb.set'...\n");
        else
        printf("Successfully created 'bb.set'.\n");
    if(set_bblo())
        printf("Error in creating 'bblo.set'...\n");
        else
        printf("Successfully created 'bblo.set'.\n");
*/
    return 0;
    }

/*-----------------------------------------------------------------*/
    int is_empty(char *x)
    {
    int i;

    for(i=0; x[i]==' '||x[i]=='\t'||x[i]=='\n'; i++);
    if (x[i]=='\0')
        return 1;
    else
        return 0;
    /*return x[i]=='\0' ? 1 : 0;*/
    }

/*-----------------------------------------------------------------*/
/*    generates a run file FE.SET for FE parameters.           */

    int set_fe(void)
    {
    int data11,data12,data21,data22,data31,data32;
    int data1,data2,data3,add_fe;
    int Walsh_Enbl,FEDAT_ON,FEDAT_OFF;
    short NG_cycle, NG_Freq[2], FE_Sel[2], WLSH_P[2], WLSH_Freq[2], WLSH_Grp[2], ns_cal_index;
    unsigned short mcm4word[2];
    char WE[16],WG[16],SWAP[16];
    FILE *f1;
    int i, b;
    short digWord[32];

    int p_data11[] = {0x0, 0x1, 0x8, 0xa, 0xa,  0xa,  0xa,  0xa,  0x10, 0x10, 0x10, 0x10, 0x10, 0x14, 0x14, 0x14, 0x14, 0x14};
    int p_data32[] = {0x0, 0x0, 0x0,0x60, 0x20, 0x40, 0x0,  0x80, 0x60 ,0x20, 0x80, 0x40, 0x00, 0x00, 0x40, 0x20, 0x60, 0x90};
    int p_add []   = {0x1, 0x2, 0x3, 0x4, 0x4,  0x4,  0x4,  0x4,  0x5 ,  0x5, 0x5,  0x5,   0x5,  0x6,  0x6,  0x6,  0x6, 0x6};
    int freq_band[]= {50, 150, 235,  290, 350,  410,  470,  325,  600, 685, 725, 770, 850,1060, 1170, 1280, 1390, 1420, -1};

    int p_data31[] = {0x06, 0x05, 0x0a, 0x09, 0x15};

    int p_data12[] = {0x80, 0xe0, 0x00, 0x60, 0xa0};
    int p_data21[] = {0x01, 0x01, 0x00, 0x00, 0x00};
    int sol_attr[] = {   0,   14,   30,   44,  -1};

    int ns_cal[]   = {   0,    1,    2,    3, -1};
    char *N_CAL[]   = {"E-HI","HI","MED","LOW","RF-OFF"};

/*    control word for BAND SELECTION & RF ON/OFF.           */

    for(i=0; freq_band[i]!=-1 && set_para[0]!= freq_band[i]; i++);


    if (freq_band[i] == -1)
    {
        error(0);
        return 1;
    }
    data11 = p_data11[i];
    data32 = p_data32[i];
    add_fe = p_add[i];

/*     control word for setting SOLAR ATTENUATOR in the common box.*/

    for(i=0; sol_attr[i]!=-1 && set_para[1] !=sol_attr[i]; i++);
/*    if(sol_attr[i] == -1)
    {
        error(1);
        return 1;
    }
*/    data12 = p_data12[i];
    data21 = p_data21[i];

/*     control word for setting SWAP SWITCH in common box.*/

    if (set_para[2]==0)
    {
        data22=0x00;
        sprintf(SWAP,"Unswapped");
    }
    else
    {
        data22=0x02;
        sprintf(SWAP,"Swapped");
    }

/*     control word for setting NOISE CAL LEVEL in front end box.*/

    for(i=0; set_para[3]!=ns_cal[i]; i++);
    ns_cal_index=i;

    data31 = p_data31[i];

    data1=data11+data12;  /* data for the lower latch for ch1 & ch2 in common box.*/
    data2=data21+data22;  /* data for the higher latch for ch1 & ch2 in common box.*/
    data3=data31+data32;  /* data for front end box.
			 address of this box is given by add_fe.*/

/* Date Nov 7, 2016, NDS Email 25 Oct, Walsh, NG Generation, Walsh group and FE settings through MCM 4 
   -- JPK */

            for(i=0;i<32;i++) digWord[i]=0;

            for( i = 0; i < 2;i++)
            {

                   /* ---- NG Cycle ------ bit D0 D1 */
                  switch(set_para[14]) {
                       case 0  :NG_cycle = 0; break;
                       case 25 :NG_cycle = 1; break;
                       case 50 :NG_cycle = 2; break;
                       case 100:NG_cycle = 3; break;
                       default : fprintf(stderr, "\n ### WARNING : Invalid NG_cycle value, considering 0 \n"); NG_cycle = 0;
                  }
                   getbitDO((short)NG_cycle); for (b=0;b<2;b++) { digWord[b + (i*16)] = bitDO[b]; }

                   /* ---- NG Frequency -------------------- bit D2 D3 */
                   NG_Freq[i] = set_para[15+i];
                   if ( NG_Freq[i] > 3 || NG_Freq[i] < 0 ) {
                         fprintf(stderr, "\n ### Error : Invalid NG_Freq value %d \n", set_para[15+i]);
                         exit(1);
                   }
                   getbitDO((short)NG_Freq[i]); for (b=0;b<2;b++) { digWord[ 2 + b + (i*16)] = bitDO[b]; }

                   /* ------ FE Sel ON OFF ----------- bit D4 */
                   FE_Sel[i] = set_para[17+i];
                   if ( FE_Sel[i] > 1 || FE_Sel[i] < 0 ) {
                         fprintf(stderr, "\n ### Error : Invalid FE_Sel value %d \n", set_para[17+i]);
                         exit(1);
                   }
                   digWord[4 + (i * 16) ] = FE_Sel[i];

                   /* --------- Walsh Pattern ------- bit D5,6,7 */
                   WLSH_P[i] = set_para[19];
                   if ( WLSH_P[i] > 7 || WLSH_P[i] < 0 ) {
                         fprintf(stderr, "\n ### Error : Invalid WLSH_P value %d \n", set_para[19]);
                         exit(1);
                   }
                   getbitDO((short)WLSH_P[i]); for (b=0;b<3;b++) { digWord[ 5 + b + (i*16)] = bitDO[b]; }

                   /* ------------ Walsh Freq --------- bit D8,9 */
                   WLSH_Freq[i] = set_para[20+i];
                   if ( WLSH_Freq[i] > 3 || WLSH_Freq[i] < 0 ) {
                         fprintf(stderr, "\n ### Error : Invalid WLSH_P value %d \n", set_para[20+i]);
                         exit(1);
                   }
                   getbitDO((short)WLSH_Freq[i]); for (b=0;b<2;b++) { digWord[ 8 + b + (i*16)] = bitDO[b]; }

                   /* SPARE -------------------- bit D10,11 - SPARE */
                   digWord[ 10 + (i*16)] = digWord[ 11 + (i*16)] = 0;

                   /* Walsh Group ------------ bit D12,13, D14- Group  FREEZED TO VALUE 1
                   WLSH_Grp[i] = set_para[22+i];
                   if ( WLSH_Grp[i] > 1 || WLSH_Grp[i] < 0 ) {
                         fprintf(stderr, "\n ### Error : Invalid WLSH_P value %d \n", set_para[22+i]);
                         exit(1);
                   }
                   getbitDO((short)WLSH_Grp[i]); 
                   */
                   digWord[ 12 + (i*16)] = digWord[ 13 + (i*16)]= digWord[ 14 + (i*16)] = 1;

                   /* bit D15 - EN */
                    digWord[15 + (i * 16) ] = i?1:0;


#ifdef JDEBUG
                fprintf(stderr, "\n CH[%d] - NG_cycle[%d] , NG_Freq[%d], FE_sel[%d], WLSH_P[%d] WLSH_Freq[%d] WLSH_Grp[%d] ", i,
                        NG_cycle[i], NG_Freq[i], FE_Sel[i], WLSH_P[i], WLSH_Freq[i], WLSH_Grp[i]);    
#endif

            }


                    mcm4word[0] = 0 ;  mcm4word[1] = 0;

                    for(i=0 ;i <  16 ;i++) {
                       if(digWord[i+16]) mcm4word[0] = mcm4word[1] | (1 << i );
                       if(digWord[i]) mcm4word[1] = mcm4word[0] | (1 << i );
                    }

#ifdef JDEBUG
                    fprintf(stderr,"\n");
                    for( i = 0 ; i < 16;i++)
                        fprintf(stderr, "%d", mcm4word[0] & ( 1 << i )? 1:0);
                    fprintf(stderr,"\n");
                    for( i = 0 ; i < 16 ;i++)
                        fprintf(stderr, "%d", mcm4word[1] & ( 1 << i )?1:0);
                    fprintf(stderr,"\n");

              fprintf(stderr,"\n* MCM4BITS :");
              fprintf(stderr, "ana 0%4xx 0%4xx\n", mcm4word[1], mcm4word[0]);   
              fprintf(stderr, "ana 0%4xx 0%4xx\n", mcm4word[1] & (~ (unsigned short)( 1 << 4)), mcm4word[0] & (~(unsigned short)( 1 << 4 )) );   
              fprintf(stderr, "ana 0%4xx 0%4xx\n", mcm4word[1] | ( 1 << 4) , mcm4word[0] | ( 1 << 4 ) );   
#endif

	    f1 = fopen("/home/observer/set/user/mcm4set/fe.set", "w");
	    if(f1==NULL)
	    {
		printf("File opening error.\nAbnormal exit...");
		return 20;
	    }

	    fprintf(f1,"\n* run file for setting FRONT END parameters.\n");

	    fprintf(f1,"\nmpa 2 4 5");
	    fprintf(f1,"\ncomm 29;dest 17;t3v\n"); fprintf(f1,"\n");

                   /* bit D4 */
            for(i = 0 ; i < 2;i++)
            fprintf(f1,"\n * CH[%d] Settings : NG_cycle, NG_Freq[%d], FE_sel[%d], WLSH_P[%d] WLSH_Freq[%d] WLSH_Grp[%d] ", 
                             i, NG_cycle, NG_Freq[i], FE_Sel[i], WLSH_P[i], WLSH_Freq[i], WLSH_Grp[i]);

         /* fprintf(f1,"\nana 070%02Xx 0f0%02Xx",FEDAT_OFF,FEDAT_OFF);  
	    fprintf(f1,"\n\n* MCM5_ON with Walsh:%s,Walsh Group:%s and Noise Generator duty cycle:%d\n",WE,WG,set_para[14]);
	    fprintf(f1,"\nana 070%02Xx 0f0%02Xx",FEDAT_ON,FEDAT_ON);
	    fprintf(f1,"\nst32dig(4)"); */

            fprintf(f1, "\n* MCM 5 OFF \n");
            fprintf(f1, "ana 0%4xx 0%4xx\n", mcm4word[1] & (~ (unsigned short)( 1 << 4)), mcm4word[0] & (~(unsigned short)( 1 << 4 )) );   
	    fprintf(f1, "st32dig(4)\n");

            fprintf(f1, "\n* MCM 5 ON \n");
            fprintf(f1, "ana 0%4xx 0%4xx\n", mcm4word[1] | ( 1 << 4 ), mcm4word[0] | ( 1 << 4) ); 
	    fprintf(f1, "st32dig(4)\n");



	    fprintf(f1,"\n\n* Freq. Band:%d MHz, Solar Attn.:%d dB\n",set_para[0],set_para[1]);

	    fprintf(f1,"\n* Polarisation:%s, Noise-Cal level:%s",SWAP,N_CAL[ns_cal_index]);
	    
	    fprintf(f1,"\n\nenamcmq");

   /* Order changed to test new 325 upgraded feed with Rabbit -- 23 May 2016 */
	/*   fprintf(f1,"\nana 07%02xx 00%02xx 08%02xx 00%02xx",data1, data1, data2, data2); */
	     fprintf(f1,"\nana 09%02xx 00%02xx 0A%02xx 00%02xx",data1, data1, data2, data2);   
	    fprintf(f1,"\nst64dig(5)");
	/*   fprintf(f1,"\nana 09%02xx 00%02xx 0A%02xx 00%02xx",data1, data1, data2, data2); */
	     fprintf(f1,"\nana 07%02xx 00%02xx 08%02xx 00%02xx",data1, data1, data2, data2);   
	    fprintf(f1,"\nst64dig(5)");
	    for(i=1; i<add_fe; i++)
			{
		fprintf(f1,"\nana %02x16x 0016x",i);
		    fprintf(f1,"\nst32dig(5)");
			}
	    for(i=add_fe+1; i<7; i++)
			{
		fprintf(f1,"\nana %02x16x 0016x",i);
		    fprintf(f1,"\nst32dig(5)");
			}
	    fprintf(f1,"\nana %02x%02xx 00%02xx",add_fe,data3,data3);
    fprintf(f1,"\nst32dig(5)");
    fprintf(f1,"\ndismcmq\n");

   /* FE-CB Monitoring */
    fprintf(f1,"\n\n* FE-CB MONITORING \n");
    fprintf(f1,"comm 710;dest=5;t3v;t3v\n");
    fprintf(f1,"comm 700;dest=5;CPA(1)=%d;t3v;t3v\n",add_fe-1);

            fprintf(f1, "\n* MCM ON \n");
            fprintf(f1, "ana 0%4xx 0%4xx\n", mcm4word[1] | ( 1 << 4 ), mcm4word[0] | ( 1 << 4) ); 
	    fprintf(f1, "st32dig(4)\n");

            fprintf(f1, "\n* MCM OFF \n");
            fprintf(f1, "ana 0%4xx 0%4xx\n", mcm4word[1] & (~ (unsigned short)( 1 << 4)), mcm4word[0] & (~(unsigned short)( 1 << 4 )) );   
	    fprintf(f1, "st32dig(4)\n");

/*
    fprintf(f1,"\n\n* MCM5_ON  with Walsh:%s,Walsh Group:%s and Noise Generator duty cycle:%d\n",WE,WG,set_para[14]);
    fprintf(f1,"\nana 070%02Xx 0f0%02Xx",FEDAT_ON,FEDAT_ON);
    fprintf(f1,"\nst32dig(4)\n");
    fprintf(f1,"\n\n* MCM5_OFF with Walsh:%s,Walsh Group:%s and Noise Generator duty cycle:%d\n",WE,WG,set_para[14]);
    fprintf(f1,"\nana 070%02Xx 0f0%02Xx",FEDAT_OFF,FEDAT_OFF);
    fprintf(f1,"\nst32dig(4)\n");
*/

    fprintf(f1,"\nmpa 1 5");
    fprintf(f1,"\ncomm 30;dest 17;t3v\n\n");

    fclose(f1);
    return 0;
    }

/*-----------------------------------------------------------------*/
/*    generates a run file IF.SET for IF parameters.           */

/* ALCs can be set Independently for the two Channels
   BWs can also be set independently
   
   Pre-Attn or Post Attn get set for the two Channels simultaneously.
   Therefore it is not possible to modify Pre or Post Attn. in a given Channel
   without disturbing the other channel 
*/
    int set_if(void)
    {
    int d[4],bw1,bw2;
    int pre_att,pst_att,bw,alc;
    int i,j;
    FILE *f1;

    int p_bw2[]     = {0xc8, 0xd0, 0xe0};
    int p_bw1[]     = {0x01, 0x02, 0x04};
    int band_width[]= {   6,   16,   32, -1};

    for (i=6,j=0; i<10; i++,j++)
    {
        if(set_para[i]<0 || 30<set_para[i])
        {
            error(5);
            return 1;
        }
        d[j]=set_para[i];
        d[j]=(d[j])/2;

        if (d[j]<=3 || (d[j]>=8 && d[j]<=11))/*to invert second MSB*/
            d[j]=d[j]+4;
        else
            d[j]=d[j]-4;

        if (j>=2)     /* j>=2  ???? */
            d[j]=d[j]*16;
    }
/* Pre and Post Attn. get set for both Channels SIMULTANEOUSLY */
    pre_att= d[0]+d[2]; /* d[0] refers to CH2 (175 MHz),d[2] to CH1 */
    pst_att= d[1]+d[3];

/*     control word for IF BW setting.                             */

    for(i=0; band_width[i]!=-1 && set_para[10]!=band_width[i]; i++);
    if(band_width[i] == -1)
        {
        error(9);
        return 1;
        }
    bw1 = p_bw1[i];
    for(i=0; band_width[i]!=-1 && set_para[11]!=band_width[i]; i++);
    if(band_width[i] == -1)
        {
        error(10);
        return 1;
        }
    bw2 = p_bw2[i];
    bw = bw2 + bw1;

/*    control word for ALC ON/OFF.                               */

    if((set_para[12]==0)&&(set_para[13]==0))
        alc=0xff;

    if((set_para[12]==0)&&(set_para[13]==1))
        alc=0xfd;

    if((set_para[12]==1)&&(set_para[13]==0))
        alc=0xfe;

    if((set_para[12]==1)&&(set_para[13]==1))
        alc=0xcc;


/* O/P File being created */
    f1 = fopen("/home/observer/set/user/mcm4set/if.set" , "w+t");
    if (f1 == NULL)
        {
        printf("File opening error.\nAbnormal exit...");
        return 20;
        }
    
    fprintf(f1,"\n*run file for setting IF parameters.\n");

    fprintf(f1,"\nmpa 1 9");
    fprintf(f1,"\ncomm 29;dest 17;t3v\n");

    fprintf(f1,"\nenamcmq\n");
    fprintf(f1,"\n*CH1 pre & post attenuations are, %d & %d dB resp.",set_para[6],set_para[7]);
    fprintf(f1,"\n*CH2 pre & post attenuations are, %d & %d dB resp.\n",set_para[8],set_para[9]);
    fprintf(f1,"\nana 0b3%02xx 0f3%02xx  0b7%02xx 0f7%02xx", pre_att,pre_att,pst_att,pst_att);
    fprintf(f1,"\nst64dig(9)");

/* IF Attn setting done */
    fprintf(f1,"\n*BW in CH1 & CH2 are, %d & %d resp.\n", set_para[10],set_para[11]);
    fprintf(f1,"\nana 0fb%02xx 0fb%02xx 0bb%02xx 0fb%02xx", bw, bw, bw, bw);
    fprintf(f1,"\nst64dig(9)\n");

    fprintf(f1,"\n*ALCs are:\n");

    if (set_para[12] == 0)
        fprintf(f1,"* \tCH1: OFF\n");
    else
        fprintf(f1,"* \tCH1: ON\n");
    
    if (set_para[13] == 0)
        fprintf(f1,"* \tCH2: OFF\n");
    else
        fprintf(f1,"* \tCH2: ON\n");


    fprintf(f1,"\nana 0ff%02xx 0ff%02xx 0bf%02xx 0ff%02xx" ,alc, alc, alc, alc);
    fprintf(f1,"\nst64dig(9)");
    fprintf(f1,"\ndismcmq\n");

    fclose(f1);
    return 0;
    }

/*-----------------------------------------------------------------*/
/*    generates a run file LO.SET for lo freq setting.  (OLD)     */
/*
    int set_lo(void)
    {
    int lofreq,vco_no,tmp,stp,frq,BITSET;
    char mask[5][8],mask1[5][8];
    char permission;
    int mnum,nnum,mdec,ndec,bx,acum;
    FILE *f1;

    lofreq = set_para[4];
    if (lofreq < 100 || lofreq > 1600)
        {
        error (4);
        return 1;
        }
    if ( 100<=lofreq && lofreq<110 || 190<lofreq && lofreq<220)
        {
        do    {
            printf("LO can not be guaranteed.\n");
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
    if(lofreq >= 100 && lofreq < 200) vco_no = 1;
    if(lofreq >= 200 && lofreq < 355) vco_no = 2;
    if(lofreq >= 355 && lofreq < 600) vco_no = 3;
    if(lofreq >= 600) vco_no = 4;

    if(vco_no == 3 || vco_no == 4)
        {
        tmp = lofreq / 5;
        lofreq = tmp * 5;
        stp = 5;
        if ((set_para[4] - lofreq) != 0)
            {
            printf("For VCO 3 and VCO 4, LOFreq only in steps of 5MHz.\n");
            printf("Selecting LOWER possible Freq as... %d.\n",lofreq);
            }
        }
        else stp = 1;

    if(vco_no == 1 || vco_no == 4)
        {
        sprintf(mask[4], "06005x");
        sprintf(mask1[4],"0E015x");
        }
    if(vco_no == 2 || vco_no == 3)
        {
        sprintf(mask[4], "06002x");
        sprintf(mask1[4],"0E012x");
        }

    if(vco_no == 1)
        {
        sprintf(mask[2], "00168x");
        sprintf(mask1[2],"08168x");
        sprintf(mask[3], "01001x");
        sprintf(mask1[3],"09001x");
        }
    if(vco_no == 2)
        {
        sprintf(mask[2], "00146x");
        sprintf(mask1[2],"08146x");
        sprintf(mask[3], "01002x");
        sprintf(mask1[3],"09002x");
        }
    if(vco_no == 3)
        {
        sprintf(mask[2], "00245x");
        sprintf(mask1[2],"08245x");
        sprintf(mask[3], "01004x");
        sprintf(mask1[3],"09004x");
        }
    if(vco_no == 4)
        {
        sprintf(mask[2], "00258x");
        sprintf(mask1[2],"08258x");
        sprintf(mask[3], "01008x");
        sprintf(mask1[3],"09008x");
        }

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

    if(vco_no == 4)
        acum = 4095 - acum;

    sprintf(mask[0], "04%03Xx",acum);
    sprintf(mask1[0],"0C%03Xx",acum);
    bx=255*16;
    acum=bx+15;

    if(vco_no == 4)
    {
        BITSET = (int)(lofreq-200)/25;
        acum = BITSET+3840;
    }

    sprintf(mask[1], "02%03Xx",acum);
    sprintf(mask1[1],"0A%03Xx",acum);

    f1 = fopen("/home/observer/set/user/mcm4set/lo.set","w+t");
    if (f1 == NULL)
        {
        printf("File opening error.\nAbnormal exit...");
        return 20;
        }

    fprintf(f1,"\n* run file for setting LO at %d MHz.\n", set_para[4]);

    if(vco_no == 4)
        fprintf(f1,"\n mpa 1 3");
    else
        fprintf(f1,"\n mpa 1 2");

    fprintf(f1,"\n comm 29;dest 17;t3v\n");

    fprintf(f1,"\n enamcmq\n");

    fprintf(f1,"\n ana %s %s %s %s", mask[0], mask1[0], mask[1], mask1[1]);
    if(vco_no == 4) fprintf(f1,"\n st64dig(3)"); 
    else fprintf(f1,"\n st64dig(2)"); 

    fprintf(f1,"\n ana %s %s %s %s", mask[2], mask1[2], mask[3], mask1[3]);
    if(vco_no == 4) fprintf(f1,"\n st64dig(3)");
    else fprintf(f1,"\n st64dig(2)"); 
 
    fprintf(f1,"\n ana %s %s", mask[4], mask1[4]);
    if(vco_no == 4) fprintf(f1,"\n st32dig(3)"); 
    else fprintf(f1,"\n st32dig(2)"); 

    fprintf(f1,"\n dismcmq\n");

    fclose(f1);
    return 0;
    }
*/
/*-----------------------------------------------------------------*/
    int error(int err)
    {
    printf("\nWRONG VALUE ENTERED FOR ");
    switch(err)
        {
        case 0:  printf("BAND SELECTION\n");   break;
        case 1:  printf("SOLAR ATTENUATOR\n"); break;
        case 3:  printf("CAL NOISE\n");        break;
        case 4:  printf("LO frequency.\n");    break;
        case 5:  printf("IF ATTENUATION\n");   break;
        case 9:  printf("IF BW for CH1\n");    break;
        case 10: printf("IF BW dor CH2\n");    break;
        case 11: printf("Invalid BB LO ( 50MHz  <= LO <= 90MHz)\n");    break;
        case 12: printf("BB LO. In steps of 100 Hz only\n");    break;
        default: printf("\nABNORMAL EXIT...");
        }
    }
/*-----------------------------------------------------------------*/
/*    generates a run file BB.SET for FE parameters.           */

    int set_bb(void)
    {
        char *bw[]={"ff","fe","f4","e2","70","a0","60","20","00"};
        char *gn[]={"00","08","04","02","01","80","40","20","10"};
        char *f[]={"0f","1f","2f","3f","4f","5f","6f","7f","8f","9f","af","bf","cf","df","ef","ff"};
         char mask[10][80];
	int addr[]={1,2,3,4,5,6,7,8,17,18,19,20,21,22,23,24,25};
        static int i,j,b,g,mcmaddr;
        FILE *fp;

	if((fp=fopen("/home/observer/set/user/mcm4set/bb.set","w"))==NULL){
            printf("Can't open BB MAP file .. quiting ..\n");
            exit(1);
        }
        fprintf(fp,"*Set BBW=%d  Gain=%d\n\n",set_para[18],set_para[19]);
        switch(set_para[18]) {
            case 16000:b=0;break;
            case 8000:b=1;break;
            case 4000:b=2;break;
            case 2000:b=3;break;
            case 1000:b=4;break;
            case 500:b=5;break;
            case 250:b=6;break;
            case 125:b=7;break;
            case 62:b=8;break;
            default :b=1;
        }
        switch(set_para[19]) {
            case 0:g=0;break;
            case 3:g=1;break;
            case 6:g=2;break;
            case 9:g=3;break;
            case 12:g=4;break;
            case 15:g=5;break;
            case 18:g=6;break;
            case 21:g=7;break;
            case 24:g=8;break;
            default :g=1;
        }
        j=0;
        for(i=0;i<=8;i++) {
            if(i<=6){
            sprintf(mask[i],"0%s%sx 0%s%sx 0%s%sx 0%s%sx",f[j],gn[g],f[j+1],gn[g],f[j+1],bw[b],f[j+2],bw[b]);
            j+=2; 
            }else{
                sprintf(mask[i],"0%s%sx 0%s%sx",f[j],gn[g],f[j+1],gn[g]);
                sprintf(mask[i+1],"0%s%sx",f[j+1],bw[b]);
		break;
	    }
        } 

        mcmaddr = set_para[17];    
        if(!mcmaddr) fprintf(fp,"ante 16 1 2 3 4 5 6 7 8 17 18 19 20 21 22 23 24 ;stcebmcm\n\n");
        else    fprintf(fp,"mpa 1 %d;stcebmcm\n",mcmaddr);    
     for(j=0;j<16;j++) {
        fprintf(fp,"enacebq\n");
	for(i=0;i<9;i++){
	    fprintf(fp,"ana %s\n",mask[i]);
	    if(!mcmaddr){
                 if(i<=6) { fprintf(fp,"stceb64(%d)\n",addr[j]);}
	         else if(i==7) { fprintf(fp,"stceb32(%d)\n",addr[j]);}
	         else if(i==8) { fprintf(fp,"stceb16(%d)\n",addr[j]);}
            } else {
                if(i<=6) fprintf(fp,"stceb64(%d)\n",mcmaddr);
                else if(i==7) fprintf(fp,"stceb32(%d)\n",mcmaddr);
                else if(i==8) fprintf(fp,"stceb16(%d)\n",mcmaddr);
	    }
        }
        if(mcmaddr != 0) break;
        if(!mcmaddr) fprintf(fp,"comm=210;dest=%d;t3v\n",addr[j]+100);
        else fprintf(fp,"comm=210;dest=%d;t3v\n",mcmaddr+100);
        fprintf(fp,"comm=3;dest=19;abcqdes=19;t3v\n\n"); 
      }
        fclose(fp);
  
	return 0;
    }
/*-----------------------------------------------------------------*/


/************************************************************************/
/* generates a run file LO.SET for lo freq setting (NEW SYN1 and SYN2 ).*/

int set_newlo(void)
{
    int lofreq,vco_no,tmp1,stp,frq,bitset,tmp2,temp=0,lofreq1,lofreq2;
    char mask[5][8],mask1[5][8],mode[80],maska[2][8],mask1a[2][8];
    char permission,ans[10],maskp[2][8],mask1p[2][8];
    char masksp[6][8] = {"00080x","08080x","01000x","09000x","0600Dx","0E01Dx"};
    int mnum,nnum,mdec,ndec,bx,acum,acuma,acump,yigsc,cnt=2,maxfrq;
	int acum1, acum2, acum3;
    FILE *f1,*fa,*fp;
   int MASK1=61440;
   int MASK2=4095;



	lofreq1 = set_para[4];
	lofreq2 = set_para[5];
	maxfrq = (lofreq2 >= lofreq1) ? lofreq2 : lofreq1;

/*	if((lofreq1 >=601 && lofreq1 <=1800) ||
		(lofreq2 >=601 && lofreq2 <=1800))
	{
	printf("\nFor LO from YIG select\n");
	printf("\nAage(3) Ya Piche(1) Enter for default (0) ? : ");
	gets(ans);
	if(ans == '\0') sc = 0;
	else
	sc = atoi(ans);
	}       */

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
	sprintf(mask[4], "06005x");
	sprintf(mask1[4],"0E015x");
        }
    if(vco_no == 2 || vco_no == 3)
        {
	sprintf(mask[4], "06002x");
	sprintf(mask1[4],"0E012x");
        }

    if((vco_no == 1) && ( lofreq1 <= lofreq2))
        {
	sprintf(mask[2], "00168x");
	sprintf(mask1[2],"08168x");
	sprintf(mask[3], "01001x");
	sprintf(mask1[3],"09001x");
        }
    if((vco_no == 1) && ( lofreq1 > lofreq2))
        {
        sprintf(mask[2], "001a8x");
        sprintf(mask1[2],"081a8x");
        sprintf(mask[3], "01001x");
        sprintf(mask1[3],"09001x");
        }
    
    if((vco_no == 2) && (lofreq1 <= lofreq2)) 
        {
	sprintf(mask[2], "00146x");
	sprintf(mask1[2],"08146x");
	sprintf(mask[3], "01002x");
	sprintf(mask1[3],"09002x");
        }
     if((vco_no == 2) && (lofreq1 > lofreq2))  
        {
        sprintf(mask[2], "00186x");
        sprintf(mask1[2],"08186x");
        sprintf(mask[3], "01002x");
        sprintf(mask1[3],"09002x");
        }
    
    if((vco_no == 3) && (lofreq1 <= lofreq2))
        {
	sprintf(mask[2], "00245x");
	sprintf(mask1[2],"08245x");
	sprintf(mask[3], "01004x");
	sprintf(mask1[3],"09004x");
        }
    if((vco_no == 3) && (lofreq1 > lofreq2)) 
        {
        sprintf(mask[2], "00285x");
        sprintf(mask1[2],"08285x");
        sprintf(mask[3], "01004x");
        sprintf(mask1[3],"09004x");
        }
   
    if((vco_no == 4) && (lofreq1 <= lofreq2)) 
        {
	sprintf(mask[2], "00258x");
	sprintf(mask1[2],"08258x");
	sprintf(mask[3], "01008x");
	sprintf(mask1[3],"09008x");
        }
  if((vco_no == 4) && (lofreq1 > lofreq2))  
        {
        sprintf(mask[2], "00298x");
        sprintf(mask1[2],"08298x");
        sprintf(mask[3], "01008x");
        sprintf(mask1[3],"09008x");
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

        if(vco_no == 4 ){
            acum = 4095 - acum;
                sprintf(mask[0], "04%03Xx",acum);
                sprintf(mask1[0],"0C%03Xx",acum);
        }
        else {
                acum1 = acum & MASK1;
        acum2 = acum ^ MASK2;
        acum3 = acum1 | acum2;
        /*printf("04%03Xx 04%03Xx\n \n", acum, acum3);*/
        sprintf(mask[0], "04%03Xx",acum3);
        sprintf(mask1[0],"0C%03Xx",acum3);
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

    sprintf(mask[1], "02%03Xx",acum);
    sprintf(mask1[1],"0A%03Xx",acum);
    if(vco_no == 4)
    {
    sprintf(maska[1], "02%03Xx",acuma);
    sprintf(mask1a[1],"0A%03Xx",acuma);
    sprintf(maskp[1], "02%03Xx",acump);
    sprintf(mask1p[1],"0A%03Xx",acump);
    }


    if(cnt == 1)
    {
    f1 = fopen("/home/observer/set/user/mcm4set/lo.set","w");
    if (f1 == NULL)
        {
        printf("File opening error.\nAbnormal exit...");
        return 20;
	}
    if(maxfrq > 600)
    {
    fa = fopen("/home/observer/set/user/mcm4set/loa.set","w");
    if (f1 == NULL)
        {
        printf("File opening error.\nAbnormal exit...");
        return 20;
	}
    fp = fopen("/home/observer/set/user/mcm4set/lop.set","w");
    if (f1 == NULL)
        {
        printf("File opening error.\nAbnormal exit...");
        return 20;
	}
    }

    fprintf(f1,"\n* Run file for setting SYN1 LO at %d MHz. and SYN2 LO at %d MHz", set_para[4],set_para[5]);
    fprintf(f1,"\n* MODE : %s\n",mode);
    if(maxfrq > 600)
    {
    fprintf(fa,"\n* Run file for incrementing one bit of YIGDAC for LO at %d MHz.", maxfrq);
    fprintf(fp,"\n* Run file for decrementing one bit of YIGDAC for LO at %d MHz.", maxfrq);
    }
  }/* if cnt == 1 */

    if(cnt == 1)
    {
    fprintf(f1,"\n mpa 2 2 3");
    fprintf(f1,"\n comm 29;dest 17;t3v");
/*    fprintf(f1,"\n enamcmq\n"); */
    if(maxfrq > 600)
    {
    fprintf(fa,"\n mpa 2 2 3");
    fprintf(fa,"\n comm 29;dest 17;t3v");
    fprintf(fa,"\n enamcmq\n");
    fprintf(fp,"\n mpa 2 2 3");
    fprintf(fp,"\n comm 29;dest 17;t3v");
    fprintf(fp,"\n enamcmq\n");
    }
    }

    if((lofreq1 != lofreq2) || (cnt == 1))
    {
    fprintf(f1,"\n* FREQ : %d MHz, STP : %d MHz, VCO : %d",lofreq,stp,vco_no);
    if(maxfrq > 600 && vco_no == 4)
    {
    fprintf(fa,"\n* FREQ : %d MHz, STP : %d MHz, VCO : %d   (AAGE) ",lofreq,stp,vco_no);
    fprintf(fp,"\n* FREQ : %d MHz, STP : %d MHz, VCO : %d   (PICHE)",lofreq,stp,vco_no);
    }
    }
    if((lofreq1 == lofreq2) && (cnt == 1))
       {
        fprintf(f1,"\n enamcmq\n"); 
	fprintf(f1,"\n ana %s %s %s %s",masksp[0],masksp[1],masksp[2],masksp[3]);
	if(vco_no == 4) fprintf(f1,"\n st64dig(2)");
	else fprintf(f1,"\n st64dig(3)");

	fprintf(f1,"\n ana %s %s",masksp[4],masksp[5]);
	if(vco_no == 4) fprintf(f1,"\n st32dig(2)");
	else fprintf(f1,"\n st32dig(3)");
        fprintf(f1,"\n dismcmq\n"); 
	}
    else
    {
        fprintf(f1,"\n enamcmq\n"); 
    fprintf(f1,"\n ana %s %s",masksp[2],masksp[3]);
    if(vco_no == 4) fprintf(f1,"\n st32dig(3)");
    else fprintf(f1,"\n st32dig(2)");

    fprintf(f1,"\n ana %s %s %s %s", mask[0], mask1[0], mask[1], mask1[1]);
    if(vco_no == 4) fprintf(f1,"\n st64dig(3)");
    else fprintf(f1,"\n st64dig(2)");

    /******/
    if(maxfrq > 600 && vco_no == 4)
    {
    fprintf(fa,"\n ana %s %s", maska[1], mask1a[1]);
    fprintf(fa,"\n st32dig(3)");
    fprintf(fp,"\n ana %s %s", maskp[1], mask1p[1]);
    fprintf(fp,"\n st32dig(3)");
    }
   /******/

    fprintf(f1,"\n ana %s %s %s %s", mask[2], mask1[2], mask[3], mask1[3]);
    if(vco_no == 4) fprintf(f1,"\n st64dig(3)");
    else fprintf(f1,"\n st64dig(2)");

    fprintf(f1,"\n ana %s %s", mask[4], mask1[4]);
    if(vco_no == 4) fprintf(f1,"\n st32dig(3)");
    else fprintf(f1,"\n st32dig(2)");
        fprintf(f1,"\n dismcmq\n"); 
    }

    if(cnt == 1) lofreq = lofreq2;

}

    /* fprintf(f1,"\n\n dismcmq\n"); */
    if(maxfrq > 600)
    {
    fprintf(fa,"\n\n dismcmq\n");
    fprintf(fp,"\n\n dismcmq\n");
    fclose(fa);
    fclose(fp);
    }
    fprintf(f1," enalo1mon; enalo2mon\n");
    fclose(f1);
    return 0;
    }
/************************************************************/

int set_bblo (void)
{
	char freq1[8],freq2[8];
	char mcm_no;
	int addr=25;

	FILE *setbb;
	int i, a1, a2, b1, b2, c1, c2, d1, d2, e1, e2, f1, f2, temp;
	for(i=0;i<=1;i++){
	    if(bblo[i] == 0 || bblo[i] < 50000000 || bblo[i] > 90000000) {
		error(11); 
		return 1;
	    }

	}

	switch(set_para[22]) {
		case 1:mcm_no = '6';break;
		case 2:mcm_no = '9';break;
		case 3:mcm_no = 'a';break;
		case 4:mcm_no = '5';break;
		defaut:mcm_no = '6';break;
	}

	temp = bblo[0]/100;
/*	printf("The value of temp is %d\n",temp);	*/
	f1 = (temp / 100000);
	temp = temp - f1 * 100000;
	e1 = temp/10000;
	temp = temp - e1 * 10000;
	d1 = temp/1000;
	temp = temp - d1 * 1000;
	c1 = temp/100;
	temp = temp - c1 * 100;
	b1 = temp/10;
	temp = temp - b1 * 10;
	a1 = temp/1;

	temp = bblo[1]/100;
	f2 = (temp / 100000);
	temp = temp - f2 * 100000;
	e2 = temp/10000;
	temp = temp - e2 * 10000;
	d2 = temp/1000;
	temp = temp - d2 * 1000;
	c2 = temp/100;
	temp = temp - c2 * 100;
	b2 = temp/10;
	temp = temp - b2 * 10;
	a2 = temp/1;

/* 	printf("f1=%d\n",f1); 
	printf("e1=%d\n",e1); 
	printf("d1=%d\n",d1); 
	printf("c1=%d\n",c1); 
	printf("b1=%d\n",b1); 
	printf("a1=%d\n\n",a1);

 	printf("f2=%d\n",f2); 
	printf("e2=%d\n",e2); 
	printf("d2=%d\n",d2); 
	printf("c2=%d\n",c2); 
	printf("b2=%d\n",b2); 
	printf("a2=%d\n",a2);	*/

	setbb = fopen("/home/observer/set/user/mcm4set/bblo.set","w");
	    if(setbb == NULL) {
	    perror("/home/observer/set/user/mcm4set/bblo.set");
	    exit(1);
	}

	sprintf(freq1,"%d%d.%d%d%d%d",f1,e1,d1,c1,b1,a1);
	sprintf(freq2,"%d%d.%d%d%d%d",f2,e2,d2,c2,b2,a2);

/*	printf("The First Synth Freq is %d%d.%d%d%d%d\n",f1,e1,d1,c1,b1,a1);
	printf("The Second Synth Freq is %d%d.%d%d%d%d\n",f2,e2,d2,c2,b2,a2);	*/
	

	fprintf(setbb, "* BASEBAND LO SETTING FILE\n\n");
	if(set_para[22] == 1 || set_para[22] == 3) fprintf(setbb, "* SETTING LO FOR CHAN #1 %s MHz\n",freq1);
	else fprintf(setbb, "* SETTING LO FOR CHAN #1 %s MHz\n",freq2);
	if(set_para[22] == 1 || set_para[22] == 4) fprintf(setbb, "* SETTING LO FOR CHAN #2 %s MHz\n\n",freq1);
	else fprintf(setbb, "* SETTING LO FOR CHAN #2 %s MHz\n\n",freq2);

	fprintf(setbb, "ante 1 %d; addcebmcm\n",addr);
	fprintf(setbb, "enacebq\n");
	fprintf(setbb, "ana 0%d%d%dx 1%d%d%dx 1%d%d%dx 2%d%d%dx\n",c1,b1,a1,c1,b1,a1,f1-5,e1,d1,f1-5,e1,d1);
	fprintf(setbb, "stceb64(%d)\n", addr);
	fprintf(setbb, "ana 2%d%d%dx 3%d%d%dx 3%d%d%dx 4%d%d%dx\n",c2,b2,a2,c2,b2,a2,f2-5,e2,d2,f2-5,e2,d2);
	fprintf(setbb, "stceb64(%d)\n", addr);
	fprintf(setbb, "ana 400%cx 500%cx 5000x 6000x\n",mcm_no,mcm_no);
	fprintf(setbb, "stceb64(%d)\n", addr);
	fprintf(setbb, "discebq\n");

	fclose(setbb);
    return(0);
}

/*float NewIF(int pratr1, int pratr2, float gain_ch1, float gain_ch2, int bw1, int bw2, int alc1, int alc2)*/
int NewIF()
{
	int i,j,bw,lsb1,lsb2,xgain_ch1,xgain_ch2,fbv_dec1,fbv_dec2,alc=0;
	int if_bw2[] = {0xc8, 0xd0, 0xe0};
	int if_bw1[] = {0x01, 0x02, 0x04};
	int ADDR=10; /* MCM adadr*/
        int pratr1,pratr2, bw1, bw2, alc1, alc2;
        float gain_ch1=post_gain[0], gain_ch2=post_gain[1];
	FILE *f1;
        pratr1=set_para[6]; pratr2=set_para[8]; bw1=set_para[10]; bw2=set_para[11]; alc1=set_para[12]; alc2=set_para[13];
//printf("\nif_bw1[0] = %x\nif_bw1[1] = %x\nif_bw1[2] = %x\n",if_bw1[0],if_bw1[1],if_bw1[2]);	

/*========================Error Checking of Different Parameters Entered START============================*/

	if( (pratr1%2 !=0) || (pratr2%2 !=0) || (pratr1<0 || pratr2<0 || pratr1>30 || pratr2>30) )
	{
	printf("Improper Attenuation in setup.txt\nENTER PROPER ATTENUATIONS\nRange :- 0 to 30 in multiples of 2\n");
	exit(1);
	}
	if(gain_ch1>35 || gain_ch1<-5)
	{
	printf("Set Gain of CH-1 in between -5 dB to 35 dB\n");
        exit(1);
	}
	else if(((gain_ch1-(int) gain_ch1)!=0.5)&&((gain_ch1-(int) gain_ch1)!=0.0))
        {
        printf("Set Gain of CH-1 in multiples of 0.5 dB\n");
        exit(1);
        }

	if(gain_ch2>35 || gain_ch2<-5) 
	{
	printf("Set Gain of CH-2 in between -5 dB to 35 dB\n");
	exit(1);
	}
	else if(((gain_ch2-(int) gain_ch2)!=0.5)&&((gain_ch2-(int) gain_ch2)!=0.0)) 
	{
	printf("Set Gain of CH-2 in multiples of 0.5 dB\n");
	exit(1);
	}

//	alc = alc1 | alc2<<1;
//printf("\nalc1 = %x\talc2 = %x\n",alc1,alc2);
	if(bw1==6) i=0; else if(bw1==16) i=1; else if(bw1==32) i=2; else { printf("Enter Proper Bandwidth (6,16,32 MHz) for CH-1\n"); exit(1);}
        if(bw2==6) j=0; else if(bw2==16) j=1; else if(bw2==32) j=2; else { printf("Enter Proper Bandwidth (6,16,32 MHz) for CH-2\n"); exit(1);}

	if(alc1>1 || alc1<0)
	{
	printf("Enter Proper Bit (0-OFF/ 1-ON) for CH-1 ALC Switching\n"); 
	exit (1);
	}
	else if(alc2>1 || alc2<0) 
		{
		printf("Enter Proper Bit (0-OFF/ 1-ON) for CH-2 ALC Switching\n");
		exit (1);
		}
	alc = alc1 | alc2<<1;

/*========================Error Checking of Different Parameters Entered END============================*/		

	f1 = fopen("/home/observer/set/user/mcm4set/if.set","w+t");
	fprintf(f1,"\n* Run file for setting IF parameters.\n");
	fprintf(f1, "\nmpa 1 %d\naddmcm\n\nenamcmq\n",ADDR);
//PRATR :-	
	fprintf(f1, "* PRE ATTENUATION CH-1 -> %d\tCH-2 -> %d \n",pratr1, pratr2);
	fprintf(f1, "ana 0B3%c%cx 0F3%c%cx 0B3%c%cx 0F3%c%cx\n", PratrCode(pratr2), PratrCode(pratr1), PratrCode(pratr2), PratrCode(pratr1), PratrCode(pratr2), PratrCode(pratr1), PratrCode(pratr2), PratrCode(pratr1));	
	fprintf(f1, "\nst64dig(%d)\n\n",ADDR);
	fprintf(f1,"* Post Gain CH-1 -> %8.3f dB\tCH-2 -> %8.3f dB \n", gain_ch1, gain_ch2);
//Gain :-	
	xgain_ch1 = 90 - (2*gain_ch1);
	xgain_ch2 = 90 - (2*gain_ch2);
	lsb1 = xgain_ch1 & 0x7;
	lsb2 = xgain_ch2 & 0x7;
	lsb1 = ((lsb2<<5) | (lsb1<<2));
	lsb1 = lsb1 | alc;
//printf("\nlsb1 = %x and alc = %x\n",lsb1,alc);	
	fprintf(f1, "ana 0B7%x%xx 0F7%x%xx 0BF%x%xx 0FF%x%xx\n",xgain_ch2>>3&0xf, xgain_ch1>>3&0xf, xgain_ch2>>3&0xf, xgain_ch1>>3&0xf, (lsb1>>4)&0xf,lsb1&0xf,(lsb1>>4)&0xf, lsb1&0xf );
	fprintf(f1, "\nst64dig(%d)\n",ADDR);
//IFBW :-
	bw = if_bw1[i]+if_bw2[j];
		
	fprintf(f1,"\n* BW in CH1 & CH2 are, %d & %d resp.\n", bw1, bw2);
        fprintf(f1,"\nana 0FB%02xx 0FB%02xx 0BB%02xx 0FB%02xx\nst64dig(%d)\n\ndismcmq\n",bw,bw,bw,bw,ADDR);
/*	printf("\nSUCESSFULLY CREATED IF.SET FILE\n");*/
	return(0);     

}

char PratrCode(int g)
{
	switch (g)
    	{
	     case 0  :{return '4';}
	     case 2  :{return '5';}
	     case 4  :{return '6';}
	     case 6  :{return '7';}
	     case 8  :{return '0';}
	     case 10 :{return '1';}
	     case 12 :{return '2';}
	     case 14 :{return '3';}
	     case 16 :{return 'C';}
	     case 18 :{return 'D';}
	     case 20 :{return 'E';}
	     case 22 :{return 'F';}
	     case 24 :{return '8';}
	     case 26 :{return '9';}
	     case 28 :{return 'A';}
	     case 30 :{return 'B';}
	     default :{printf("Enter proper attenuation value:");exit(1); }
	}
}
