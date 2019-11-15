/*
 * Parser.h
 *
 *  Created on: Mar 5, 2018
 *      Author: Joel
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>

std::string* parse(std::string input,std::string seperators);

std::string skip(std::string input, std::string skipped);

double getD(std::string input);

std::string getS(double input);

std::string combine(const char* input,const char* input2);

bool contains(std::string subject, std::string find);

long* toLong(long* base,const std::string* input, int size, int offset) ;

long** toLong(long** base,std::string const*const* input, int rows, int columns, int offset);

#endif /* PARSER_H_ */
