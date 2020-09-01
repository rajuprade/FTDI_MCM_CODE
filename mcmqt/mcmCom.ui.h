/****************************************************************************
** mcmCom.ui.h
** extension file, QT specific for mcmCom.ui form
** included from the uic-generated form implementation.
*****************************************************************************/

#include <qstatusbar.h>
#include <qapplication.h>
#include <unistd.h> //sleep


/*-----------------------------------------------*/
/* Commands relative functions    */
/*-----------------------------------------------*/

/******* Get Command function***********
 * get command selected
 * call setCmdArgument                    */
void mcmCOM::GetCommand()
{
    if (SerialCon.fd < 0) SerialConnection(); 
    CommandArray[CmdIndx] = new Command;
    CommandArray[CmdIndx]->CmdNum = CmdAvaiList->currentItem();
    setCmdArgument(CommandArray[CmdIndx]); 
    CmdList->clearSelection();
    CmdAvaiList->clearSelection();
    CmdIndx++;
    McmAddrEdit->setDisabled(TRUE);
    Argument1->setDisabled(TRUE);
    statusBar()->clear();
}

/******* Set Cmd Argument function***********
 * get command Arguments from inputs      */
void mcmCOM::setCmdArgument(Command *CmdTx)
{
    int i;
    
    if (CmdTx->CmdNum == 0) {
	CmdTx->Argument[0] = (char) McmAddrEdit->text().toShort(0,10);
	CmdList->insertItem(QString("%1 - %2 ").arg(CmdAvaiList->currentText()).arg(CmdTx->Argument[0], 0, 16),-1);	
    }
    else if ((CmdTx->CmdNum == 4) || (CmdTx->CmdNum == 18)) {
	CmdTx->Argument[0] = (char) Argument1->text().toShort(0,10);
	CmdList->insertItem(QString("%1 - %2 ").arg(CmdAvaiList->currentText()).arg(CmdTx->Argument[0], 0, 10),-1);	
    }
     else if (CmdTx->CmdNum == 16) {
	CmdTx->Argument[0] = (char) Argument1->text().toShort(0,16);
	CmdList->insertItem(QString("%1 - %2 ").arg(CmdAvaiList->currentText()).arg(CmdTx->Argument[0], 0, 16),-1);	
    }
    else if ((CmdTx->CmdNum == 6)||(CmdTx->CmdNum == 17)) {
	QString s = Argument1->text();
	for (i=0;i<2;i++) CmdTx->Argument[i] = (unsigned char) s.section(' ',i,i).toUShort(0,16);
	CmdList->insertItem(QString("%1 - %2 %3").arg(CmdAvaiList->currentText()).arg(CmdTx->Argument[0], 0, 16).arg(CmdTx->Argument[1], 0, 16),-1);
    }
    else if ((CmdTx->CmdNum == 7)) {
	QString s = Argument1->text();
	for (i=0;i<4;i++) CmdTx->Argument[i] = (unsigned char) s.section(' ',i,i).toUShort(0,16);
	CmdList->insertItem(QString("%1 - %2 %3 %4 %5").arg(CmdAvaiList->currentText()).arg(CmdTx->Argument[0], 0, 16).arg(CmdTx->Argument[1], 0, 16).arg(CmdTx->Argument[2], 0, 16).arg(CmdTx->Argument[3], 0, 16),-1);
    }
    else if (CmdTx->CmdNum == 5) {
	QString s = Argument1->text();
	for (i=0;i<8;i++) CmdTx->Argument[i] = (unsigned char) s.section(' ',i,i).toUShort(0,16);
	CmdList->insertItem(QString("%1 - %2 %3 %4 %5 %6 %7 %8 %9").arg(CmdAvaiList->currentText()).arg(CmdTx->Argument[0], 0, 16).arg(CmdTx->Argument[1], 0, 16).arg(CmdTx->Argument[2], 0, 16).arg(CmdTx->Argument[3], 0, 16).arg(CmdTx->Argument[4], 0, 16).arg(CmdTx->Argument[5], 0, 16).arg(CmdTx->Argument[6], 0, 16).arg(CmdTx->Argument[7], 0, 16),-1);
    }
    else CmdList->insertItem(QString("%1 ").arg(CmdAvaiList->currentText()),-1);
    Argument1->clear();
}

/******* Remove Cmd function******************
 * remove selected Cmd from Cmd List      */
