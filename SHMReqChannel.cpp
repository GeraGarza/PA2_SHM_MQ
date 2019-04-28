//
//  SHMReqChannel.cpp
//  PA2_SHM_MQ
//
//  Created by Gera Garza on 4/26/19.
//  Copyright © 2019 Gera Garza. All rights reserved.
//

#include "SHMReqChannel.hpp"

#include <sys/sem.h>

#include <iostream>
#include <sys/mman.h>

#include "common.h"
#include "SHMReqChannel.hpp"
#include "ReqChannel.h"


using namespace std;

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   R e q u e s t C h a n n e l  */
/*--------------------------------------------------------------------------*/



KernelSemaphore::KernelSemaphore(string _file_name, int val):file_name(_file_name){ //send value,key'
    sem = sem_open((char*)file_name.c_str(), O_CREAT, 0666, val);
}

KernelSemaphore::~KernelSemaphore(){
    sem_close(sem);
    sem_unlink((char*)file_name.c_str());
}

void KernelSemaphore::P(){ //Decrement
    sem_wait(sem);
}

void KernelSemaphore::V(){ //Increment
    sem_post(sem);
}


//SHMBoundedBuffer Definitons
SHMBoundedBuffer::SHMBoundedBuffer(string file_name, int bs): file_name(file_name),bs(bs){

    empty = new KernelSemaphore(file_name + "_e",1);
    full = new KernelSemaphore(file_name + "_f", 0);
    
    


    fd = shm_open((char*)file_name.c_str(), O_RDWR| O_CREAT, 0664);
    ftruncate(fd,bs);
    
    buf = (char*)mmap(NULL,bs, PROT_READ| PROT_WRITE, MAP_SHARED, fd,0);
    
    if(buf==NULL){
        cerr << "ERROR:" <<endl;
    }
    
    
}
SHMBoundedBuffer::~SHMBoundedBuffer(){
    
    close(fd);
    munmap(buf, bs);
    shm_unlink((char*)file_name.c_str());
    
}
void SHMBoundedBuffer::push(char* msg){
    
    empty->P();
    memcpy(buf,msg,bs);
    full->V();

}
char* SHMBoundedBuffer::pop(){
    
    char* tmp = new char[bs];
    
    full->P();
    memcpy(tmp,buf,bs);
    empty->V();

    return tmp;
}



SHMRequestChannel::SHMRequestChannel(const string _name, const Side _side, int bs) :RequestChannel(_name,_side, bs)
{
    
    T1 = "/" + _name + "_1";
    T2 =  "/" + _name + "_2";
    
    
    if(my_side==RequestChannel::SERVER_SIDE){
        buffer1 = new SHMBoundedBuffer(T1,bs);
        buffer2 = new SHMBoundedBuffer(T2,bs);
    }
    else if(my_side==RequestChannel::CLIENT_SIDE){
        buffer2 = new SHMBoundedBuffer(T1,bs);
        buffer1 = new SHMBoundedBuffer(T2,bs);
    }
    
    
}

SHMRequestChannel::~SHMRequestChannel()
{}

char* SHMRequestChannel::cread(int *len)
{
    
   char *buf = new char [bufferSize];

    buf = buffer2->pop();
    return buf;
}



int SHMRequestChannel::cwrite(char* msg, int len)
{
    buffer1->push(msg);
    return len;
}
