/*
 **Author: Simar Chhabra
 **File takes in 2 files, the first of which is an input file to be compressed
   and the second the file that is the compressed version of that file,
   containing header information to rebuild the tree and the encoded message
 **/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "HCNode.h"
#include "HCTree.h"
#include "BitOutputStream.h"

int main(int argc, char* argv[])
{
    vector<int> freqs (256, 0); 
    unsigned char nextChar;
    int nextByte;
    int count = 0;
    //open file to be compressed
    ifstream in(argv[1], ios::binary);
  
    //store frequency of each unique byte value in message
    while ((nextByte=in.get())!=EOF)
    {
        //error checking
        if (in.eof())
        {
            break;
        }
        nextChar = (unsigned char) nextByte;
        freqs.at(nextByte)++;
        ++count;
    }
    in.close();
    HCTree hctree;

    //build Huffman Code tree
    hctree.build(freqs);
    
    //write to compressed file
    ofstream out(argv[2]);
  
    //output count of number of values to be encoded
    BitOutputStream bitwrite(out);
    out << count << '\n';
  
    //output the byte symbols
    hctree.outputLeafs(out);
    
    //output the length of each encoded message for each leaf
    hctree.getLengths(freqs,bitwrite);
    
    //output the encoded message for each leaf
    hctree.leafEncode(bitwrite);
    
    ifstream secin(argv[1]);
    //encode message
    while ((nextByte=secin.get())!=EOF)
    {
        if (secin.eof())
        {
            break;
        }
        nextChar = (unsigned char) nextByte;
        hctree.encode(nextChar, bitwrite);
    }
    //flush any remaining bits
    bitwrite.flush();
    secin.close();
    out.close();
}
