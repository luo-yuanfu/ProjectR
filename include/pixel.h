#ifndef PIXEL_H_
#define PIXEL_H_

#include "image_table.h"

using namespace std;

typedef PixelInfo Offset; //offset has the same data structure with PixelInfo

//*******pixel class
class Pixel{
public:

	PixelInfo info_;

	int image_index_;//the index of the image the pixel belongs to; for calculating the neighbors

	vector <Offset> label_;// label is the vector of offset; offset is the difference between pixel and joints

	Pixel();
	Pixel(int image_index, PixelInfo info, ImageTable *image_table);
	~Pixel();

	void CalLabel(ImageTable *image_table);//calculate the labels for the pixels
};


#endif
