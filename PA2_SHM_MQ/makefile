# makefile

all: dataserver client

common.o: common.h common.cpp
	g++ -g -w -std=c++14 -c common.cpp

FIFOreqchannel.o: FIFOreqchannel.h FIFOreqchannel.cpp
	g++ -g -w -std=c++14 -c FIFOreqchannel.cpp




#SHMreqchannel.o SHMBoundedBuffer.o KernelSemaphore.o

SHMreqchannel.o: SHMreqchannel.hpp SHMreqchannel.cpp 
	g++ -g -w  -std=c++11 -c SHMreqchannel.cpp 


client: client.cpp FIFOreqchannel.o common.o SHMreqchannel.o
	g++ -g -w -std=c++14 -o client client.cpp FIFOreqchannel.o  SHMreqchannel.o common.o -lpthread 

dataserver: dataserver.cpp  FIFOreqchannel.o common.o SHMreqchannel.o
	g++ -g -w -std=c++14 -o dataserver dataserver.cpp FIFOreqchannel.o SHMreqchannel.o common.o -lpthread 

clean:
	rm -rf *.o *.csv fifo* dataserver client data*_*
