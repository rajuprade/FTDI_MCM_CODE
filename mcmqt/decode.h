/***************************************************
 * decode.h
 * header file for Linux / MCM communication
 * objects to receive and decode packets from MCM
 **************************************************/

#include "cfile.h"
/* Command class with file arguments */
class CommandPlus : public Command {
  public :
    char fileread[30];
    char filewrt[30];
    int delay, Avgfact, iter, FEbox;
    char McmAddr;
    void SetCmdName(void);
};  

/* Class to decode MCM answer packets and to store their data */
class Decode {
  public :
    Decode(void);
    ~Decode(void);
    char *CmdStatus;
    CommandPlus CmdRx;
    int Mode;
    char *Version;
    char *Avgfact;
    char McmAddr;
    int NbofChannels;
    unsigned char anlmask[8];
    unsigned char scandata[64];
    int ReadBuffer(int fd);
  private :
    char* buf_ptr0;
    void DecodeBuffer(void);
    void DecodeFirstPacket(void);
    void DecodeSecondPacket(char *SecPackStart);      
    void RecordFirstPack(char *ptr, int recordsiz);
    void SetProcess(char *SecPackStart);
    void ReadProcess(char *SecPackStart);
    void BoxProcess(char *SecPackStart, int siz);
};
    


