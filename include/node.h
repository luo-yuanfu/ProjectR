#ifndef NODE_H_
#define NODE_H_

#include "pixel.h"
#include <vector>
#include <string>
#include <iostream>

//*******node class: the node in the tree
class Node{
public:
	Node();
	~Node();

	vector <Pixel> pixels_;

	int u_;
	int v_;

	double threshold_;

	Node * parent_;
	Node * left_child_;
	Node * right_child_;

	
	//calculate and return the entropy of the node itself.
	double Entropy();
	
	//calculate and return the entropy for pixels. say if we want to calculate the entropy for parent, then we feed all the pixels to it
	//if we want to calculate the entropy for left child, we feed the pixels of the left child to it
	double Entropy(vector <Pixel> pixels);

	void set_uv(int u, int v);
	void set_threshold(double threshold);
	void set_parent(Node* parent);
	void set_left_child(Node* left_child);
	void set_right_child(Node* right_child);
	
	
};

#endif