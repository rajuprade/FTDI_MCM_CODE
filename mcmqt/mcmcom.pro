SOURCES	+= main.cpp \
	cfile.cpp \
	decode.cpp \
	exec.cpp
HEADERS	+= cfile.h \
	decode.h \
	exec.h
unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
FORMS	= mcmCom.ui
IMAGES	= images/editcopy \
	images/editcut \
	images/editpaste \
	images/filenew \
	images/fileopen \
	images/filesave \
	images/print \
	images/redo \
	images/searchfind \
	images/undo
TEMPLATE	=app
CONFIG	+= qt warn_on release
LANGUAGE	= C++
