#ifndef TREE_H_
#define TREE_H_

#define MAX_TREE_DEPTH 8
#define SAMPLE_UV_NUM 100
#define SAMPLE_THRESHOLD_NUM 50

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
	void ConstructTree();
	void BuildTree(int depth, Node* cur_node);//build the tree, without calculating the labels for the leaf nodes
	void CalLeafLabel(); //calculate the labels for the leaf nodes
	void RandomSample(int &u, int &v);
	void RandomSample(double &threshold);
	double InformationGain(Node *cur_node, vector<Pixel> left_pixel, vector<Pixel> right_pixel);

	void set_image_table(ImageTable* image_table);
};

#endif