void mcmCOM::RmCommand()
{
    int i, j;    
    
    if (CmdIndx > 0) {	
	i = CmdList->currentItem();
	if (i<0) return;
	CmdList->removeItem(i);	
	CmdList->clearSelection();
	CmdIndx--;
	for (j=i;j<CmdIndx;j++) *CommandArray[j] = *CommandArray[j+1];
	delete CommandArray[CmdIndx];	
    }    
}

/******* Clear List function******************
 * clear Cmd Array                              */
void mcmCOM::ClearList()
{
    int i;
    
    if (SerialCon.fd < 0) SerialConnection(); //otherwise CmdIndx undeclared->seg fault
    for (i=0;i<CmdIndx;i++) {
	delete CommandArray[i]; 
	CmdList->removeItem(0);	
    }
    CmdIndx = 0;
}

/*-----------------------------------------------*/
/* Answers relative functions        */
/*-----------------------------------------------*/

/******* Display Response function************************
 * display MCM response data from decode object    */
void mcmCOM::DisplayRes(Decode *dec, int delay)
{
    int i;
    QString s;
    
    usleep(delay*1000);
    McmAddr2Edit->setText(s.setNum((dec->McmAddr), 10));
    switch (dec->Mode) {
	case IDLE_MODE : ModEdit->setText("Idle Mode"); break;
	case SCAN_MODE : ModEdit->setText("Scan Mode"); break;
	case MEAN_MODE : ModEdit->setText("Mean Mode"); break;
	case LIMITS_MODE : ModEdit->setText("Limits Mode");
	}
    CmdSttusEdit->setText(dec->CmdStatus);     
    CmdRxEdit->setText(dec->CmdRx.CmdName);
    
    if (dec->Mode != IDLE_MODE) {	
	AnMaskEdit->clear();
	for (i=0;i<8;i++) AnMaskEdit->insert(QString(" %1").arg(dec->anlmask[i], 0, 16));
	ScanDataEdit->clear();	
	for (i=0;i<dec->NbofChannels;i++) ScanDataEdit->insert(QString(" %1").arg(dec->scandata[i], 0, 16));
	if (DisplayButton->isChecked()) DisplayAll(dec);
    }
    if (dec->Mode == MEAN_MODE) AvgfactEdit->setText(s.setNum(*dec->Avgfact, 10));    
    else  AvgfactEdit->clear(); 
    
    if (dec->CmdRx.CmdNum == 13) VersEdit->setText(dec->Version);    
    else if ((dec->CmdRx.CmdNum == 6) || (dec->CmdRx.CmdNum == 10) || (dec->CmdRx.CmdNum == 17)) {
	DigMaskEdit->clear();
	for (i=0;i<2;i++) DigMaskEdit->insert(QString(" %1").arg(dec->CmdRx.Argument[i], 0, 16));
    }
    else if ((dec->CmdRx.CmdNum == 16)) {
	DigMaskEdit->clear();
	DigMaskEdit->insert(QString(" %1").arg(dec->CmdRx.Argument[0], 0, 16));
    }
    else if ((dec->CmdRx.CmdNum == 7) || (dec->CmdRx.CmdNum == 11)) {
	DigMaskEdit->clear();
	for (i=0;i<4;i++) DigMaskEdit->insert(QString(" %1").arg(dec->CmdRx.Argument[i], 0, 16));
    }
    else if ((dec->CmdRx.CmdNum == 8) || (dec->CmdRx.CmdNum == 12)) {
	DigMaskEdit->clear();
	for (i=0;i<8;i++) DigMaskEdit->insert(QString(" %1").arg(dec->CmdRx.Argument[i], 0, 16));
    }
    else if ((dec->CmdRx.CmdNum == 18) || (dec->CmdRx.CmdNum == 19)) {
	ScanDataEdit->clear();
	for (i=0;i<dec->NbofChannels;i++) ScanDataEdit->insert(QString(" %1").arg(dec->CmdRx.Argument[i], 0, 16));
    } 
    else if (dec->CmdRx.CmdNum == 15) {DigMaskEdit->clear();ScanDataEdit->clear();AnMaskEdit->clear();}
}

