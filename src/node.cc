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


//to do: given the pixels in the node, calculate the label of it using meanshift method
//the function Tree::CalLeafLabel will call this function to calculate the labels
//for all the leaf nodes
//*****************************************************************************
void Node::MeanShift()
{
	//call set_label() to set label_ for the leaf node after calculating the label
}
//*****************************************************************************



//to do: given the pixels in the node, calculate the label of it using averaging method
//the function Tree::CalLeafLabel will call this function to calculate the labels
//for all the leaf nodes
//*****************************************************************************
void Node::Average()
{
	//call set_label() to set label_ for the leaf node after calculating the label
}
//*****************************************************************************



//set label_
//*****************************************************************************
void Node::set_label(vector <Offset> label)
{
	label_=label;
}
//*****************************************************************************



//get label_ of node, if the node is leaf node
//*****************************************************************************
vector <Offset>  Node::get_label()
{
	if(left_child_!=NULL || right_child_ != NULL){
		cout<<"not leaf node"<<endl;
		return label_;//since this is not a leaf node, label_ is empty, 
		//need to check label_.size() in the function that calls this function
	}
	return label_;
}


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

