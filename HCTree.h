/*
 **Author: Simar Chhabra
 **File holds class declaration of HCTree, the central object of the program.
   Defines all methods and variables necessary, and includes necessary 
   standard and created files.
 **/

#ifndef HCTREE_H
#define HCTREE_H

#include <queue>
#include <vector>
#include <fstream>
#include "HCNode.h"
#include "BitInputStream.h"
#include "BitOutputStream.h"

using namespace std;

/** A 'function class' for use as the Compare class in a
 *  priority_queue<HCNode*>.
 *  For this to work, operator< must be defined to
 *  do the right thing on HCNodes.
 */
class HCNodePtrComp {
public:
    bool operator()(HCNode*& lhs, HCNode*& rhs) const {
        return *lhs < *rhs;
    }
};

/** A Huffman Code Tree class.
 *  Not very generic:  Use only if alphabet consists
 *  of unsigned chars.
 */
class HCTree {
private:
    HCNode* root;
    vector<HCNode*> leaves;

public:
    // explicit keyword is used to avoid accidental implicit conversions
    explicit HCTree() : root(0) {
        leaves = vector<HCNode*>(256, (HCNode*) 0);
    }

    ~HCTree();

    /** Use the Huffman algorithm to build a Huffman coding trie.
     *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
     *  the frequency of occurrence of byte i in the message.
     *  POSTCONDITION:  root points to the root of the trie,
     *  and leaves[i] points to the leaf node containing byte i.
     */
    void build(const vector<int>& freqs);

    /**
     * Outputs structure of Huffman tree to create a smaller header for
     * compressed file. 
     * PRECONDITION: A BitOutputStream object has been created, a vector 
     * of bytes that contains all unique byte values in the message, and
     * a vector of lengths that tracks the length of each encoded leaf 
     * message.
     * POSTCONDITION:root points to the root of the tree and leaves[i] 
     * points to the leaf node containing byte i. Essentially, tree
     * has been built and all connections made.
     */ 
    void buildHeader(BitInputStream& in, vector<byte> leafSymbols, 
        vector<int> lengths);

    /** Write to the given BitOutputStream
     *  the sequence of bits coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    void encode(byte symbol, BitOutputStream& out) const;

    /** Return symbol coded in the next sequence of bits from the stream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    int decode(BitInputStream& in) const;

    /** Helper method used in destructor */
    void destroy(HCNode* node);

    /** Gets the lengths of every encoding of every leaf. Writes to the
     * given BitOutPutStream this data.
     * PRECONDITION: build() has been called and a vector of
     * the frequencies of each possible byte value is known.
     */
    void getLengths(const vector<int>& freqs, BitOutputStream& out);
    
    /* Writes to the given BitOutputStream the encoded message of every leaf.
     * PRECONDITION: build() has been called.
     */
    void leafEncode(BitOutputStream& out);
    /* Writes to the given ostream the byte representation of every leaf's
     * symbol.
     * PRECONDITION: build() has been called such that all leaves have been
     * created.
     */
    void outputLeafs(ostream& out);
};

#endif // HCTREE_H
