#ifndef IMAGE_TABLE_H_
#define IMAGE_TABLE_H_

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>

using namespace std;

struct Coord{
	int x;
	int y;
};

struct BoundBox{
	Coord start;//the left and top most pixel position of bounding box;
	int width;//the width of the bounding box
	int height;//the height of the bounding box
};

//*******pixel information
struct PixelInfo{
	int x;//x position of the pixel
	int y;//y position of the pixel
	int depth;//depth of the pixel
};

//to do: modify "int" to define image data type
typedef int ImageType; //the "int" here is just for sucessful compilation. use the image type you prefer to replace it

struct ImageEntry{
	ImageType image;
	BoundBox bounding_box;
	int image_id;//the id of the image, for quick indexing. this param seems unnecessary. because we can index by the index of vector
	vector<PixelInfo> joints;//the joints information;
};


//******** image lookup table
class ImageTable{
public:
	vector <ImageEntry *> images_;

public:
	ImageTable();
	ImageTable(string path);
	~ImageTable();

	void LoadImages(string path);

	ImageEntry * get_image(int index);

};

#endif