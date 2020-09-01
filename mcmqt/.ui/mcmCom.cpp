/****************************************************************************
** Form implementation generated from reading ui file 'mcmCom.ui'
**
** Created by User Interface Compiler
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "mcmCom.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qframe.h>
#include <qcheckbox.h>
#include <qtextedit.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qlistbox.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../mcmCom.ui.h"
/*
 *  Constructs a mcmCOM as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
mcmCOM::mcmCOM( QWidget* parent, const char* name, WFlags fl )
    : QMainWindow( parent, name, fl )
{
    (void)statusBar();
    if ( !name )
	setName( "mcmCOM" );
    QPalette pal;
    QColorGroup cg;
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 253, 253, 253) );
    cg.setColor( QColorGroup::Dark, QColor( 206, 206, 206) );
    cg.setColor( QColorGroup::Mid, QColor( 191, 191, 191) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, QColor( 250, 250, 250) );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 68, 100, 172) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Light, QColor( 250, 250, 250) );
    cg.setColor( QColorGroup::Midlight, QColor( 253, 253, 253) );
    cg.setColor( QColorGroup::Dark, QColor( 143, 143, 143) );
    cg.setColor( QColorGroup::Mid, QColor( 191, 191, 191) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, QColor( 250, 250, 250) );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 94, 122, 183) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Light, QColor( 250, 250, 250) );
    cg.setColor( QColorGroup::Midlight, QColor( 253, 253, 253) );
    cg.setColor( QColorGroup::Dark, QColor( 143, 143, 143) );
    cg.setColor( QColorGroup::Mid, QColor( 191, 191, 191) );
    cg.setColor( QColorGroup::Text, QColor( 191, 191, 191) );
    cg.setColor( QColorGroup::BrightText, QColor( 250, 250, 250) );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setDisabled( cg );
    setPalette( pal );
    setCentralWidget( new QWidget( this, "qt_central_widget" ) );

    Title = new QLabel( centralWidget(), "Title" );
    Title->setGeometry( QRect( 290, 20, 360, 23 ) );
    QFont Title_font(  Title->font() );
    Title_font.setPointSize( 14 );
    Title_font.setBold( TRUE );
    Title->setFont( Title_font ); 
    Title->setFrameShape( QLabel::NoFrame );
    Title->setFrameShadow( QLabel::Plain );
    Title->setAlignment( int( QLabel::AlignCenter ) );

    ExitButton = new QPushButton( centralWidget(), "ExitButton" );
    ExitButton->setGeometry( QRect( 750, 300, 90, 30 ) );

    Connection_box = new QGroupBox( centralWidget(), "Connection_box" );
    Connection_box->setGeometry( QRect( 710, 50, 160, 110 ) );

    OpenButton = new QPushButton( Connection_box, "OpenButton" );
    OpenButton->setGeometry( QRect( 20, 30, 80, 30 ) );
    OpenButton->setToggleButton( FALSE );

    PortEdit = new QLineEdit( Connection_box, "PortEdit" );
    PortEdit->setGeometry( QRect( 120, 29, 21, 31 ) );

    CloseButton = new QPushButton( Connection_box, "CloseButton" );
    CloseButton->setEnabled( FALSE );
    CloseButton->setGeometry( QRect( 21, 67, 120, 30 ) );
    CloseButton->setToggleButton( FALSE );

    PollingBox = new QGroupBox( centralWidget(), "PollingBox" );
    PollingBox->setGeometry( QRect( 720, 180, 140, 100 ) );

    PollEdit = new QLineEdit( PollingBox, "PollEdit" );
    PollEdit->setGeometry( QRect( 20, 30, 100, 23 ) );
    PollEdit->setPaletteBackgroundColor( QColor( 230, 230, 230 ) );
    PollEdit->setAlignment( int( QLineEdit::AlignAuto ) );
    PollEdit->setEdited( TRUE );
    PollEdit->setReadOnly( TRUE );

    PollButton = new QPushButton( PollingBox, "PollButton" );
    PollButton->setGeometry( QRect( 20, 70, 100, 20 ) );
    PollButton->setToggleButton( FALSE );

    ReadMcm = new QGroupBox( centralWidget(), "ReadMcm" );
    ReadMcm->setGeometry( QRect( 30, 340, 840, 270 ) );

    CmdReceived = new QLabel( ReadMcm, "CmdReceived" );
    CmdReceived->setGeometry( QRect( 21, 54, 125, 23 ) );

    AnlMask = new QLabel( ReadMcm, "AnlMask" );
    AnlMask->setGeometry( QRect( 21, 131, 90, 23 ) );

    DigMask = new QLabel( ReadMcm, "DigMask" );
    DigMask->setGeometry( QRect( 21, 101, 90, 23 ) );

    ScanData = new QLabel( ReadMcm, "ScanData" );
    ScanData->setGeometry( QRect( 21, 161, 90, 23 ) );

    CmdStatus = new QLabel( ReadMcm, "CmdStatus" );
    CmdStatus->setGeometry( QRect( 21, 23, 125, 23 ) );

    frame10 = new QFrame( ReadMcm, "frame10" );
    frame10->setGeometry( QRect( 600, 30, 191, 101 ) );
    frame10->setFrameShape( QFrame::StyledPanel );
    frame10->setFrameShadow( QFrame::Sunken );

    Mode = new QLabel( frame10, "Mode" );
    Mode->setGeometry( QRect( 11, 42, 43, 23 ) );

    McmAddr2Edit = new QLineEdit( frame10, "McmAddr2Edit" );
    McmAddr2Edit->setGeometry( QRect( 108, 12, 71, 23 ) );
    McmAddr2Edit->setPaletteBackgroundColor( QColor( 230, 230, 230 ) );
    McmAddr2Edit->setReadOnly( TRUE );

    ModEdit = new QLineEdit( frame10, "ModEdit" );
    ModEdit->setGeometry( QRect( 60, 42, 119, 23 ) );
    ModEdit->setPaletteBackgroundColor( QColor( 230, 230, 230 ) );
    ModEdit->setReadOnly( TRUE );

    McmAddr2 = new QLabel( frame10, "McmAddr2" );
    McmAddr2->setGeometry( QRect( 11, 12, 91, 23 ) );

    VersEdit = new QLineEdit( frame10, "VersEdit" );
    VersEdit->setGeometry( QRect( 108, 72, 71, 23 ) );
    VersEdit->setPaletteBackgroundColor( QColor( 230, 230, 230 ) );
    VersEdit->setReadOnly( TRUE );

    Version = new QLabel( frame10, "Version" );
    Version->setGeometry( QRect( 11, 72, 91, 23 ) );

    ClearButton = new QPushButton( ReadMcm, "ClearButton" );
    ClearButton->setGeometry( QRect( 20, 240, 110, 20 ) );

    DisplayButton = new QCheckBox( ReadMcm, "DisplayButton" );
    DisplayButton->setGeometry( QRect( 520, 140, 200, 20 ) );

    ScanDataEdit = new QTextEdit( ReadMcm, "ScanDataEdit" );
    ScanDataEdit->setGeometry( QRect( 118, 160, 280, 68 ) );
    ScanDataEdit->setPaletteBackgroundColor( QColor( 230, 230, 230 ) );
    ScanDataEdit->setVScrollBarMode( QTextEdit::AlwaysOn );
    ScanDataEdit->setReadOnly( TRUE );

    DDEdit = new QTextEdit( ReadMcm, "DDEdit" );
    DDEdit->setGeometry( QRect( 420, 170, 410, 80 ) );
    DDEdit->setPaletteBackgroundColor( QColor( 230, 230, 230 ) );
    QFont DDEdit_font(  DDEdit->font() );
    DDEdit_font.setFamily( "Monospace" );
    DDEdit->setFont( DDEdit_font ); 
    DDEdit->setDragAutoScroll( TRUE );
    DDEdit->setReadOnly( TRUE );

    CmdSttusEdit = new QLineEdit( ReadMcm, "CmdSttusEdit" );
    CmdSttusEdit->setGeometry( QRect( 152, 23, 210, 23 ) );
    CmdSttusEdit->setPaletteBackgroundColor( QColor( 230, 230, 230 ) );
    CmdSttusEdit->setFrameShape( QLineEdit::LineEditPanel );
    CmdSttusEdit->setReadOnly( TRUE );

    CmdRxEdit = new QLineEdit( ReadMcm, "CmdRxEdit" );
    CmdRxEdit->setGeometry( QRect( 152, 54, 210, 23 ) );
    CmdRxEdit->setPaletteBackgroundColor( QColor( 230, 230, 230 ) );
    CmdRxEdit->setReadOnly( TRUE );

    CountEdit = new QLineEdit( ReadMcm, "CountEdit" );
    CountEdit->setGeometry( QRect( 499, 31, 50, 23 ) );
    CountEdit->setPaletteBackgroundColor( QColor( 230, 230, 230 ) );
    CountEdit->setReadOnly( TRUE );

    Counter = new QLabel( ReadMcm, "Counter" );
    Counter->setGeometry( QRect( 411, 31, 80, 23 ) );

    AvgFact = new QLabel( ReadMcm, "AvgFact" );
    AvgFact->setGeometry( QRect( 411, 70, 103, 23 ) );

    AvgfactEdit = new QLineEdit( ReadMcm, "AvgfactEdit" );
    AvgfactEdit->setGeometry( QRect( 520, 70, 29, 23 ) );
    AvgfactEdit->setPaletteBackgroundColor( QColor( 230, 230, 230 ) );
    AvgfactEdit->setFrameShape( QLineEdit::LineEditPanel );
    AvgfactEdit->setReadOnly( TRUE );

    DigMaskEdit = new QLineEdit( ReadMcm, "DigMaskEdit" );
    DigMaskEdit->setGeometry( QRect( 118, 102, 160, 23 ) );
    DigMaskEdit->setPaletteBackgroundColor( QColor( 230, 230, 230 ) );
    DigMaskEdit->setReadOnly( TRUE );

    AnMaskEdit = new QLineEdit( ReadMcm, "AnMaskEdit" );
    AnMaskEdit->setGeometry( QRect( 118, 131, 160, 23 ) );
    AnMaskEdit->setPaletteBackgroundColor( QColor( 230, 230, 230 ) );
    AnMaskEdit->setReadOnly( TRUE );

    CmdTab = new QTabWidget( centralWidget(), "CmdTab" );
    CmdTab->setGeometry( QRect( 30, 60, 660, 270 ) );

    tab = new QWidget( CmdTab, "tab" );

    CmdList = new QListBox( tab, "CmdList" );
    CmdList->setGeometry( QRect( 330, 20, 300, 100 ) );
    CmdList->setVScrollBarMode( QListBox::AlwaysOn );
    CmdList->setHScrollBarMode( QListBox::AlwaysOff );

    InsertButton = new QPushButton( tab, "InsertButton" );
    InsertButton->setGeometry( QRect( 218, 45, 76, 20 ) );

    RemoveButton = new QPushButton( tab, "RemoveButton" );
    RemoveButton->setGeometry( QRect( 218, 81, 76, 20 ) );

    ClearListButton = new QPushButton( tab, "ClearListButton" );
    ClearListButton->setGeometry( QRect( 218, 127, 76, 20 ) );

    MaskTitle = new QLabel( tab, "MaskTitle" );
    MaskTitle->setGeometry( QRect( 20, 190, 102, 20 ) );
    MaskTitle->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );

    Argument1 = new QLineEdit( tab, "Argument1" );
    Argument1->setEnabled( FALSE );
    Argument1->setGeometry( QRect( 130, 190, 200, 25 ) );
    Argument1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, Argument1->sizePolicy().hasHeightForWidth() ) );
    Argument1->setMaxLength( 24 );

    WriteButton = new QPushButton( tab, "WriteButton" );
    WriteButton->setGeometry( QRect( 380, 190, 120, 30 ) );

    StopButton = new QPushButton( tab, "StopButton" );
    StopButton->setEnabled( FALSE );
    StopButton->setGeometry( QRect( 520, 190, 110, 30 ) );

    RepeatButton = new QCheckBox( tab, "RepeatButton" );
    RepeatButton->setGeometry( QRect( 510, 140, 140, 20 ) );

    delaylabel = new QLabel( tab, "delaylabel" );
    delaylabel->setGeometry( QRect( 330, 140, 96, 33 ) );
    delaylabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );

    DelayEdit1 = new QLineEdit( tab, "DelayEdit1" );
    DelayEdit1->setEnabled( TRUE );
    DelayEdit1->setGeometry( QRect( 430, 140, 50, 23 ) );
    DelayEdit1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, DelayEdit1->sizePolicy().hasHeightForWidth() ) );

    CmdAvaiList = new QListBox( tab, "CmdAvaiList" );
    CmdAvaiList->setGeometry( QRect( 20, 20, 170, 100 ) );

    McmAddr = new QLabel( tab, "McmAddr" );
    McmAddr->setGeometry( QRect( 20, 150, 120, 20 ) );
    McmAddr->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );

    McmAddrEdit = new QLineEdit( tab, "McmAddrEdit" );
    McmAddrEdit->setEnabled( FALSE );
    McmAddrEdit->setGeometry( QRect( 160, 150, 30, 23 ) );
    McmAddrEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, McmAddrEdit->sizePolicy().hasHeightForWidth() ) );
    McmAddrEdit->setMaxLength( 2 );
    CmdTab->insertTab( tab, QString::fromLatin1("") );

    tab_2 = new QWidget( CmdTab, "tab_2" );

    groupBox3 = new QGroupBox( tab_2, "groupBox3" );
    groupBox3->setGeometry( QRect( 11, 85, 234, 130 ) );

    FileRdLabel = new QLabel( groupBox3, "FileRdLabel" );
    FileRdLabel->setGeometry( QRect( 8, 20, 90, 22 ) );

    FileRdEdit = new QLineEdit( groupBox3, "FileRdEdit" );
    FileRdEdit->setGeometry( QRect( 36, 40, 190, 23 ) );

    FileWrLabel = new QLabel( groupBox3, "FileWrLabel" );
    FileWrLabel->setGeometry( QRect( 8, 71, 74, 23 ) );

    FileWrEdit = new QLineEdit( groupBox3, "FileWrEdit" );
    FileWrEdit->setGeometry( QRect( 36, 91, 190, 23 ) );

    EnterButton = new QPushButton( tab_2, "EnterButton" );
    EnterButton->setEnabled( TRUE );
    EnterButton->setGeometry( QRect( 561, 98, 76, 30 ) );

    StopButton2 = new QPushButton( tab_2, "StopButton2" );
    StopButton2->setEnabled( FALSE );
    StopButton2->setGeometry( QRect( 561, 154, 76, 30 ) );

    ComboCmd = new QComboBox( FALSE, tab_2, "ComboCmd" );
    ComboCmd->setGeometry( QRect( 20, 30, 190, 31 ) );

    groupBox4 = new QGroupBox( tab_2, "groupBox4" );
    groupBox4->setGeometry( QRect( 271, 45, 270, 170 ) );

    AnlMaskLabel = new QLabel( groupBox4, "AnlMaskLabel" );
    AnlMaskLabel->setGeometry( QRect( 12, 27, 236, 20 ) );
    AnlMaskLabel->setAlignment( int( QLabel::AlignVCenter ) );

    AnMaskEdit2 = new QLineEdit( groupBox4, "AnMaskEdit2" );
    AnMaskEdit2->setGeometry( QRect( 32, 57, 210, 23 ) );

    DelayEdit = new QLineEdit( groupBox4, "DelayEdit" );
    DelayEdit->setGeometry( QRect( 156, 131, 90, 23 ) );

    DelayLabel = new QLabel( groupBox4, "DelayLabel" );
    DelayLabel->setGeometry( QRect( 21, 131, 114, 23 ) );

    McmAddrEdit2 = new QLineEdit( groupBox4, "McmAddrEdit2" );
    McmAddrEdit2->setEnabled( FALSE );
    McmAddrEdit2->setGeometry( QRect( 155, 100, 90, 23 ) );

    McmAddrLabel = new QLabel( groupBox4, "McmAddrLabel" );
    McmAddrLabel->setGeometry( QRect( 21, 102, 114, 23 ) );
    CmdTab->insertTab( tab_2, QString::fromLatin1("") );

    // toolbars

    languageChange();
    resize( QSize(888, 632).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( ClearListButton, SIGNAL( clicked() ), this, SLOT( ClearList() ) );
    connect( StopButton, SIGNAL( clicked() ), this, SLOT( StopCmds() ) );
    connect( ClearButton, SIGNAL( clicked() ), this, SLOT( ClearAns() ) );
    connect( CmdAvaiList, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( ProposeArg() ) );
    connect( OpenButton, SIGNAL( clicked() ), this, SLOT( SerialConnection() ) );
    connect( RemoveButton, SIGNAL( clicked() ), this, SLOT( RmCommand() ) );
    connect( InsertButton, SIGNAL( clicked() ), this, SLOT( GetCommand() ) );
    connect( CloseButton, SIGNAL( clicked() ), this, SLOT( SerialDisconnection() ) );
    connect( WriteButton, SIGNAL( clicked() ), this, SLOT( WriteBuf() ) );
    connect( ExitButton, SIGNAL( clicked() ), this, SLOT( Exitfinal() ) );
    connect( ComboCmd, SIGNAL( activated(int) ), this, SLOT( AskCmdListArg() ) );
    connect( EnterButton, SIGNAL( clicked() ), this, SLOT( ExecFileCmd() ) );
    connect( StopButton2, SIGNAL( clicked() ), this, SLOT( StopCmds() ) );
    connect( PollButton, SIGNAL( clicked() ), this, SLOT( McmPoll() ) );

    // tab order
    setTabOrder( OpenButton, CloseButton );
    setTabOrder( CloseButton, CmdAvaiList );
    setTabOrder( CmdAvaiList, McmAddrEdit );
    setTabOrder( McmAddrEdit, Argument1 );
    setTabOrder( Argument1, InsertButton );
    setTabOrder( InsertButton, WriteButton );
    setTabOrder( WriteButton, StopButton );
    setTabOrder( StopButton, ComboCmd );
    setTabOrder( ComboCmd, FileRdEdit );
    setTabOrder( FileRdEdit, FileWrEdit );
    setTabOrder( FileWrEdit, AnMaskEdit2 );
    setTabOrder( AnMaskEdit2, DelayEdit );
    setTabOrder( DelayEdit, EnterButton );
    setTabOrder( EnterButton, ExitButton );
    setTabOrder( ExitButton, CmdTab );
    setTabOrder( CmdTab, CmdList );
    setTabOrder( CmdList, RepeatButton );
    setTabOrder( RepeatButton, RemoveButton );
    setTabOrder( RemoveButton, ClearListButton );
    setTabOrder( ClearListButton, McmAddrEdit2 );
    setTabOrder( McmAddrEdit2, CmdRxEdit );
    setTabOrder( CmdRxEdit, DigMaskEdit );
    setTabOrder( DigMaskEdit, AnMaskEdit );
    setTabOrder( AnMaskEdit, DisplayButton );
    setTabOrder( DisplayButton, CmdSttusEdit );
    setTabOrder( CmdSttusEdit, McmAddr2Edit );
    setTabOrder( McmAddr2Edit, ModEdit );
    setTabOrder( ModEdit, VersEdit );
    setTabOrder( VersEdit, ClearButton );
    setTabOrder( ClearButton, AvgfactEdit );
    setTabOrder( AvgfactEdit, CountEdit );
    setTabOrder( CountEdit, DDEdit );
    setTabOrder( DDEdit, DelayEdit1 );
    setTabOrder( DelayEdit1, StopButton2 );
}

/*
 *  Destroys the object and frees any allocated resources
 */
