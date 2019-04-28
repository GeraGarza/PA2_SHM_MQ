//
//  SHMReqChannel.hpp
//  PA2_SHM_MQ
//
//  Created by Gera Garza on 4/26/19.
//  Copyright © 2019 Gera Garza. All rights reserved.
//

#ifndef SHMReqChannel_hpp
#define SHMReqChannel_hpp

#include <stdio.h>
#include <semaphore.h>
#include "common.h"
#include "ReqChannel.h"

//dataserver, except in new, clinet change fifo to req

class KernelSemaphore{
    public:
    sem_t* sem;
    string file_name;

    

    KernelSemaphore(string,int); //int value, int seed
    ~KernelSemaphore();
    void P();
    void V();
};


class SHMBoundedBuffer{


public:
    char* buf;
    int fd ;
    int bs;
    string file_name;
    KernelSemaphore* full;
    KernelSemaphore* empty;



    SHMBoundedBuffer(string, int);
    ~SHMBoundedBuffer();
    void push(char*);
    char* pop();
};




class SHMRequestChannel : public RequestChannel
{
private:
    /*  The current implementation uses named pipes. */
    
    SHMBoundedBuffer* buffer1;
    SHMBoundedBuffer* buffer2;
    string T1;
    string T2;
    
    
public:
    SHMRequestChannel(const string _name, const Side _side, int _bs);
    
    ~SHMRequestChannel();
    /* Destructor of the local copy of the bus. By default, the Server Side deletes any IPC
     mechanisms associated with the channel. */
    
    char* cread(int *len=NULL);
    /* Blocking read of data from the channel. Returns a string of characters
     read from the channel. Returns NULL if read failed. */
    
    int cwrite(char *msg, int msglen);
    /* Write the data to the channel. The function returns the number of characters written
     to the channel. */
    
 
};



#endif /* SHMReqChannel_hpp */