void mcmCOM::DisplayAll(Decode *dec)
{
    int i, j=0;
    DDEdit->clear();
    for (i=0;i<8;i++) {
	if (dec->anlmask[i] & 0x01) {DDEdit->insert(QString("%1 ").arg(dec->scandata[j], 2, 16));j++;}
	else DDEdit->insert(QString("xx "));
	if (dec->anlmask[i] & 0x02) {DDEdit->insert(QString("%1 ").arg(dec->scandata[j], 2, 16));j++;}
	else DDEdit->insert(QString("xx "));
	if (dec->anlmask[i] & 0x04) {DDEdit->insert(QString("%1 ").arg(dec->scandata[j], 2, 16));j++;}
	else DDEdit->insert(QString("xx "));
	if (dec->anlmask[i] & 0x08) {DDEdit->insert(QString("%1 ").arg(dec->scandata[j], 2, 16));j++;}
	else DDEdit->insert(QString("xx "));
	if (dec->anlmask[i] & 0x10) {DDEdit->insert(QString("%1 ").arg(dec->scandata[j], 2, 16));j++;}
	else DDEdit->insert(QString("xx "));
	if (dec->anlmask[i] & 0x20) {DDEdit->insert(QString("%1 ").arg(dec->scandata[j], 2, 16));j++;}
	else DDEdit->insert(QString("xx "));
	if (dec->anlmask[i] & 0x40) {DDEdit->insert(QString("%1 ").arg(dec->scandata[j], 2, 16));j++;}
	else DDEdit->insert(QString("xx "));
	if (dec->anlmask[i] & 0x80) {DDEdit->insert(QString("%1 ").arg(dec->scandata[j], 2, 16));j++;}
	else DDEdit->insert(QString("xx "));
    }
}

void mcmCOM::ClearAns()
{
    DDEdit->clear();    ScanDataEdit->clear();
    AnMaskEdit->clear();    DigMaskEdit->clear();
    CmdRxEdit->clear();    CmdSttusEdit->clear();
    VersEdit->clear();    ModEdit->clear();
    McmAddr2Edit->clear();    CountEdit->clear();
    AvgfactEdit->clear();
}


/*--------------------------------------------------*/
/* Serial Connection / Poll functions */
/*--------------------------------------------------*/

/******* Serial Connection************************************
 * open serial port   */
void mcmCOM::SerialConnection()
{
    SerialCon.SerialPortNb = PortEdit->text().toShort(0,10);
    SerialCon.Connect(); 
    statusBar()->message(QString("Serial Port %1 open.").arg(SerialCon.SerialPortNb, 0, 10));
    CmdIndx = 0;
    PortEdit->setDisabled(TRUE);
    OpenButton->setDisabled(TRUE);
    CloseButton->setEnabled(TRUE);
}

/******* Serial Disconnection************************************
 * close serial port   */
void mcmCOM::SerialDisconnection()
{
    SerialCon.Disconnect();
    ClearList();
    SerialCon.fd = -1;
    statusBar()->message(QString("Serial Port %1 close.").arg(SerialCon.SerialPortNb, 0, 10));
    PortEdit->setEnabled(TRUE);
    OpenButton->setEnabled(TRUE);
    CloseButton->setDisabled(TRUE);
}

/******* MCM Polling**********************************************
 * give MCM present on the RS485 link
 * polling limited to {0, 2, 3, 5, 9, 14, 15} MCM Addresses */
void mcmCOM::McmPoll()
{
  int j;
  Command NewCmd;
  int McmAddrArr[7] = {0, 2, 3, 5, 9, 14, 15};

  if (SerialCon.fd < 0) SerialConnection();
  statusBar()->message("MCM polling in process, please wait...");
  PollButton->setDisabled(TRUE);
  PollEdit->clear();
  for (j=0;j<7;j++) {
    qApp->processEvents();
    Onecom.set_mcm_addr((unsigned char *) &McmAddrArr[j]);    //set mcm addr
    NewCmd.CmdNum = 1;                            //Null Cmd
    Onecom.WriteBuffer(SerialCon.fd, NewCmd);
    if (Onedecode.ReadBuffer(SerialCon.fd) > 1) PollEdit->insert(QString(" %1").arg(McmAddrArr[j], 0, 10));
  }
  statusBar()->message("MCM polling done.");
  PollButton->setEnabled(TRUE);
}


/*--------------------------------------------------*/
/* Communication relative functions */
/*--------------------------------------------------*/

void mcmCOM::ErrorMessage()
{
    statusBar()->message("Serial Port writing Error, ...maybe a wrong driver installation?");
}

/******* Write Command Buffer **********************************
 * execute Commands selected in basic cmd array
 * call OneCom and OneDecode objet functions
 * call DisplayRes function                                              */
