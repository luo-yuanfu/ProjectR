#include "tree.h"
#include <stdlib.h>
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
	FreeNode(root_);	
}
//*****************************************************************************
void Tree::FreeNode(Node* cur_node)
{
	if(cur_node->left_child_ != NULL) { // cur_node is not leaf node
          FreeNode(cur_node->left_child_);
          FreeNode(cur_node->right_child_);
	  delete cur_node;
	}else {
	  // cur_node is leaf node, to delete node
	  delete cur_node;
	}
}
//*****************************************************************************


//to do: calculate and return feature value for pixel using u and v
//need to use image_table_ to get the pixels 
//*****************************************************************************
double Tree::FeatureValue(Pixel piexl, std::pair<int,int> u, std::pair<int,int> v)
{
  	int x     = piexl.info_.x;
  	int y     = piexl.info_.y;
  	int depth = piexl.info_.depth;
	int imgIdx = piexl.image_index_;
	int ux = x + u.first/depth;
	int uy = y + u.second/depth;
	int vx = x + v.first/depth;
	int vy = y + v.second/depth;

	//*** To edit based on ImageType defined in ImageTable Class
	ImageType I = image_table_->images_[imgIdx]->image;
	const unsigned int I_height = I.size();
	const unsigned int I_width = I[0].size();
	int depth_u, depth_v;
	if(ux > I_width-1 || uy > I_height-1) {  // pixel u out of image boundary
          depth_u = 255; // treat as background (white)
	}else {
	  depth_u = I[uy][ux];
	}
	if(vx > I_width-1 || vy > I_height-1) {  // pixel v out of image boundary
          depth_v = 255; // treat as background (white)
	}else {
	  depth_v = I[vy][vx];
	}
	return ((double)(depth_u - depth_v));
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
	CalLeafLabel(root_);
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
	std::pair<int,int> best_u, best_v;
	double best_threshold=0;
	
	//find the best u v and threshold to split the pixels. currently select the best from SAMPLE_NUM randomly sampled <u,v,threshold>
	for(int iter=0; iter<SAMPLE_UV_NUM; iter++){
	        std::pair<int, int> u, v;
		double threshold;
		vector<Pixel> left_pixel;
		vector<Pixel> right_pixel;

		RandomSample(u,v);

		// determine range of features, then decide threshold values
		double max_feat = FeatureValue(cur_node->pixels_.at(0), u, v);
		double min_feat = FeatureValue(cur_node->pixels_.at(0), u, v);
		for(int i=1; i<cur_node->pixels_.size(); i++){
			double curr_feat = FeatureValue(cur_node->pixels_.at(i), u, v);
			if(curr_feat < min_feat) min_feat = curr_feat;
			if(curr_feat < max_feat) max_feat = curr_feat;
		}
                double step = (max_feat-min_feat)/(SAMPLE_THRESHOLD_NUM-1);
		for(int iter2=0; iter2<SAMPLE_THRESHOLD_NUM; iter2++){
			threshold = min_feat + iter2*step;
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
void Tree::RandomSample(std::pair<int,int>& u, std::pair<int,int>& v)
{
	const int max_uv = UV_RANGE; // +- 10 pixels boundary to search for uv
	u.first  = rand()%(2*max_uv+1)-max_uv;
	u.second = rand()%(2*max_uv+1)-max_uv;
	v.first  = rand()%(2*max_uv+1)-max_uv;
	v.second = rand()%(2*max_uv+1)-max_uv;
}
//*****************************************************************************


//to do: calculate the labels for the leaf node
//hint: can use the same way I build the tree to find the leaf nodes and calculate
//their labels, and will need to redefine the parameters for this function.
//call Node::MeanShift or Node::Average function here
//*****************************************************************************
void Tree::CalLeafLabel(Node* cur_node)
{
  	// assume when building tree, when splitting, always create left and right child
	if(cur_node->left_child_ != NULL) {    // cur_node is not leaf node
          CalLeafLabel(cur_node->left_child_);
          CalLeafLabel(cur_node->right_child_);
	}else {
          // cur_node is a leaf node
#ifdef MEANSHIFT
	  cur_node->MeanShift();
#elif define AVERAGE
          cur_node->Average();
#endif
	}
}
//*****************************************************************************


//set image_table_
//*****************************************************************************
void Tree::set_image_table(ImageTable* image_table)
{
	image_table_=image_table;
}
//*****************************************************************************
