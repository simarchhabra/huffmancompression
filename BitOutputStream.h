/*
 **Author: Simar Chhabra
 **File declares attributes and methods for BitOutputStream
 **/
#ifndef BITOUTPUTSTREAM_H
#define BITOUTPUTSTREAM_H

#include <iostream>

using namespace std;

class BitOutputStream {
    private:
        char buf;
        int nbits;
        ostream& out;
    public:
        BitOutputStream(ostream& os) : out(os), buf(0), nbits(0) {
        }
        /**
         * If BYTE bits have been written to buffer, flush the stream and 
         * reset value of buf to 0 and nbits to 0.*/
        void flush();
        /**
         * Set value of i at position (BYTE-nbits) index (reverse of that 
         * value in relation to BYTE so 7 to 0, 6 to 1 etc) and call flush
         * when nbits reaches BYTE */
        void writeBit(int i);
        
}; 
#endif
