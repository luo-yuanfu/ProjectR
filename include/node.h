#ifndef NODE_H_
#define NODE_H_

#include "pixel.h"
#include <vector>
#include <string>
#include <iostream>

#define BANDWIDTH 0.75
//*******node class: the node in the tree
class Node{
public:
	Node();
	~Node();

	vector <Pixel> pixels_;

	std::pair<int,int> u_;
	std::pair<int,int> v_;

	double threshold_;

	//representative offset of the leaf node. for nodes that are leaf nodes, do not need to calculate this
	vector <Offset> label_;

	Node * parent_;
	Node * left_child_;
	Node * right_child_;

	
	//calculate and return the entropy of the node itself.
	double Entropy();
	
	//calculate and return the entropy for pixels. say if we want to calculate the entropy for parent, then we feed all the pixels to it
	//if we want to calculate the entropy for left child, we feed the pixels of the left child to it
	double Entropy(vector <Pixel> pixels);
        double Cal_DetCov(const vector<vector<int> > M);
	//two ways to calculate the label of the node
	void MeanShift();
	void Average();

	void set_uv(std::pair<int,int> u, std::pair<int,int> v);
	void set_threshold(double threshold);
	void set_parent(Node* parent);
	void set_left_child(Node* left_child);
	void set_right_child(Node* right_child);
	void set_label(vector <Offset> label);
	vector <Offset> get_label();//get label_ of node, if the node is leaf node

	
	
};

#endif
