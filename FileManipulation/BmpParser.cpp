/*
 * BmpParser.cpp
 *
 *  Created on: Apr 17, 2018
 *      Author: Joel
 */

#include "BmpParser.h"
#include "Bitmanipulator.h"
#include <math.h>
#include <iostream>

//https://stackoverflow.com/questions/9296059/read-pixel-value-in-bmp-file
//https://en.wikipedia.org/wiki/BMP_file_format
//https://stackoverflow.com/questions/141525/what-are-bitwise-shift-bit-shift-operators-and-how-do-they-work

using namespace std;

pixel::pixel(int bits) {
	this->bits = bits;

	if (bits <= 8)//calculates bytes used by pixel
		bytes = 1;
	else
		bytes = bits / 8;

	color = new unsigned char[bytes];
}

pixel::~pixel() {
	delete[] color;
}

pixel** topxls(unsigned char*pixels,int bitrate,int size){
	int bytes;
	if(bitrate ==32)
		bytes = 4;
	else if(bitrate == 24)
		bytes = 3;
	else if(bitrate == 16)
		bytes = 2;
	else
		bytes = 1;

	pixel** image = new pixel*[size/bytes];
	for(int i = 0;i<size/bytes;i++){
		image[i] = new pixel(bitrate);
		for(int j=0;j<bytes;j++){
			image[i]->get(j) = pixels[i*j];
		}
	}
	return image;
}

/*
 * changes all pixels to another bitrate for example: 24 bit, 8 bit, 1 bit etc from 32 or 24 bit
 * pixels - the pixels to change bitrate of
 * bits - the bitrate to change to
 * pixelnum - the number of pixels inputted
 */
void changebitrate(pixel** pixels, int bits, int pixelnum) {
	if (pixels[0]->bits >=24) { //cant if lower than 24 bits

		if (bits == 16) { //if changing to 16-bit image

			for (int i = 0; i < pixelnum; i++) { //remake each pixel
				pixels[i]->bits = 16; //change info

				unsigned char temp[2]; //change each pixel to rounded versionsof original
				temp[0] = 0;
				temp[1] = 0;

				temp[0] += insertBits(0, pixels[i]->get(0) >> 3, 0, 5); //Blue
				temp[0] += insertBits(0, (pixels[i]->get(1) << 2) >> 7, 5, 3); //Green
				temp[1] += insertBits(0, (pixels[i]->get(1)) >> 6, 0, 2); //Green
				temp[1] += insertBits(0, pixels[i]->get(2) >> 3, 2, 5); //Red

				if (pixels[0]->bits == 32) //Alpha
					temp[1] += insertBits(0, pixels[i]->get(3) >> 7, 7, 1);
				else
					//if 24 bit image without alpha initially
					temp[1] += insertBits(0, 1, 7, 1);

				pixels[i]->resize(2, temp);
			}

		} else if (pixels[0]->bits == 32 && bits == 24) { //changing to 24-bit image

			for (int i = 0; i < pixelnum; i++) { //repeat for each pixel in image
				unsigned char temp[3];

				pixels[i]->bits = 24;

				temp[0] = pixels[i]->get(0); //get rid of alpha
				temp[1] = pixels[i]->get(1);
				temp[2] = pixels[i]->get(2);

				pixels[i]->resize(3, temp);
			}

		} else if (bits < 16) { //changing to a 1,2,4,8 bit image

			for (int i = 0; i < pixelnum; i++) {

				pixels[i]->bits = bits;

				unsigned char temp[1];

				int highest = pixels[i]->get(0);
				int lowest = pixels[i]->get(0);
				for (int j = 1; j < 3; j++) { //calculate highest and lowest color values to get darkness of pixel
					if (pixels[i]->get(j) > highest)
						highest = pixels[i]->get(j);
					if (pixels[i]->get(j) < lowest)
						lowest = pixels[i]->get(j);
				}

				temp[0] = insertBits(0,
						(int) ((highest + lowest) >> 1) >> (8 - bits), 0, bits); //resize the pixel so it is the right number of bits

				pixels[i]->resize(1, temp);
			}
		}
	}
}
/*
 * get() gets the color at the position with error proofing
 * pos - the position in the array to get the color of
 * returns reference to the position in the array
 */
unsigned char& pixel::get(int pos) {
	if (pos < bytes && pos >= 0) //check if outside of bounds
		return color[pos];
	return color[0];
}
/*
 * print prints the entire array
 * returns the array as one integer
 */
int pixel::print() {
	int result = 0;
	for (int i = 0; i < bytes; i++)
		result += color[i] << (8 * i);
	return result;
}
/*
 * resize changes the amount of bytes in a pixel
 * bytes - the new bytes in the pixel
 * values - the array to change the pixel's array to
 */
