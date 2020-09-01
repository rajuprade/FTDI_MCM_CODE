TOPDIR  := $(shell cd ..; pwd)
include $(TOPDIR)/Rules.make

APP = mcmtest

all: $(APP)

$(APP): mcmtest.c	
	$(CC) -o $(APP) mcmtest.c $(CFLAGS)	
	
clean:
	rm -f *.o ; rm $(APP)
