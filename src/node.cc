#include "node.h"

//*****************************************************************************
Node::Node()
{
	parent_=NULL;
	left_child_=NULL;
	right_child_=NULL;
}
//*****************************************************************************


//calculate and return the entropy of the node itself
//*****************************************************************************
double Node::Entropy()
{
	return Entropy(pixels_);
}
//*****************************************************************************


//to do: calculate and return the entropy for pixels. say if we want to calculate the entropy for parent, then we feed all the pixels to it
//if we want to calculate the entropy for left child, we feed the pixels of the left child to it
//*****************************************************************************
double Node::Entropy(vector <Pixel> pixels)
{

}
//*****************************************************************************


//to do: given the pixels in the node, calculate the label of it using meanshift
//the function Tree::CalLeafLabel will call this function to calculate the labels
//for all the leaf nodes
//*****************************************************************************
void Node::MeanShift()
{

}
//*****************************************************************************



//to do: given the pixels in the node, calculate the label of it using average
//the function Tree::CalLeafLabel will call this function to calculate the labels
//for all the leaf nodes
//*****************************************************************************
void Node::Average()
{

}
//*****************************************************************************


//set u and v
//*****************************************************************************
void Node::set_uv(int u, int v)
{
	u_=u;
	v_=v;
}
//*****************************************************************************


//set_threshold
//*****************************************************************************
void Node::set_threshold(double threshold)
{
	threshold_=threshold;
}
//*****************************************************************************


//set parent
//*****************************************************************************
void Node::set_parent(Node* parent)
{
	parent_=parent;
}
//*****************************************************************************


//set left_child_
//*****************************************************************************
void Node::set_left_child(Node* left_child)
{
	left_child_=left_child;
}
//*****************************************************************************


//set right_child_
//*****************************************************************************
void Node::set_right_child(Node* right_child)
{
	right_child_=right_child;
}
//*****************************************************************************