void mcmCOM::WriteBuf()
{
    int i,j=0, errorflag=0, delay;
    QString s;
    
    stopped = 0;
    delay = DelayEdit1->text().toShort(0,10);
    WriteButton->setDisabled(TRUE);
    RepeatButton->setDisabled(TRUE);
    StopButton->setEnabled(TRUE);
    DelayEdit1->setDisabled(TRUE);
    statusBar()->message("MCM communication in process...");
    qApp->processEvents();
    while (!stopped) {
	for (i=0;i<CmdIndx;i++) {
	    if (CommandArray[i]->CmdNum == 18) return;
	    else if (CommandArray[i]->CmdNum == 0) Onecom.set_mcm_addr(CommandArray[i]->Argument);
	    else {
		if (Onecom.WriteBuffer(SerialCon.fd, *CommandArray[i])<=0) 
		{ErrorMessage();errorflag=1;break;}
		else if (Onedecode.ReadBuffer(SerialCon.fd)<=0) 
		{ErrorMessage();errorflag=1;break;}
		else {DisplayRes(&Onedecode, delay); CountEdit->setText(s.setNum(++j, 10));}	
	    }	    	    
	}
	if (!RepeatButton->isChecked()) break;
	qApp->processEvents();
    }
    WriteButton->setEnabled(TRUE);
    StopButton->setDisabled(TRUE);
    RepeatButton->setEnabled(TRUE);
    DelayEdit1->setEnabled(TRUE);
    if(!errorflag) statusBar()->clear();
}

/******* Stop Cmds ********************************************
 * interrupts Cmd repetitions when RepeatButton is set   */
void mcmCOM::StopCmds()
{
    stopped = 1;
    RepeatButton->setChecked(FALSE);
}

void mcmCOM::Exitfinal()
{
    int i;
    
    stopped = 1;
    if (SerialCon.fd > 0) {
	SerialCon.Disconnect();
	for (i=0;i<CmdIndx;i++) delete CommandArray[i];
    }
    close();
}

/******* Propose Cmd Arguments ********************************************
 * display message for cmd arguments expected   */
void mcmCOM::ProposeArg()
{    
    McmAddrEdit->setDisabled(TRUE);
    Argument1->setDisabled(TRUE);
    if (CmdAvaiList->currentItem() == 0) {
	statusBar()->message("Enter MCM Address (base 16).");
	McmAddrEdit->setEnabled(TRUE);
    }
    else if ((CmdAvaiList->currentItem() == 5) || (CmdAvaiList->currentItem() == 8)) {
	Argument1->setEnabled(TRUE);
	statusBar()->message("Enter 8-bytes Argument (xx xx xx xx xx xx xx xx).");
    }
    else if (CmdAvaiList->currentItem() == 6) {
	Argument1->setEnabled(TRUE);
	statusBar()->message("Enter 2-bytes Argument (xx xx).");
    }
    else if (CmdAvaiList->currentItem() == 17) {
	Argument1->setEnabled(TRUE);
	statusBar()->message("Enter 1-byte data and 1-byte address (xx xx).");
    }
    else if (CmdAvaiList->currentItem() == 16) {
	Argument1->setEnabled(TRUE);
	statusBar()->message("Enter 1-byte data (xx).");
    }
    else if (CmdAvaiList->currentItem() == 7) {
	Argument1->setEnabled(TRUE);
	statusBar()->message("Enter 4-bytes Argument (xx xx xx xx).");
    }
    else if (CmdAvaiList->currentItem() == 18) {
	statusBar()->message("Enter FE-box Number (integer between 1 and 6).");
	Argument1->setEnabled(TRUE);
    }
    else if (CmdAvaiList->currentItem() == 4) {
	statusBar()->message("Enter Averaging factor (base 10, between 1 and 150).");
	Argument1->setEnabled(TRUE);
    }
    else statusBar()->clear();
}

/*-------------------------------------------------------*/
/* Pre-composed Command functions   */
/*-------------------------------------------------------*/

/******* OneTxRx function ****************************************
 * execute 1 Command, used in pre-combined functions
 * call OneCom and OneDecode objet functions
 * call DisplayRes function                                              */
int mcmCOM::OneTxRx(Command Cmd, int delay)
{
    Onecom.WriteBuffer(SerialCon.fd, Cmd);
    if (Onedecode.ReadBuffer(SerialCon.fd)<=0) {ErrorMessage();return 0;}
    else DisplayRes(&Onedecode, delay);     
    return 1;
}

/******* File Of File function************************************
 * execute the File Of File set of basic commands   */
