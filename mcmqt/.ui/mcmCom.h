/****************************************************************************
** Form interface generated from reading ui file 'mcmCom.ui'
**
** Created by User Interface Compiler
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef MCMCOM_H
#define MCMCOM_H

#include <qvariant.h>
#include <qmainwindow.h>
#include "decode.h"
#include "exec.h"
#include "cfile.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class QLabel;
class QPushButton;
class QGroupBox;
class QLineEdit;
class QFrame;
class QCheckBox;
class QTextEdit;
class QTabWidget;
class QWidget;
class QListBox;
class QListBoxItem;
class QComboBox;

class mcmCOM : public QMainWindow
{
    Q_OBJECT

public:
    mcmCOM( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~mcmCOM();

    QLabel* Title;
    QPushButton* ExitButton;
    QGroupBox* Connection_box;
    QPushButton* OpenButton;
    QLineEdit* PortEdit;
    QPushButton* CloseButton;
    QGroupBox* PollingBox;
    QLineEdit* PollEdit;
    QPushButton* PollButton;
    QGroupBox* ReadMcm;
    QLabel* CmdReceived;
    QLabel* AnlMask;
    QLabel* DigMask;
    QLabel* ScanData;
    QLabel* CmdStatus;
    QFrame* frame10;
    QLabel* Mode;
    QLineEdit* McmAddr2Edit;
    QLineEdit* ModEdit;
    QLabel* McmAddr2;
    QLineEdit* VersEdit;
    QLabel* Version;
    QPushButton* ClearButton;
    QCheckBox* DisplayButton;
    QTextEdit* ScanDataEdit;
    QTextEdit* DDEdit;
    QLineEdit* CmdSttusEdit;
    QLineEdit* CmdRxEdit;
    QLineEdit* CountEdit;
    QLabel* Counter;
    QLabel* AvgFact;
    QLineEdit* AvgfactEdit;
    QLineEdit* DigMaskEdit;
    QLineEdit* AnMaskEdit;
    QTabWidget* CmdTab;
    QWidget* tab;
    QListBox* CmdList;
    QPushButton* InsertButton;
    QPushButton* RemoveButton;
    QPushButton* ClearListButton;
    QLabel* MaskTitle;
    QLineEdit* Argument1;
    QPushButton* WriteButton;
    QPushButton* StopButton;
    QCheckBox* RepeatButton;
    QLabel* delaylabel;
    QLineEdit* DelayEdit1;
    QListBox* CmdAvaiList;
    QLabel* McmAddr;
    QLineEdit* McmAddrEdit;
    QWidget* tab_2;
    QGroupBox* groupBox3;
    QLabel* FileRdLabel;
    QLineEdit* FileRdEdit;
    QLabel* FileWrLabel;
    QLineEdit* FileWrEdit;
    QPushButton* EnterButton;
    QPushButton* StopButton2;
    QComboBox* ComboCmd;
    QGroupBox* groupBox4;
    QLabel* AnlMaskLabel;
    QLineEdit* AnMaskEdit2;
    QLineEdit* DelayEdit;
    QLabel* DelayLabel;
    QLineEdit* McmAddrEdit2;
    QLabel* McmAddrLabel;

public slots:
    virtual void GetCommand();
    virtual void RmCommand();
    virtual void ClearList();
    virtual void DisplayAll( Decode * dec );
    virtual void ClearAns();
    virtual void SerialConnection();
    virtual void SerialDisconnection();
    virtual void McmPoll();
    virtual void WriteBuf();
    virtual void StopCmds();
    virtual void Exitfinal();
    virtual void ProposeArg();
    virtual void AskCmdListArg();
    virtual void ExecFileCmd();

protected:

protected slots:
    virtual void languageChange();

private:
    int stopped;
    Connection SerialCon;
    int CmdIndx;
    Command* CommandArray[20];
    ComExec Onecom;
    Decode Onedecode;

    virtual void setCmdArgument( Command * CmdTx );
    virtual void DisplayRes( Decode * dec, int delay );
    virtual void ErrorMessage();
    virtual int OneTxRx( Command Cmd, int delay );
    virtual int FofQt( char * filerd, char * filewrt, int delay, char AnlMask[8] );
    virtual void DataMonQt( char * filewrt, int delay, char * Arguments );
    virtual int FileData( char * filerd, int delay );

};

#endif // MCMCOM_H