mcmCOM::~mcmCOM()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void mcmCOM::languageChange()
{
    setCaption( tr( "MCM Communication" ) );
    setIconText( tr( "Cmd to exec" ) );
    Title->setText( tr( "MCM Serial Communication" ) );
    ExitButton->setText( tr( "Exit" ) );
    Connection_box->setTitle( tr( "PC serial port (default 1)" ) );
    OpenButton->setText( tr( "Open Port:" ) );
    CloseButton->setText( tr( "Close Port" ) );
    PollingBox->setTitle( tr( "Connected MCMs" ) );
    PollButton->setText( tr( "Poll" ) );
    ReadMcm->setTitle( tr( "MCM Answers" ) );
    CmdReceived->setText( tr( "Command Received:" ) );
    AnlMask->setText( tr( "Analog Mask:" ) );
    DigMask->setText( tr( "Digital Mask:" ) );
    ScanData->setText( tr( "Scanned Data:" ) );
    CmdStatus->setText( tr( "Command Status:" ) );
    Mode->setText( tr( "Mode:" ) );
    McmAddr2->setText( tr( "MCM Address:" ) );
    Version->setText( tr( "Version:" ) );
    ClearButton->setText( tr( "Clear Ans" ) );
    DisplayButton->setText( tr( "Display all data (1 line / MUX):" ) );
    ScanDataEdit->setText( QString::null );
    DDEdit->setText( QString::null );
    Counter->setText( tr( "Ans Counter:" ) );
    AvgFact->setText( tr( "Averaging factor:" ) );
    AnMaskEdit->setText( QString::null );
    InsertButton->setText( tr( "->" ) );
    RemoveButton->setText( tr( "<-" ) );
    ClearListButton->setText( tr( "Clear List" ) );
    MaskTitle->setText( tr( "Cmd Parameters:" ) );
    Argument1->setText( QString::null );
    WriteButton->setText( tr( "Write" ) );
    StopButton->setText( tr( "Stop" ) );
    RepeatButton->setText( tr( "Repeat Commands" ) );
    delaylabel->setText( tr( "Inter-Cmd delay:\n"
"(ms, default 0)" ) );
    DelayEdit1->setText( QString::null );
    CmdAvaiList->clear();
    CmdAvaiList->insertItem( tr( "Set Mcm Addr" ) );
    CmdAvaiList->insertItem( tr( "Null Cmd" ) );
    CmdAvaiList->insertItem( tr( "Set Idle Mode" ) );
    CmdAvaiList->insertItem( tr( "Set Scan Mode" ) );
    CmdAvaiList->insertItem( tr( "Set Mean Mode" ) );
    CmdAvaiList->insertItem( tr( "Set Anl Mask" ) );
    CmdAvaiList->insertItem( tr( "Set Dig Mask 16b " ) );
    CmdAvaiList->insertItem( tr( "Set Dig Mask 32b " ) );
    CmdAvaiList->insertItem( tr( "Set Dig Mask 64b" ) );
    CmdAvaiList->insertItem( tr( "Read Anl Mask" ) );
    CmdAvaiList->insertItem( tr( "Read Dig Mask 16b" ) );
    CmdAvaiList->insertItem( tr( "Read Dig Mask 32b" ) );
    CmdAvaiList->insertItem( tr( "Read Dig Mask 64b" ) );
    CmdAvaiList->insertItem( tr( "Read Version" ) );
    CmdAvaiList->insertItem( tr( "Read Mode" ) );
    CmdAvaiList->insertItem( tr( "Reboot" ) );
    CmdAvaiList->insertItem( tr( "Feed Cntrl (old) " ) );
    CmdAvaiList->insertItem( tr( "Feed Cntrl (new) " ) );
    CmdAvaiList->insertItem( tr( "Fe Box Monitor " ) );
    CmdAvaiList->insertItem( tr( "Common Box Monitor" ) );
    McmAddr->setText( tr( "MCM Address (dec):" ) );
    McmAddrEdit->setText( QString::null );
    CmdTab->changeTab( tab, tr( "Basic Commands" ) );
    groupBox3->setTitle( tr( "File parameters" ) );
    FileRdLabel->setText( tr( "Read from file:" ) );
    FileWrLabel->setText( tr( "Write to file:" ) );
    EnterButton->setText( tr( "Enter" ) );
    StopButton2->setText( tr( "Stop" ) );
    ComboCmd->clear();
    ComboCmd->insertItem( tr( "File of file" ) );
    ComboCmd->insertItem( tr( "Data Monitoring" ) );
    ComboCmd->insertItem( tr( "File Data" ) );
    groupBox4->setTitle( tr( "Cmds parameters" ) );
    AnlMaskLabel->setText( tr( "Analog Mask (xx xx xx xx xx xx xx xx):" ) );
    DelayLabel->setText( tr( "Delay (option, ms):" ) );
    McmAddrLabel->setText( tr( "MCM Address (dec):" ) );
    CmdTab->changeTab( tab_2, tr( "Pre-composed Commands" ) );
}

