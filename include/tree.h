#ifndef TREE_H_
#define TREE_H_

#define MAX_TREE_DEPTH 8
#define SAMPLE_UV_NUM 100
#define SAMPLE_THRESHOLD_NUM 50
#define UV_RANGE 10
#define MEANSHIFT

#include "node.h"
#include "image_table.h"

//*******tree class
class Tree{
public:
	Node* root_;
	ImageTable* image_table_;

	Tree();
	~Tree();
	void FreeNode(Node* cur_node);

	//calculate feature value for pixel using u and v
	double FeatureValue(Pixel piexl, std::pair<int,int> u, std::pair<int,int> v);
	double FeatureValue(Pixel piexl, std::pair<int,int> u, std::pair<int,int> v, ImageTable* test_image_table);
	void InitRoot(vector <Pixel> pixels);
	void ConstructTree();
	void BuildTree(int depth, Node* cur_node);//build the tree, without calculating the labels for the leaf nodes
	void CalLeafLabel(Node* cur_node); //calculate the labels for the leaf nodes
	void RandomSample(std::pair<int,int> &u, std::pair<int,int> &v);
	double InformationGain(Node *cur_node, vector<Pixel> left_pixel, vector<Pixel> right_pixel);

	void set_image_table(ImageTable* image_table);
};

#endif
