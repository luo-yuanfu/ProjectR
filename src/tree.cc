#include "tree.h"

//*****************************************************************************
Tree::Tree()
{
	root_=NULL;
	image_table_=NULL;
}
//*****************************************************************************


//to do: delete all the nodes in the tree
//*****************************************************************************
Tree::~Tree()
{
	
}
//*****************************************************************************


//to do: calculate and return feature value for pixel using u and v
//need to use image_table_ to get the pixels 
//*****************************************************************************
double Tree::FeatureValue(Pixel piexl, int u, int v)
{

}
//*****************************************************************************


//init root node
//*****************************************************************************
void Tree::InitRoot(vector <Pixel> pixels)
{
	root_=new Node();
	root_->pixels_=pixels;
}
//*****************************************************************************


//build the tree, and calculate the labels for the leaf nodes
//*****************************************************************************
void Tree::ConstructTree()
{
	BuildTree(1, root_);
	CalLeafLabel();
}
//*****************************************************************************


//*****************************************************************************

//build decision tree
//*****************************************************************************
void Tree::BuildTree(int depth, Node* cur_node)
{
	if(depth>MAX_TREE_DEPTH)  return;
	
	//if there are no enough pixels in the node, we can stop going further
	if(cur_node->pixels_.size()<0.005*root_->pixels_.size())
		return;
	
	double max_info_gain=0; //the max information gained
	int best_u=0, best_v=0;
	double best_threshold=0;
	
	//find the best u v and threshold to split the pixels. currently select the best from SAMPLE_NUM randomly sampled <u,v,threshold>
	for(int iter=0; iter<SAMPLE_UV_NUM; iter++){
		int u,v;
		double threshold;
		vector<Pixel> left_pixel;
		vector<Pixel> right_pixel;

		RandomSample(u,v);

		for(int iter2=0; iter2<SAMPLE_THRESHOLD_NUM; iter2++){
			RandomSample(threshold);
			for(int i=0; i<cur_node->pixels_.size(); i++){
				if(FeatureValue(cur_node->pixels_.at(i), u, v)<threshold)
					left_pixel.push_back(cur_node->pixels_.at(i));
				else right_pixel.push_back(cur_node->pixels_.at(i));
			}		
			double info_gain=InformationGain(cur_node, left_pixel, right_pixel);
			if(info_gain>max_info_gain){
				max_info_gain=info_gain;
				best_u=u;
				best_v=v;
				best_threshold=threshold;
			}
			
			left_pixel.clear();
			right_pixel.clear();
		}
	}
	
	Node *left_child=new Node();
	Node *right_child=new Node();
	for(int i=0; i<cur_node->pixels_.size(); i++){
			if(FeatureValue(cur_node->pixels_.at(i), best_u, best_v)<best_threshold)
				left_child->pixels_.push_back(cur_node->pixels_.at(i));
			else right_child->pixels_.push_back(cur_node->pixels_.at(i));
	}
	
	//link children and parent
	left_child->set_parent(cur_node);
	right_child->set_parent(cur_node);
	cur_node->set_left_child(left_child);
	cur_node->set_right_child(right_child);
	
	//store u,v,and threshold for current node
	cur_node->set_uv(best_u, best_v);
	cur_node->set_threshold(best_threshold);
	
	//build subtree for left and right children
	BuildTree(depth+1, cur_node->left_child_);
	BuildTree(depth+1, cur_node->right_child_);
}
//*****************************************************************************



//calculate the information gain
//*****************************************************************************
double Tree::InformationGain(Node *cur_node, vector<Pixel> left_pixel, vector<Pixel> right_pixel)
{
	return cur_node->Entropy()-(left_pixel.size()/double(cur_node->pixels_.size())*cur_node->Entropy(left_pixel) + right_pixel.size()/double(cur_node->pixels_.size())*cur_node->Entropy(right_pixel));
}
//*****************************************************************************


//to do: randomly sample a legal pair <u,v>
//*****************************************************************************
void Tree::RandomSample(int &u, int &v)
{
	
}
//*****************************************************************************


//to do: randomly sample a legal threshold
//*****************************************************************************
void Tree::RandomSample(double &threshold)
{
	
}
//*****************************************************************************


//to do: calculate the labels for the leaf node
//hint: can use the same way I build the tree to find the leaf nodes and calculate
//their labels, and will need to redefine the parameters for this function.
//call Node::MeanShift or Node::Average function here
//*****************************************************************************
void Tree::CalLeafLabel()
{

}
//*****************************************************************************


//set image_table_
//*****************************************************************************
void Tree::set_image_table(ImageTable* image_table)
{
	image_table_=image_table;
}
//*****************************************************************************
