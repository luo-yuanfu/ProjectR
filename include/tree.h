#ifndef TREE_H_
#define TREE_H_

#define MAX_TREE_DEPTH 8
#define SAMPLE_NUM 30

#include "node.h"
#include "image_table.h"

//*******tree class
class Tree{
public:
	Node* root_;
	ImageTable* image_table_;

	Tree();
	~Tree();

	//calculate feature value for pixel using u and v
	double FeatureValue(Pixel piexl, int u, int v);
	void InitRoot(vector <Pixel> pixels);
	void BuildTree(int depth, Node* cur_node);
	void RandomSample(int &u, int &v, double &threshold);
	double InformationGain(Node *cur_node, vector<Pixel> left_pixel, vector<Pixel> right_pixel);

	void set_image_table(ImageTable* image_table);
};

#endif