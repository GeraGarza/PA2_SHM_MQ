//
//  ReqChannel.h
//  PA2_SHM_MQ
//
//  Created by Gera Garza on 4/26/19.
//  Copyright © 2019 Gera Garza. All rights reserved.
//

#ifndef ReqChannel_h
#define ReqChannel_h
#include <string>



class RequestChannel{
    
public:
    
    
    typedef enum {SERVER_SIDE, CLIENT_SIDE} Side ;
    typedef enum {READ_MODE, WRITE_MODE} Mode ;
    
    
    std::string my_name;
    Side my_side;
    int bufferSize;
    
    RequestChannel(const std::string _name, const Side _side, const int _bufferS):my_name(_name),my_side(_side), bufferSize(_bufferS){}
    virtual ~RequestChannel() {}
    
    std::string name(){
        return my_name;
    }
    
    virtual char* cread( int* ptr = NULL) = 0;
    /* Blocking read of data from the channel. Returns a string of
     characters read from the channel. Returns NULL if read failed. */
    virtual int cwrite( char* ptr , int len) = 0;
    /* Write the data to the channel. The function returns
     the number of characters written to the channel. */
    
};



#endif /* ReqChannel_h */
