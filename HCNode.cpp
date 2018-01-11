/*
 *Author: Simar Chhabra
 *File has an operator overloaded method to compare nodes in pqueue 
 */

#include <iostream>
#include "HCNode.h"

using namespace std;

//account for overloaded operator to compare the nodes in pqueue

bool HCNode::operator<(const HCNode& other)
{
	if(count != other.count)
	{
		return (count > other.count);
	}
	//if counts are equal do a symbol comparison
	else
	{
		return symbol < other.symbol;
	}	
}
