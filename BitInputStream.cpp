/*
 **Author: Simar Chhabra
 **File defines methods of BitInputStream
 **/
#include <iostream>
#include "BitInputStream.h"

#define BYTE 8

void BitInputStream::fill()
{
    buf = in.get();
    nbits = 1;
    
}

int BitInputStream::readBit() {
    if (nbits++ == BYTE)
    {
        fill();
    }
    //extract correct digit at (BYTE - nbits) position
    return (buf >> (BYTE - nbits)) & 1;  
}
