#include "node.h"

Node::Node()
{
	parent_=NULL;
	left_child_=NULL;
	right_child_=NULL;
}

//to do: calculate feature value for pixel using u and v
double Node::FeatureValue(Pixel piexl, int u, int v)
{

}

//to do: calculate the entropy for pixels. say if we want to calculate the entropy for parent, then we feed all the pixels to it
//if we want to calculate the entropy for left child, we feed the pixels of the left child to it
double Node::Entropy(vector <Pixel> pixels)
{

}

void Node::set_uv(int u, int v)
{
	u_=u;
	v_=v;
}

void Node::set_threshold(double threshold)
{
	threshold_=threshold;
}

void Node::set_parent(Node* parent)
{
	parent_=parent;
}

void Node::set_left_child(Node* left_child)
{
	left_child_=left_child;
}

void Node::set_right_child(Node* right_child)
{
	right_child_=right_child;
}