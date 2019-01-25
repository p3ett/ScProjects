OBJS 	= main.o functions.o
SOURCE	= main.c functions.c
HEADER  = simulator.h
OUT  	= factory
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
