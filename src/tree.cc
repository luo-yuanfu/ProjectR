#include "tree.h"

Tree::Tree()
{
	root_=NULL;
	image_table_=NULL;
}

void InitRoot(vector <Pixel> pixels)
{
	root_=new Node();
	root_->pixels_=pixels;
}

void Tree::BuildTree(int depth, Node* cur_node)
{
	if(depth>MAX_TREE_DEPTH)  return;

	// to do: find the best u v and threshold to split the pexiels. it can be like the following form:
	/*
	random_init(u,v,threshold);
	vector<Pixel> left_pixel;
	vector<Pixel> right_pixel;
	for(u,v)
		for(threshold)
			for(int i=0; i<cur_node->pixels.size(); i++){
				if(cur_node->FeatureValue(cur_node->pixels.at(i), u, v)<threshold)
					left_pixel.push_back
			}
	*/
}

void Tree::set_image_table(ImageTable* image_table);
{
	image_table_=image_table;
}