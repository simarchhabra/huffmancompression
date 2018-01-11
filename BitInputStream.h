/*
 **Author: Simar Chhabra
 **File holds class declarations for BitInputStream and defines constructor
 **/
#ifndef BITINPUTSTREAM_H
#define BITINPUTSTREAM_H

#include <iostream>

using namespace std;

class BitInputStream {
    private:
        char buf; 
        int nbits;
        istream & in;
    public:
        /**Constructor:initializes members**/
        BitInputStream(istream & is) : in(is) {
            buf = 0;
            nbits = 8;
        }
        /** Fills the buffer with new values once BYTE nbits values have been
         * stored in buffer. Resets nbits to 1 */
        void fill();
        /** At the correct index position, reads values of buffer at that index
         * and calls fill once nbits index has reached BYTE.
         */
        int readBit();
};    

#endif
