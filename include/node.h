#ifndef NODE_H_
#define NODE_H_

#include "pixel.h"
#include <vector>

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

	double FeatureValue(Pixel piexl, int u, int v);//calculate feature value for pixel using u and v

	//calculate the entropy for pixels. say if we want to calculate the entropy for parent, then we feed all the pixels to it
	//if we want to calculate the entropy for left child, we feed the pixels of the left child to it
	double Entropy(vector <Pixel> pixels);

	void set_uv(int u, int v);
	void set_threshold(double threshold);
	
};

#endif