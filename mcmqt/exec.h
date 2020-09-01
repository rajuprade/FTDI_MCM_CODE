/****************************************************
 * com_exec.h
 * header file for Linux / MCM communication
 * objects to prepare and send command packets to MCM
 ****************************************************/

/* Class for Serial Port Connection */
class Connection {
  public :
    Connection(void);
    int fd, SerialPortNb;
    void Connect(void);
    void Disconnect(void);
};

/* Class to prepare and send command packets to MCM */
class ComExec {
  public :
    ComExec(void);
    ~ComExec(void);
    char McmAddr;      
    int WriteBuffer(int fd, Command Cmd);
    void set_mcm_addr(unsigned char Arg[]);
    
  private :
    char *buf_ptr0;
    void MakeBuffer(Command Cmd); 
    void AddCksum(char *buff);
    void InitBuff(char buff[]);
    void set_anl_mask(unsigned char Arg[]);
    void set_dig_mask16(unsigned char Arg[]);
    void set_dig_mask32(unsigned char Arg[]);
    void set_dig_mask64(unsigned char Arg[]);
    void set_mean_mode(unsigned char Arg[]);
    void feed_cntrl_old(unsigned char Arg[]);
    void feed_cntrl_new(unsigned char Arg[]);
    void fe_box_mon(unsigned char Arg[]);
};

