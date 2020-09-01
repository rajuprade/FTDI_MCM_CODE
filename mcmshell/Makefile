# Makefile for Linux MCM Communication
CFLAGS= -g -Wall /home/teleset/Desktop/Hello/ftd2xx.h
SRCS= main.cpp exec.cpp decode.cpp cfile.cpp shell.cpp
RM= rm -f
CC= g++
LD= g++
PROG= mcmcom_shell
OBJS= main.o exec.o decode.o cfile.o shell.o

all: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(OBJS) -o $(PROG)

#%.o: %.c %.h
#	$(CC) $(CFLAG) -c %.c

main.o: main.cpp cfile.h
	$(CC) $(CFLAGS) -c main.cpp

exec.o: exec.cpp exec.h cfile.h ftd2xx.h
	$(CC) $(CFLAGS) -c exec.cpp

decode.o: decode.cpp decode.h cfile.h
	$(CC) $(CFLAGS) -c decode.cpp

cfile.o: cfile.cpp cfile.h
	$(CC) $(CFLAGS) -c cfile.cpp

shell.o: shell.cpp shell.h cfile.h
	$(CC) $(CFLAGS) -c shell.cpp

clean:
	$(RM) $(PROG) $(OBJS)





