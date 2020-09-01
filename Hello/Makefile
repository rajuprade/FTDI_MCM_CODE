TOPDIR  := $(shell cd ..; pwd)
include $(TOPDIR)/Rules.make

APP = mcmtest

all: $(APP)

$(APP): mcmtest.c	
	$(CC) -o $(APP) $(CFLAGS) mcmtest.c	
	
clean:
	rm -f *.o ; rm $(APP)
