#ifndef RANDOM_FOREST_H_
#define RANDOM_FOREST_H_

#define MAX_TREE_NUM 1
#define IMG_SUBSET_SIZE 20
#define PX_SUBSET_SIZE 2000

#include "tree.h"
#include <cmath>
#include <algorithm>

class RandomForest{
public:
	vector <Tree*> trees_;
	
	ImageTable* image_table_;
	
	~RandomForest();
	void BuildForest(string path = "~/Pictures/");
	void PreprocessData(string path);
	vector<Pixel> SelectInput();

	vector < vector<PixelInfo> > Predict(ImageTable* test_image_table);
	double Evaluate(vector <vector<PixelInfo> > predict_result, ImageTable* test_image_table);
	vector <Offset> meanshift(vector <Offset> label,vector<vector<PixelInfo> > joints_set);
	vector <Offset> splitpixel(Tree *tmptree,Node *tmpnode,Pixel tmppixel,ImageTable* test_image_table);
};

#endif