int mcmCOM::FofQt(char *filerd, char *filewrt, int delay, char AnlMask[8])
{
  int j, cmdok;
  CommandList *CurrentCmd, *FirstCmd;
  Command NewCmd;
  Filebase *filecmd, *fileres;
  
  statusBar()->message("File of File in process...");
  EnterButton->setDisabled(TRUE);
  DisplayButton->setDisabled(TRUE);
  filecmd = new Filebase(filerd);
  fileres = new Filebase(filewrt); 
  CurrentCmd = new CommandList; 
  FirstCmd = CurrentCmd;
  fileres->WriteHeader1(delay, (unsigned char *)AnlMask); 
  cmdok = filecmd->ReadCmdParam1(&CurrentCmd);
  //Reboot, otherwise some cmd can be truncated according to MCM previous state
  NewCmd.CmdNum = 15;
  OneTxRx(NewCmd, delay);
  
  if (cmdok) {
      do {
	  if (CurrentCmd->CmdNum == 0) { 
	      Onecom.set_mcm_addr(CurrentCmd->Argument);    //set mcm addr 
	      NewCmd.CmdNum = 15;                   //Reboot, otherwise truncating can occur
	      OneTxRx(NewCmd, delay);
	      CurrentCmd = CurrentCmd->NextCmd;        
	      NewCmd.CmdNum = 5;                            //set Anl mask
	      for (j=0;j<8;j++) {NewCmd.Argument[j] = AnlMask[j];}
	      OneTxRx(NewCmd, delay);
	      NewCmd.CmdNum = 3;                            //set Scan Mode
	      OneTxRx(NewCmd, delay);
	  }
	  OneTxRx(*CurrentCmd, delay);      //set dig mask 16b
	  NewCmd.CmdNum = 1;                              //null Cmd
	  if (OneTxRx(NewCmd, delay)) 
	      fileres->WriteVal1(CurrentCmd->Argument, Onedecode.McmAddr, Onedecode.scandata, Onedecode.NbofChannels);
	  CurrentCmd = CurrentCmd->NextCmd;
	  qApp->processEvents();
      }
      while (CurrentCmd->NextCmd != 0);
  }
  EnterButton->setEnabled(TRUE);
  DisplayButton->setEnabled(TRUE);
  CurrentCmd = FirstCmd; //to delete all cmds from the 1srt
  delete CurrentCmd; // creates seg fault if ReadCmdParam(&CurrentCmd) returns for timeout, ??
  delete filecmd;
  delete fileres;
  if (cmdok) return 1;
  else return 0;
}

/******* Data Monitoring function************************************
 * execute the Data Monitoring set of basic commands   */
void mcmCOM::DataMonQt( char *filewrt, int delay, char *Arguments)
{
  int j;
  Command NewCmd;
  Filebase *fileres;
      
  statusBar()->message("Data Monitoring in process...");
  EnterButton->setDisabled(TRUE);
  StopButton2->setEnabled(TRUE);
  fileres = new Filebase(filewrt);
  fileres->WriteHeader2(delay, (unsigned char*)Arguments, Arguments[9]);
    
  Onecom.set_mcm_addr((unsigned char *)&Arguments[9]);    //set mcm addr   
  NewCmd.CmdNum = 15;                   //Reboot, otherwise truncating can occur
  OneTxRx(NewCmd, delay);
  NewCmd.CmdNum = 3;                            //set Scan Mode
  OneTxRx(NewCmd, delay);
  usleep(500000);
  NewCmd.CmdNum = 5;                            //set Anl mask
  for (j=0;j<8;j++) NewCmd.Argument[j] = Arguments[j];
  OneTxRx(NewCmd, delay);
  usleep(500000);  
  
  while (!stopped) {
    NewCmd.CmdNum = 1;                              //null Cmd
    if (OneTxRx(NewCmd, delay))
	fileres->WriteVal2(Onedecode.scandata, Onedecode.NbofChannels);
    qApp->processEvents();
  }
  EnterButton->setEnabled(TRUE);
  StopButton2->setDisabled(TRUE);
  delete fileres; 
}

/******* File Data function************************************
 * execute the File Data set of basic commands   */