void pixel::resize(int bytes, const unsigned char* values) {
	delete[] color;
	unsigned char* temp = new unsigned char[bytes];
	for (int i = 0; i < bytes; i++) //reload array
		temp[i] = values[i];
	color = temp;
	this->bytes = bytes;
}
/*
 * writebmp() creates a bmp file use data inputted
 * filename - the name of the file to write to
 * bh - the bitmap header to use in the file
 * dh - the dib header to use in the file
 * pixels - the pixel array to use for the image
 */
void writebmp(string filename, bitmapheader bh, dibheader dh, pixel** pixels) {
	ofstream f(filename.c_str(), ios::out | ios::binary);
	if (f.fail()) { //check for errors
		cerr << "file not opened" << endl;
		exit(1);
	}
	//print BM header
	f.write((char*) &bh.field, sizeof(bh.field));
	f.write((char*) &bh.size, sizeof(bh.size));
	f.write((char*) &bh.reserved1, sizeof(bh.reserved1));
	f.write((char*) &bh.reserved2, sizeof(bh.reserved2));
	f.write((char*) &bh.offset, sizeof(bh.offset));
	//print DIM header
	f.write((char*) &dh, sizeof(dh));

	const int zero = 0;
	int ppb = 1;
	if (pixels[0]->bits < 8)
		ppb = 8 / pixels[0]->bits;

	//create color table with grayscale colors only
	if (pixels[0]->bits <= 8)
		for (int i = 0; i < 256;
				i += (int) 255 / ((1 << pixels[0]->bits) - 1)) {
			int zero = 0;

			f.write((char*) &i, 1);
			f.write((char*) &i, 1);
			f.write((char*) &i, 1);
			f.write((char*) &zero, 1);
		}

	int rowsize = ((pixels[0]->bits * dh.width + 31) >> 5) * 4; //the width of the row with buffer

	//print all pixels
	for (unsigned int y = 0; y < dh.height; y++) { //for each veritcal pixel
		for (unsigned int x = 0; x < dh.width; x += ppb) { //for each horizontal pixel

			int data = 0;//buffer for printing the data

			if (pixels[0]->bits <= 8)//if there is more than pixel per byte
				for (int p = 0; p < ppb && p + x < dh.width; p++)//fill the data up
					data += (pixels[p + y * dh.width + x]->print() << 8)
							>> (pixels[0]->bits * (p + 1));
			else //more than one byte per pixel
				data = pixels[y * dh.width + x]->print();

			f.write((char*) &data, pixels[y * dh.width + x]->bytes);//print out the bytes
		}
		f.write((char*) &zero, rowsize - pixels[0]->bytes * dh.width / ppb);//put in buffers
	}
	f.close();
}
/*
 * getWidth finds the width of a bmp image based on the file
 * filename - the name of the file being read
 * returns the width of the image
 */
int getWidth(string filename) {
	ifstream f(filename.c_str(), ios::in | ios::binary);

	if (f.fail()) { //check for errors with files
		cerr << "file not opened" << endl;
		exit(1);
	}

	f.seekg(18, ios::beg); //go to wanted position
	int width;
	f.read((char*) &width, 4); //get the data
	f.close();
	return width;
}
/*
 * getHeight finds the width of a bmp image based on the file
 * filename - the name of the file being read
 * returns the height of the image
 */
int getHeight(string filename) {
	ifstream f(filename.c_str(), ios::in | ios::binary);

	if (f.fail()) { //check for image not read
		cerr << "file not opened" << endl;
		exit(1);
	}

	f.seekg(22, ios::beg); // go to wanted positon
	int height;
	f.read((char*) &height, 4); //get the data
	f.close();
	return height;
}
/*
 * read() reads a bmp file and makes a list of pixels from it
 * filename - the name of the file to read
 * returns an array of pixels corresponding to the image
 */
pixel** read(string filename) {
	ifstream f(filename.c_str(), ios::in | ios::binary);
	if (f.fail()) {//check for errors
		cerr << "file not opened" << endl;
		exit(1);
	}

	unsigned char info[54];// bytes of header

	f.read((char*) &info, 54);//read the header

	int width = *(int*) &info[18];//get info from header
	int height = *(int*) &info[22];
	short bpp = *(short*) &info[28];//bits per pixel
	short bytepp = bpp / 8;//bytes per pixel

	if (bytepp < 1)
		bytepp = 1;

	int row_padded = (width * bytepp + bytepp) & (~bytepp);//the size of each row with buffer

	f.seekg(*(int*) &info[10]); //go to beggining of list

	pixel** result = new pixel*[width * height];//list of pixels

	for (int i = 0; i < height; i++) {

		unsigned char* temp = new unsigned char[row_padded];
		f.read((char*) temp, row_padded);//read row

		for (int j = 0; j < width; j++) {
			result[j + i * width] = new pixel(bpp);//convert chars to pixels
			for (int b = 0; b < result[j + i * width]->bytes; b++) {
				result[j + i * width]->get(b) = temp[bytepp * j + b];
			}
		}
		delete[] temp;
	}

	f.close();
	return result;

}
