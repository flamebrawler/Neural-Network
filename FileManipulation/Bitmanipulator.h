/*
 * Bitmanipulator.h
 *
 *  Created on: Apr 24, 2018
 *      Author: Joel
 */

#ifndef BITMANIPULATOR_H_
#define BITMANIPULATOR_H_

int hibit(unsigned int n);

int createmask(int originalsize, int pos, int size);

int insertBits(int original, int piece, int pos,
		int size);

int getBits(int original, int pos, int size);

#endif /* BITMANIPULATOR_H_ */
