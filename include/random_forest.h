#ifndef RANDOM_FOREST_H_
#define RANDOM_FOREST_H_

#define MAX_TREE_NUM 10

#include "tree.h"
#include <cmath>

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
};

#endif