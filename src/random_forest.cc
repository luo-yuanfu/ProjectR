#include "random_forest.h"


//to do: delete memory if have dynamic allocated
//*****************************************************************************
RandomForest::~RandomForest()
{
	if(image_table_!=NULL)
	{
		delete image_table_;
	}
}
//*****************************************************************************


//build random forest, i.e., training
//*****************************************************************************
void RandomForest::BuildForest(string path)
{
	PreprocessData(path);
	
	while(trees_.size()<=MAX_TREE_NUM)
	{
		Tree * tree=new Tree();
		tree->set_image_table(image_table_);
		
		tree->InitRoot(SelectInput());
		tree->BuildTree(1, tree->root_);
		
		trees_.push_back(tree);
	}
}
//*****************************************************************************

//load images
//*****************************************************************************
void RandomForest::PreprocessData(string path)
{
	image_table_=new ImageTable();
	image_table_->LoadImages(path);//load images to image table
}
//*****************************************************************************


//to do: select and return input(ie. pixels) for a tree
//*****************************************************************************
vector<Pixel> RandomForest::SelectInput()
{
	
}
//*****************************************************************************


//*****************************************************************************
Label RandomForest::Predict(string filename)
{
	if(trees_.size()==0){
		cout<<"the regressor hasn't been trained yet"<<endl;
		cout<<"begin building random forest..."<<endl;
		BuildForest();
	}
	
	//to do: load image and predict its label using  the trees we get
}
//*****************************************************************************
