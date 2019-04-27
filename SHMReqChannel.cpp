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



KernelSemaphore::KernelSemaphore(string file_name, int bs){ //send value,key'
    
    sem = sem_open(file_name.c_str(), O_CREAT, 0666, 0);

    
}
KernelSemaphore::~KernelSemaphore(){
    
    sem_close(sem);
}
void KernelSemaphore::P(){ //Decrement

    sem_wait(sem);
}
void KernelSemaphore::V(){ //Increment
    sem_post(sem);
}


//SHMBoundedBuffer Definitons
SHMBoundedBuffer::SHMBoundedBuffer(string file_name, int bs): full(file_name+"f", 0), empty(file_name+"e",1){
    
    this->file_name = file_name;
    

    fd = shm_open(file_name.c_str(), O_RDWR| O_CREAT, 0644);
    ftruncate(fd,bs);
    
    buf = (char*)mmap(NULL,bs, PROT_READ| PROT_WRITE, MAP_SHARED, fd,0);
    
    if(buf==NULL){
        cerr << "ERROR:" <<endl;
    }
    
    
}
SHMBoundedBuffer::~SHMBoundedBuffer(){
    
    close(fd);
    munmap(buf, bs);
    shm_unlink(file_name.c_str());
    
}
void SHMBoundedBuffer::push(string message){
    empty.P();
    memcpy(buf,message.c_str(),bs);
    full.V();
}
char* SHMBoundedBuffer::pop(){
    
    char* temp_return = new char[bs];
    full.P();
    memcpy(temp_return,buf,bs);
    empty.V();
    return temp_return;
}



SHMRequestChannel::SHMRequestChannel(const string _name, const Side _side, int bs) :RequestChannel(_name,_side, bs)
{
    
    T1 = "/" + _name + "1";
    T2 =  "/" + _name + "2";
    
    
    if(my_side==RequestChannel::CLIENT_SIDE){
        buffer1 = new SHMBoundedBuffer(T1,bs);
        buffer2 = new SHMBoundedBuffer(T2,bs);
    }
    else if(my_side==RequestChannel::SERVER_SIDE){
        buffer2 = new SHMBoundedBuffer(T1,bs);
        buffer1 = new SHMBoundedBuffer(T2,bs);
    }
    
    
}

SHMRequestChannel::~SHMRequestChannel()
{
    delete buffer1;
    delete buffer2;
    remove((T1).c_str());
    remove((T2).c_str());
}



char* SHMRequestChannel::cread(int *len)
{
    
    
    string temp_return;
    if(len){
        *len = 229;
    }
    
    return buffer2->pop(); //2 always popping
}


string SHMRequestChannel::create_file_name(){
    return "/shm_"+my_name;
}


int SHMRequestChannel::cwrite(char* msg, int len)
{
    
    buffer1->push(msg);
    return len;
}
