/*******************************************************
 * cfile.h
 * header file for Linux / MCM communication
 * common constant definitions
 * basic objects and objects for file based commmands
 ******************************************************/

#ifndef H_CFILE_H
#define H_CFILE_H

#define NB_CMD 21  //total nb of commands
#define IDLE_MODE 1
#define SCAN_MODE 2
#define MEAN_MODE 3
#define LIMITS_MODE 4


/* basic Command Class */
class Command {
  public :
    int CmdNum;
    char CmdName[30];
    unsigned char Argument[64];
};

/* chained list of Commands */
class CommandList : public Command {
  public :
    CommandList(void);
    ~CommandList(void);
    CommandList *NextCmd;
};

/* file relative class */
class Filebase {
  public :
    Filebase(char *filename);
    ~Filebase(void);
    void WriteHeader1(int delay, unsigned char *AnlMask);
    void WriteVal1(unsigned char digmask[2], char Mcmaddr, unsigned char Arg[], int NbofChannels);
    void WriteHeader2(int delay, unsigned char *AnlMask, char McmAddr);
    void WriteVal2(unsigned char Arg[], int NbofChannels);
    int ReadCmdParam1(CommandList **CurrentCmdptr);
    int ReadCmdParam2(CommandList **CurrentCmdptr);
  private :
    FILE *fd;
    void Convert(int offset, unsigned char *conv1, unsigned char *conv2, char line[21]);
};

#endif
