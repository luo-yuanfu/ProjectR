#ifndef PIXEL_H_
#define PIXEL_H_


using namespace std;

//*******coordinate
struct Coord{
	int x;
	int y;
};

//*******pixel class
class Pixel{
public:

	Coord location_;//the coordinate of the pixel

	int depth_;//depth of the pixel

	//int rgb_[3]; //for potential future use

	int image_index_;//the image index the pixel belongs to

	//we dont need to store label for the pixel, because we 
	//can get the label from the image got by image_index
};


#endif