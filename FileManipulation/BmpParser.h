/*
 * BmpParser.h
 *
 *  Created on: Apr 17, 2018
 *      Author: kambjoel430
 */

#ifndef BMPPARSER_H_
#define BMPPARSER_H_

#include<iostream>
#include <fstream>
using namespace std;

class pixel {

private:
	unsigned char *color;
public:
	int bits; //bits per pixel
	int bytes; //bytes taken up

	pixel(int bits);

	virtual ~pixel();

	void resize(int bytes, const unsigned char* values);
	int print();
	unsigned char & get(int pos);
};
//portion needed in header to make bmp file
struct bitmapheader {
	unsigned short field;
	unsigned int size;
	unsigned short reserved1;
	unsigned short reserved2;
	unsigned int offset;
};
//portion needed in header to make bmp file
struct dibheader {
	unsigned int headersize;
	unsigned int width;
	unsigned int height;
	unsigned short colorplanes;
	unsigned short bitsperpixel;
	unsigned int compression;
	unsigned int imagesize;
	unsigned int horzres;
	unsigned int vertres;
	unsigned int colors;
	unsigned int importantcolors;
};

pixel** topxls(unsigned char* pixels,int bitrate,int size);
void changebitrate(pixel** pixels, int bits, int pixelnum);
pixel** read(string filename);
void writebmp(string filename, bitmapheader bh, dibheader dh, pixel** pixels);
int getHeight(string filename);
int getWidth(string filename);

/*
 * getPos gets a value located at the intended position of the file on a byte scale
 * filename - the  name of the file to search
 * pos - the position of in the file to get the data
 * T - the bytes read based on the size of the type
 * returns the data at the position
 */

template<class T> T getPos(string filename, unsigned int pos) {
	ifstream f(filename.c_str(), ios::in | ios::binary);
	if (f.fail()) { //check if file isn't read
		cerr << "file not opened" << endl;
		exit(1);
	}
	f.seekg(pos, ios::beg); //go to the wanted position
	T data;
	f.read((char*) &data, sizeof(T)); //get the data
	f.close();
	return data;
}

#endif /* BMPPARSER_H_ */
