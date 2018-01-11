/*
 **Author: Simar Chhabra
 **File defines all methods in HCTree class
 **/
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include "HCTree.h"

using namespace std;

void HCTree::build(const vector<int>& freqs)
{
    //priority queue to hold forest of nodes when building tree
    priority_queue<HCNode*,vector<HCNode*>, HCNodePtrComp> pq;
    for (int i = 0; i < freqs.size(); i++)
    {
        byte symbol = i;
        if (freqs.at(i) > 0)
        {
            //if a unique byte value is in the message, create node and
            //push to priority queue. store value in respective leaf
            HCNode* sntree = new HCNode(freqs[i], symbol, 0, 0, 0);
            pq.push(sntree);
            leaves.at(i) = sntree;
        }
    }
    //while the size of pq > 1, pop 2 lowest frequency nodes and add values
    //to create a subtree that is a parent of those 2 nodes with their combined
    //frequency. Push to priority queue.
    while (pq.size() > 1)
    {
        HCNode* T1 = pq.top();
        pq.pop();
        HCNode* T2 = pq.top();
        pq.pop();
        HCNode* roottree = new HCNode(T1->count + T2->count, 0, 0, 0, 0);
        T1->p = roottree;
        T2->p = roottree;  
        roottree->c1 = T1;
        roottree->c0 = T2;
        pq.push(roottree);
    }
    //root is max frequency node, top of pq
    root = pq.top();
}

void HCTree::buildHeader(BitInputStream& in, vector<byte> leafSymbols, 
  vector<int> lengths)
{
    //define root for other file that uses smaller header structure to
    //uncompress
    root = new HCNode(0, 0, 0, 0, 0);
    for (int i = 0; i < leafSymbols.size(); i++)
    {
        int lengthsSoFar = 0;
        HCNode* curr = root;
        //while encoded message for leaf[byte_value] has not reached
        //its max length for that respective leaf
        while (lengthsSoFar != lengths.at(i))
        {
            int bit = in.readBit();
            if (bit == 0)
            {
                //if 0 edge exists, traverse through tree
                if (curr->c0)
                {
                    curr = curr->c0;
                }

                else
                {
                    //if last edge, create node and store it in leaf
                    if (lengthsSoFar == (lengths.at(i) - 1))
                    {
                        curr->c0 = new HCNode(0, leafSymbols.at(i), 0, 0, 0);
                        curr->c0->p = curr;
                        curr = curr->c0;
                        leaves.at(leafSymbols.at(i)) = curr;
                    }
                    // otherwise, create new node and traverse
                    else
                    {
                        curr->c0 = new HCNode(0, 0, 0, 0, 0);
                        curr->c0->p = curr;
                        curr = curr->c0;
                    }
                }
            }
            //same for 1
            else if (bit == 1)
            {
                if (curr->c1)
                {
                    curr = curr->c1;
                }
                else
                {
                    if (lengthsSoFar == (lengths.at(i) - 1))
                    {
                        curr->c1 = new HCNode(0, leafSymbols.at(i), 0, 0, 0);
                        curr->c1->p = curr;
                        curr = curr->c1;
                        leaves.at(leafSymbols.at(i)) = curr;
                    }
                    else
                    {
                        curr->c1 = new HCNode(0, 0, 0, 0, 0);
                        curr->c1->p = curr;
                        curr = curr->c1;
                    }
                }
            }
            else
            {
                break;
            }
            lengthsSoFar++;
        }
    }
}

void HCTree::outputLeafs(ostream& out)
{
    int count = 0;
    //get the number of leaves and output it to stream
    for (int i = 0; i < leaves.size(); i++)
    {
        if (leaves.at(i) == 0)
        {
            continue;
        }
        ++count;
    }
    out << count << '\n';
    //output every leaf symbol to stream
    for (int i = 0; i < leaves.size(); i++)
    {
        if (leaves.at(i) == 0)
        {
            continue;
        }
        out << leaves.at(i)->symbol;
    }
}
void HCTree::encode(byte symbol, BitOutputStream& out) const {
    //get node corresponding to symbol
    HCNode* symbolptr = leaves.at(symbol);
    //create stack that will be used to traverse up tree to root
    //popped nodes will represent order of message
    stack<HCNode *> hcstack;
    hcstack.push(symbolptr);
    HCNode* curr = symbolptr;
    while (curr != root)
    {
        hcstack.push(curr->p);
        curr = curr->p;
    }

    while (hcstack.size() > 1)
    {
        curr = hcstack.top();
        hcstack.pop();
        if (curr->c0 == hcstack.top())
        {
            out.writeBit(0);
        }   
        else if (curr->c1 == hcstack.top())
        {
            out.writeBit(1);
        }
        else 
        {
            break;
        }
    }        
}

void HCTree::getLengths(const vector<int>& freqs, BitOutputStream& out) 
{
    stack<HCNode*> leafEncodingStack;
    //for every leaf that is not null
    for (int i = 0; i < leaves.size(); i++)
    {
        if (freqs.at(i) <= 0)
        {
            continue;
        }
        HCNode* curr = leaves.at(i);
        leafEncodingStack.push(curr);
        //create stack that gets all values from leaf to root
        while (curr != root)
        {
            leafEncodingStack.push(curr->p);   
            curr = curr->p;
        }
        while (leafEncodingStack.size() > 1)
        {
            curr = leafEncodingStack.top();
            leafEncodingStack.pop();
            //if next node is a leaf, output 1, otherwise output 0
            HCNode* potentialLeaf = leafEncodingStack.top();
            if (potentialLeaf == leaves.at(i))
            {
                out.writeBit(1);
            }
            else
            {
                out.writeBit(0);
            }
        }
        //set stack size back to 0
        leafEncodingStack.pop();
    }
}

void HCTree::leafEncode(BitOutputStream& out)
{
    stack<HCNode*> leafEncodingStack;
    //for every leaf that exists in the tree
    for (int i = 0; i < leaves.size(); i++)
    {
        if (leaves.at(i) == 0)
        {
            continue;
        }
        HCNode* curr = leaves.at(i);
        leafEncodingStack.push(curr);
        //same as above method
        while (curr != root)
        {
            leafEncodingStack.push(curr->p);
            curr = curr->p;
        }
        //encode message of every leaf from root to leaf and output to
        //BitOutputStream
        while (leafEncodingStack.size() > 1)
        {
            curr = leafEncodingStack.top();
            leafEncodingStack.pop();
            if (curr->c0 == leafEncodingStack.top())
            {
                out.writeBit(0);
            }
            else if (curr->c1 == leafEncodingStack.top())
            {
                out.writeBit(1);
            }
            else
            {
                break;
            }
        }
        leafEncodingStack.pop();
    }
}
int HCTree::decode(BitInputStream& in) const
{
	HCNode* temp = root;
	int bit;
	//read while not null
	while(temp->c0 || temp->c1)
	{
		bit = in.readBit();
		//based on bit value, reassign temp pointer
		if(bit == 0)
		{
			temp = temp->c0;
		}
		else if(bit == 1)
		{
			temp = temp->c1;
		}
		else
		{
			break;
		}
	}
	return (int)temp->symbol; 
}

//destructor method for the HCTree
HCTree::~HCTree()
{
	destroy(root);
}

//helper method used to destroy the HCTree
void HCTree::destroy(HCNode* node)
{
	if(node != nullptr)
	{
		//recursively destroy all the children
		destroy(node->c0);
		destroy(node->c1);
	}
	delete node;
}
