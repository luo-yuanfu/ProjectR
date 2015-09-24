#ifndef IMAGE_TABLE_H_
#define IMAGE_TABLE_H_

#include <string>
#include <vector>
#include <iostream>

using namespace std;

//to do: modify "int" to define image data type
typedef int ImageType; //the "int" here is just for sucessful compilation. use the image type you prefer to replace it

//******* x,y,z coordinate 
struct Location{
	int x;
	int y;
	int z;
};

struct Label{
	vector<Location> joint_locs; //x,y,z coordinate for the joints
};

struct ImageEntry{
	ImageType image;
	int image_id;//the id of the image, for quick indexing. this param seems unnecessary. because we can index by the index of vector
	Label label;
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