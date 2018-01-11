/*
 **Authors: Simar Chhabra
 **File defines methods for BitOutputStream class. 
 **/
#include <iostream>
#include "BitOutputStream.h"

#define BYTE 8

using namespace std;

void BitOutputStream::flush() {
    out.put(buf);
    out.flush();
    buf = 0;
    nbits = 1;
}

void BitOutputStream::writeBit(int i)
{
    if (nbits++ == BYTE)
    {
        flush();
    }
    //store i value in (BYTE - nbits) position left shifted from starting
    //point
    buf = buf | (i << (BYTE - nbits));
}
