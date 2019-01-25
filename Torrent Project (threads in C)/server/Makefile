OBJS 	= MirrorServer.o functions.o queue.o server_assistant.o MirrorManager.o Worker.o terminator.o
SOURCE	= MirrorServer.c functions.c queue.c server_assistant.c MirrorManager.c Worker.c terminator.c
HEADER  = MirrorServer.h
OUT  	= MirrorServer
CC	= gcc -pthread
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
