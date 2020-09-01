/***************************************************
 * shell.h
 * header file for Linux / MCM communication
 * shell terminal specific classes
 **************************************************/

/* Class for Shell Terminal Command inputs */
class ShellInput : public CommandPlus {
  public :
    void GetCmd(void);
  private :
    void SetCmdArgument(void);
    void Fofinputs(void);
    void Datmoninputs(void);
    void Filedatinputs(void);
};

/* Class for Shell Terminal MCM Responses display */
class RxToShell : public Decode {
  public :
    void PrintRes(void);
};

/* Class for File OF File pre-composed command */
class FileofFile {
  public :
    FileofFile(char *filerd, char *filewrt, int delay0, unsigned char AnlMask0[8], FT_HANDLE ftHandle);
    ~FileofFile(void);
    int Fof(void);
    Decode Fofdec;
  private :
    Filebase *filecmd, *fileres;
    ComExec Fofcom;
    unsigned char AnlMask[8];
    int delay;
    int serialfd;
    void OneTxRx(Command Cmd);
};

/* Class for Data Monitoring pre-composed command */
class DataMonitoring {
  public :
    DataMonitoring(char *filewrt, int delay0, unsigned char AnlMask0[8],FT_HANDLE	ftHandle, unsigned char McmAddr, int iter0);
    ~DataMonitoring(void);
    void DataMon(void);
    Decode DataMondec;
  private :
    ComExec DataMoncom;
    Filebase *fileres;
    int delay, serialfd, iter;
    void OneTxRx(Command Cmd);
};

/* Class for File Data pre-composed command */
class FileData {
  public :
    FileData(char *filerd, int delay0,FT_HANDLE ftHandle);
    ~FileData(void);
    int FData(void);
    Decode FDatdec;
  private :
    Filebase *filecmd;
    ComExec FDatcom;
    int delay;
    int serialfd;
    void OneTxRx(Command Cmd);
};


