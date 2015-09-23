#ifndef TREE_H_
#define TREE_H_

#define MAX_TREE_DEPTH 8

#include "node.h"

//*******tree class
class Tree{
public:
	Node* root_;
	ImageTable* image_table_;

	void InitRoot(vector <Pixel> pixels);
	void BuildTree();

};

#endif