#ifndef RANDOM_FOREST_H_
#define RANDOM_FOREST_H_

#define MAX_TREE_NUM 10

#include "tree.h"

class RandomForest{
public:
	vector <Tree*> trees_;
	
	ImageTable* image_table_;
	
	~RandomForest();
	void BuildForest(string path = "~/Pictures/");
	void PreprocessData(string path);
	vector<Pixel> SelectInput();
	
	//Label type defined in image_table.h
	Label Predict(string filename);
};

#endif