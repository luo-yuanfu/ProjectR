#ifndef IMAGE_TABLE_H_
#define IMAGE_TABLE_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>


using namespace std;
using namespace cv;

#define Image_Width  640.0
#define Image_Height 320.0
#define FOCUS 533.0

struct BoundBox{
	int x,y;						//(x,y) position of the left-top x is column y is row
	int width;						//the number of columns
	int height;						//the number of rows
};


struct PixelInfo{
	int x,y;						//position of the pixel x is column y is row
	int depth;					//depth of the pixel
};

struct ImageEntry{
	//Mat image;
	int **image_depth;
	int width,height;
	BoundBox bounding_box;
	vector<PixelInfo> joints;		//the joints information;
};


//******** image lookup table
class ImageTable{

private:
	float delta;					// the threshold percentage for the bounding box range(0,1)
	BoundBox CalcBoundBox(Mat image, vector<PixelInfo> joints);
	BoundBox CalcBoundBox(Mat image);
	inline void TransCoord(float &x, float &y, float &z){x = (x * FOCUS / z + Image_Width / 2); y = (y * FOCUS / z + Image_Height / 2); z = -z;};
public:
	vector <ImageEntry *> images_;
	ImageTable();
	ImageTable(float _delta);
	ImageTable(string path);
	~ImageTable();

	void LoadImages(string Ipath, string Lpath, string Iext);		// load image for train images
	void LoadImages(string Iname);									// load image for test image
	void SetDelta(float _delta){delta=_delta;}


	ImageEntry * get_image(int index);
};

#endif
