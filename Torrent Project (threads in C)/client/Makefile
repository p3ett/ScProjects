OBJS 	= MirrorInitiator.o functions.o client_assistant.o
SOURCE	= MirrorInitiator.c functions.c client_assistant.c
HEADER  = MirrorInitiator.h
OUT  	= MirrorInitiator
CC	= gcc
FLAGS   = -g -c
    
# -g option enables debugging mode 
# -c flag generates object code for separate files

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

# create/compile the individual files >>separately<< 

# clean house
clean:
	rm -f $(OBJS) $(OUT)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
