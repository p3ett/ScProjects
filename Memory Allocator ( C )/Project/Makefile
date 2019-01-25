OBJS	= main.o functions.o hashTable.o hashTable2.o
SOURCE	= main.c functions.c hashTable.c hashTable2.c
HEADER	= functions.h hashTable.h hashTable2.h MemoryStatus.h
OUT	= myAllocator
CC	= gcc
FLAGS	= -g -c -Wall

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) 


main.o: main.c
	$(CC) $(FLAGS) main.c 

functions.o: functions.c
	$(CC) $(FLAGS) functions.c 

hashtable.o: hashtable.c
	$(CC) $(FLAGS) hashtable.c 

hashtable2.o: hashtable2.c
	$(CC) $(FLAGS) hashtable2.c 

clean:
	rm -f $(OBJS) $(OUT)