int mcmCOM::FileData(char *filerd, int delay)
{
    int cmdok=0;
    CommandList *CurrentCmd, *FirstCmd; 
    Filebase *filecmd;
    
    statusBar()->message("File Data in process...");
    EnterButton->setDisabled(TRUE);
    DisplayButton->setDisabled(TRUE);
    filecmd = new Filebase(filerd);
    CurrentCmd = new CommandList;
    FirstCmd = CurrentCmd;
    cmdok = filecmd->ReadCmdParam2(&CurrentCmd);
    if (cmdok) {
	do {
	    if (CurrentCmd->CmdNum == 0) { 		
		Onecom.set_mcm_addr(CurrentCmd->Argument);    //set mcm addr  
		CurrentCmd = CurrentCmd->NextCmd; 
	    }
	    OneTxRx(*CurrentCmd, delay);                      //set dig mask 16b/32b/64b
	    CurrentCmd = CurrentCmd->NextCmd;
	    qApp->processEvents();
	}
	while (CurrentCmd->NextCmd != 0);
    }
    EnterButton->setEnabled(TRUE);
    DisplayButton->setEnabled(TRUE);
    CurrentCmd = FirstCmd; //to delete all cmds from the 1srt
    delete CurrentCmd; // creates seg fault if ReadCmdParam(&CurrentCmd) returns for timeout, ??
    delete filecmd;
    if (cmdok) return 1;
    else return 0;	
}

void mcmCOM::AskCmdListArg()
{
    if (ComboCmd->currentItem() == 0) {
	FileRdEdit->setEnabled(TRUE);
	FileWrEdit->setEnabled(TRUE);
	AnMaskEdit2->setEnabled(TRUE);
	McmAddrEdit2->setDisabled(TRUE);
	statusBar()->message("Enter parameters. (files used from current directory)");
    }
    else if (ComboCmd->currentItem() == 1) {
	FileRdEdit->setDisabled(TRUE);
	FileWrEdit->setEnabled(TRUE);
	AnMaskEdit2->setEnabled(TRUE);
	McmAddrEdit2->setEnabled(TRUE);
	statusBar()->message("Enter parameters. (files used from current directory)");
    }
    else if (ComboCmd->currentItem() == 2) {
	FileRdEdit->setEnabled(TRUE);
	FileWrEdit->setDisabled(TRUE);
	AnMaskEdit2->setDisabled(TRUE);
	McmAddrEdit2->setDisabled(TRUE);
	statusBar()->message("Enter parameters. (files used from current directory)");
    }
}

/******* Execute Pre-composed Command***********************************
 * read the command inputs
 * call the pre-composed command function                                       */
void mcmCOM::ExecFileCmd()
{
    int i;
    CommandPlus Cmd;    
    
    if (SerialCon.fd < 0) SerialConnection();
    DisplayButton->setChecked(TRUE);
    if (ComboCmd->currentItem() == 0) {
	QString s = AnMaskEdit2->text();
	if (!FileRdEdit->text().isEmpty()) sprintf(Cmd.fileread, FileRdEdit->text());
	else {AskCmdListArg(); return;}
	if (!FileWrEdit->text().isEmpty()) sprintf(Cmd.filewrt, FileWrEdit->text());
	else {AskCmdListArg(); return;}
	Cmd.delay = DelayEdit->text().toShort(0,10);
	for (i=0;i<8;i++) Cmd.Argument[i] = (unsigned char) s.section(' ',i,i).toUShort(0,16);	
	if (!FofQt(Cmd.fileread, Cmd.filewrt, Cmd.delay, (char *)Cmd.Argument)) 
	  {statusBar()->message("Wrong file command.");return;}
	statusBar()->message("File of File done.");
    }
    else if (ComboCmd->currentItem() == 1) {
	QString s = AnMaskEdit2->text();
	if (!FileWrEdit->text().isEmpty()) sprintf(Cmd.filewrt, FileWrEdit->text());
	else {AskCmdListArg(); return;}
	Cmd.delay = DelayEdit->text().toShort(0,10);
	for (i=0;i<8;i++) Cmd.Argument[i] = (unsigned char) s.section(' ',i,i).toUShort(0,16);
	Cmd.Argument[9] = (char) McmAddrEdit2->text().toShort(0,10);
		
	stopped = 0;
	DataMonQt(Cmd.filewrt, Cmd.delay, (char *)Cmd.Argument);
	statusBar()->message("Data Monitoring Done.");
    }
    else if (ComboCmd->currentItem() == 2) {
	if (!FileRdEdit->text().isEmpty()) sprintf(Cmd.fileread, FileRdEdit->text());
	else {AskCmdListArg(); return;}
	Cmd.delay = DelayEdit->text().toShort(0,10);
	if (!FileData(Cmd.fileread, Cmd.delay)) {statusBar()->message("Wrong file command.");return;}
	statusBar()->message("File Data done.");
    }
}
