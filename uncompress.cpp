/*
 **Author: Simar Chhabra
 **File takes in two files, one compressed output as its input and uncompressed
   output as its output. 
 **/
#include <iostream>
#include "fstream"
#include "HCNode.h"
#include "HCTree.h"

int main(int argc, char * argv[])
{
    //Read in the encoded message using an ifstream
    ifstream in(argv[1]);
    
    //build an empty vector of 256 ints initialized to 0
    vector<int> freqs(256,0);
    string bitCountString;
  
    //the first line read is converted to an int
    //bitcount is used to make sure extra bits are not read 
    getline(in,bitCountString);
    int bitCount = stoi(bitCountString);
    string leafCountString;
  
    //second line contains an int that represents the number of leaves
    getline(in, leafCountString);
    int leafCount = stoi(leafCountString);
  
    //used to track the number of leaves processed 
    int storeLeafCount = 0;
  
    //this vector stores the various symbols
    vector<byte> leafSymbols;
  
    while (storeLeafCount != leafCount)
    {
        byte c;
        c = in.get();
        //push the new byte into the vector and increment the number read
        leafSymbols.push_back(c);
        storeLeafCount++;
    }
     BitInputStream bitread (in);
    
    //this vector represents the message length
    vector<int> lengths;
  
    //use similar logic to previous loop to make sure all are counted
    int storeLengthCount = 0;
    while (storeLengthCount != leafCount)
    {
        int lengthCount = 1;
        int bit;
        while ((bit = bitread.readBit()) != 1)
        {
            lengthCount++;
        }
        lengths.push_back(lengthCount);
        storeLengthCount++;
    }

    //construct a HCTree and build the header with the vectors
    HCTree hctree;
    hctree.buildHeader(bitread, leafSymbols, lengths);
  
    //use ofstream to write the decoded message to the file
    ofstream out(argv[2]);
    int size = 0;
    for (int i = 0; i < bitCount; i++)
    {
        //error check for wrong input
        if (in.eof())
        {
        break;
        }
        //decode the tree based on the symbol
        int c = hctree.decode(bitread);
        if (c >= 0)
        {
            byte output = (byte) c;
            //add this newly read character to the file
            out << output;
        }
    }
    in.close();
    out.close();
    return 0;
}

