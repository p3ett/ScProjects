OBJS 	= main.o pool.o job.o status.o status_all.o showActive.o showFinished.o showPools.o suspend.o resume.o shutdown.o functions.o
SOURCE	= main.c pool.c job.c status.c status_all.c showActive.c showFinished.c showPools.c suspend.c resume.c shutdown.c functions.c
HEADER  = main.h
OUT  	= jms_coord
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
