/*
 * Bitmanipulator.cpp
 *
 *  Created on: Apr 24, 2018
 *      Author: Joel
 */

#include "Bitmanipulator.h"
#include <math.h>
#include <iostream>

//https://stackoverflow.com/questions/53161/find-the-highest-order-bit-in-c

/*
 * creates a mask where areas around slected area are 1 and others are 0 so if a & operator is used only selected bits will be deleted
 * originalsize - the bit number of the data size(8 for char, 16 for short/WORD etc)
 * pos - the bit numer that the unmasked part will start at
 * size - the bits covered by the mask
 * returns a mask of 0s and 1s where chosen areas are 0s
 */
int createmask(int originalsize, int pos, int size) {
	return (int) (1<<originalsize)-1 - (((1 << size) - 1)<<pos);
}

/*
 *overwrites specified bits with other values
 *original - the char to change the bits of
 *piece - the bits that will be put into the new number
 *pos - the position that the new number will go
 * size - the length of the new number in bits
 * returns the original with an inserted value
 */
int insertBits(int original, int piece, int pos, int size) {
	return  (original & createmask(4*sizeof(int), pos, size))
			+ (piece << pos);
}
/*
 * gets the bytes selected from another number
 * original - number to search from
 * pos - the position of the number that will be obtained
 * size - the length of the number in bits wanted to be obtained
 * returns the bits at in the wanted positions
 */
int getBits(int original, int pos, int size) {
	return (original
			& ((hibit(original) << 1)
					+ ~createmask(4*sizeof(original), pos, size))) >> pos;
}
/*
 * finds the highest bit of a number
 * n - the number to find the highest bit of
 * returns the value of the highest 1
 */
int hibit(unsigned int n) {
	n |= (n >> 1); //fill up number with ones until final
	n |= (n >> 2);
	n |= (n >> 4);
	n |= (n >> 8);
	n |= (n >> 16);
	return n - (n >> 1); //get rid of all ones but last
}
