/*
 * Parser.cpp
 *
 *  Created on: Mar 5, 2018
 *      Author: kambjoel430
 */

#include "Parser.h"

using namespace std;

//https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c

/*
 * input - the subject string
 * seperators - the string used to split the original string
 * returns - array of strings around each instance of the seperator
 *
 * first term is the size of the array
 * note: maximum splits is 200 before not more splits are made
 */
string* parse(string input, string seperators) {
	size_t pos = 0;
	string token;
	string* result = new string[200];
	int a = 1; //position in the array

	while ((pos = input.find(seperators)) != string::npos && a < 200) { //iterate through the string
		token = input.substr(0, pos);
		result[a] = token; //use string
		input.erase(0, pos + seperators.length()); //shorten the original string so parts are not reused
		a++;
	}
	string* r = new string[a + 1]; //resize array so no memory is wasted on empty space
	for (int i = 1; i <= a; i++) {
		r[i] = result[i];
	}
	result = r;
	result[0] = getS(a); //make first term the size of the array

	result[a] = input;

	return result;
}

/*
 * skip() skips a selected string in another string so they are essentially deleted
 * input - the string subject
 * skipped - the string being skipped
 * returns - string of the input without the skipped strings in it
 */
string skip(string input, string skipped) {
	size_t pos = 0;

	while ((pos = input.find(skipped)) != string::npos) //iterate through the string
		input.erase(pos, skipped.length()); //shorten the original string so parts are not reused

	return input;
}
/*
 * getD() converts from string to double
 * input -  string to be converted to double
 * returns - double of inputed value
 */
double getD(string input) {
	return atof(input.c_str());
}
/*
 * getS() converts from double to string
 * input -  double to be converted to string
 * returns - string of inputed value
 */
string getS(double input) {
	char buffer[50];
	snprintf(buffer,50,"%lf", input); //convert from double to string

	return string(buffer);
}

/*
 * combine() combines 2 char*
 * input/input2 - inputs being combines
 * returns - the strings side-by-side as one string from left(input) to right(input2)
 */
string combine(const char* input, const char* input2) {
	char buffer[50];
	snprintf(buffer,50, "%s%s", input, input2);
	return string(buffer);
}

/*
 * contains() checks whether a string is contained in another string
 * subject - the string checked
 * find - the string the is being looked for
 * returns whether the string is contained in the subject
 */
bool contains(string subject, string find) {
	while ((subject.find(find)) != string::npos) //iterates through function for string
		return true;

	return false;
}

/*
 * toLong converts a string* to long*
 * base - the long* variable that will be writen to. used so no unnecessary memory is allocated
 * input - the string* array that will be copied
 * size - the dimension of the new array [row]
 * offset - the number of spaces at the begining of the input's columns that will not be used
 * returns - new value of base
 *
 * note: offset + columns must not exceed columns of input
 */
long* toLong(long* base,const string* input, int size, int offset) {
	for (int c = 0; c < size; c++) { //for each column
		base[c] = (long) getD(input[c+offset]); //convert from string to decimal
	}
	return base;

}
/*
 * toLong converts a string** to long**
 * base - the long** variable that will be writen to. used so no unnecessary memory is allocated
 * input - the string** array that will be copied
 * rows/columns - the dimensions of the new array [row][column]
 * offset - the number of spaces at the begining of the input's columns that will not be used
 * returns - new value of base
 *
 * note: offset + columns must not exceed columns of input
 */
long** toLong(long** base,string const*const* input, int rows, int columns, int offset) {

	for (int r = 0; r < rows; r++) { //for each row
		long* Ptr = new long[columns];

		for (int c = 0; c < columns; c++) { //for each column
			Ptr[c] = (long) getD(input[r][c + offset]); //convert from string to decimal
		}
		delete input[r];

		base[r] = Ptr;
	}
	return base;
}
