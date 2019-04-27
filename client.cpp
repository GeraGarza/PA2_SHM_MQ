/*
 Tanzir Ahmed
 Department of Computer Science & Engineering
 Texas A&M University
 Date  : 2/8/19
 */
#include "common.h"
#include "FIFOreqchannel.h"
#include "ReqChannel.h"
#include "SHMReqChannel.hpp"
#include <fstream>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


using namespace std;


struct timeval start;
struct timeval end_time;
unsigned long time_in_micros;
double timeTotal;
fstream o_time("received/outtime.csv", std::ofstream::out);

float round_3dec(float var)
{
    float value = (int)(var * 1000 + .5);
    return (float)value / 1000;
}


char* new_Channel(RequestChannel* channel){
    
    MESSAGE_TYPE* msg =  new MESSAGE_TYPE(NEWCHANNEL_MSG);
    channel->cwrite((char*)msg,sizeof(MESSAGE_TYPE));
    char* chan = channel->cread();
    
    return chan;
    
}


//datamsg (int _person, double _seconds, int _eno)
void request_Point(RequestChannel* channel){
    
    
    fstream ofs("received/outfile.csv", std::ofstream::out);
    
    
    float counter = 0.000;
    while (counter < 00.008){
        
        datamsg msg1(1,counter,1);
        channel->cwrite((char*)(&msg1),sizeof(datamsg));
        double* response1 = (double*)(channel->cread());
        
        
        datamsg msg2(1,counter,2);
        channel->cwrite((char*)(&msg2),sizeof(datamsg));
        double* response2 = (double*)(channel->cread());
        
        cout << counter << "," <<   *response1 << "," << *response2  << endl;
        gettimeofday(&end_time, NULL);
        timeTotal =  (end_time.tv_sec + end_time.tv_usec)  - (start.tv_sec + start.tv_usec );
        o_time << timeTotal <<endl;
        counter = round_3dec(counter + 0.004);
    }
    ofs.close();
    
}


void request_File(RequestChannel* channel){
    
    FILE *file = fopen ("received/x1.csv", "wb");
    //send message to get length
    string fileName;
    char* buffer = new char [sizeof(filemsg)+fileName.length()+1];
    //int total_requests = size/256 ;
    //filemsg (__int64_t _offset, int _length)
    filemsg* msg = new filemsg(0,0);
    
    //offset :  0*256 ,
    //process_file_request(channel,buffer);
    fileName = "1.csv";
    
    
    memcpy(buffer, (char*)msg,sizeof(filemsg));
    strcpy(buffer+sizeof(filemsg), fileName.c_str());
    channel->cwrite(buffer,sizeof(filemsg) + fileName.length()+1 );
    int* response1 = (int*)(channel->cread());
    
    
    int* length = response1;
    int duration = ceil(((double)*response1)/256);
    int tmp = *response1;
    
    for(int i=0; i < duration; i++){
        
        ((filemsg*)buffer)->offset = i*256;
        
        if(i!=duration-1) ((filemsg*)buffer)->length = 256;
        else ((filemsg*)buffer)->length = tmp%256;
        
        channel->cwrite(buffer,sizeof(filemsg) + fileName.length()+1 );
        char* response = (char*)(channel->cread());
        
        fwrite( &response[0]  ,1 , ((filemsg*)buffer)->length, file );
        gettimeofday(&end_time, NULL);
        timeTotal =  (end_time.tv_sec + end_time.tv_usec)  - (start.tv_sec + start.tv_usec );
        o_time << timeTotal <<endl;
    }
    
    fclose (file);
    
    
}

void request_Binary(RequestChannel* channel){
    
    FILE *file = fopen ("received/x1.csv", "wb");
    //send message to get length
    string fileName;
    char* buffer = new char [sizeof(filemsg)+fileName.length()+1];
    //int total_requests = size/256 ;
    //filemsg (__int64_t _offset, int _length)
    filemsg* msg = new filemsg(0,0);
    
    //offset :  0*256 ,
    //process_file_request(channel,buffer);
    fileName = "1.csv";
    
    
    memcpy(buffer, (char*)msg,sizeof(filemsg));
    strcpy(buffer+sizeof(filemsg), fileName.c_str());
    
    channel->cwrite(buffer,sizeof(filemsg) + sizeof(fileName)+1 );
    int* response1 = (int*)(channel->cread());
    int* length = response1;
    int duration = ceil(((double)*response1)/256);
    int tmp = *response1;
    
    for(int i=0; i < duration; i++){
        
        ((filemsg*)buffer)->offset = i*256;
        
        if(i!=duration-1) ((filemsg*)buffer)->length = 256;
        else ((filemsg*)buffer)->length = tmp%256;
        
        channel->cwrite(buffer,sizeof(filemsg) + sizeof(fileName)+1 );
        char* response = (char*)(channel->cread());
        
        fwrite( &response[0]  ,1 , ((filemsg*)buffer)->length, file );
        gettimeofday(&end_time, NULL);
        timeTotal =  (end_time.tv_sec + end_time.tv_usec)  - (start.tv_sec + start.tv_usec );
        o_time << timeTotal <<endl;
    }
    
    fclose (file);
    
}


int main(int argc, char *argv[]){
    
    
    int process_id = fork();
    if (process_id == 0){
        //child
        //brain transplant, get process and swap
        execl("dataserver", (char*) NULL);
    }
    else {
        
        int n = 100;    // default number of requests per "patient"
        int p = 15;        // number of patients
        srand(time_t(NULL));
        SHMRequestChannel* chan = new SHMRequestChannel("control", RequestChannel::CLIENT_SIDE,256);
        //string channel_name = "control";
        gettimeofday(&start, NULL);
        request_Point(chan);
        MESSAGE_TYPE msg =  QUIT_MSG;
        //  chan->cwrite((char*)msg,sizeof( MESSAGE_TYPE));
        
        
    }
    
    
    return 0;
}

