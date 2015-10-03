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
		tree->ConstructTree();
		
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



//to do:  predict the labels for all the images contained in test_image_table using  
//the random forest we get
//*****************************************************************************
vector <vector<PixelInfo> > RandomForest::Predict(ImageTable* test_image_table)
{
	if(trees_.size()==0){
		cout<<"the regressor hasn't been trained yet"<<endl;
		cout<<"begin building random forest..."<<endl;
		BuildForest();
	}
	
	//hint: call Node::get_label() function to get the label_ of the leaf nodes

}
//*****************************************************************************



//evaluate the performance by calculating the error. the error now is the average
//error over all images and all joints
//*****************************************************************************
double RandomForest::Evaluate(vector <vector<PixelInfo> > predict_result, ImageTable* test_image_table)
{
	double error=0;

	for(int i=0; i<predict_result.size(); i++){

		vector<PixelInfo> label_for_one_image=test_image_table->images_.at(i)->joints;
		vector<PixelInfo> prediction_of_ith_image = predict_result.at(i);
		
		for(int j=0; j<label_for_one_image.size(); j++){
			double joints_error=0;	
			joints_error+=fabs(prediction_of_ith_image.at(j).x - label_for_one_image.at(j).x);
			joints_error+=fabs(prediction_of_ith_image.at(j).y - label_for_one_image.at(j).y);
			joints_error+=fabs(prediction_of_ith_image.at(j).depth - label_for_one_image.at(j).depth);
		}
		error+=joints_error/label_for_one_image.size();//average joint error for one image
	
	}

	return error/predict_result.size();
}
//*****************************************************************************