#include "random_forest.h"
#include "singleton.h"


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
	
  int tree_num=0;
  int MAX_TREE_NUM = singleton::get().MAX_TREE_NUM();
	while(trees_.size()<MAX_TREE_NUM)
	{
		Tree * tree=new Tree();
		tree->set_image_table(image_table_);

    cout<<tree_num<<" is being constructed"<<endl;
		
		tree->InitRoot(SelectInput());

    cout<<tree_num<<"'s root node has been initialied"<<endl;

		tree->ConstructTree();

    cout<<tree_num<<" has been constructed"<<endl;	

    tree_num++;

		trees_.push_back(tree);

    
	}
}
//*****************************************************************************

//load images
//*****************************************************************************
void RandomForest::PreprocessData(string path)
{
	string train_path=path+"/depth";//"../train/pic/depth";
  string train_image_ext  = ".png";
  string train_label_path ="./train/label";
  image_table_=new ImageTable();
  image_table_->LoadImages(train_path,train_label_path,train_image_ext);//load images to image table
}
//*****************************************************************************


//to do: select and return input(ie. pixels) for a tree
//*****************************************************************************
vector<Pixel> RandomForest::SelectInput()
{
  int IMG_SUBSET_SIZE = singleton::get().IMG_SUBSET_SIZE();
  int PX_SUBSET_SIZE = singleton::get().PX_SUBSET_SIZE();
	vector<Pixel> input_px;

  // for each tree, sample from image with replacement
  const unsigned int Ntraining = image_table_->images_.size();
  if(IMG_SUBSET_SIZE > Ntraining) {
    cout<<"IMG_SUBSET_SIZE more than Ntraining"<<endl;
    throw;
  }
	// shuffle training data
	vector<int> train_img_idx(Ntraining);
	for(int i=0;i<Ntraining;i++) {
          //train_img_idx = i;
		train_img_idx[i]=i;
	}
	std::random_shuffle(train_img_idx.begin(),train_img_idx.end());
	// select first N images
	train_img_idx.resize(IMG_SUBSET_SIZE);

	// for each selected image, random choose pixels
	for(int i=0;i<IMG_SUBSET_SIZE;i++) {////////////mismatch
	  int img_index = train_img_idx[i];
	  ImageEntry * I = image_table_->get_image(img_index);
//	  BoundBox * bbox = I->bounding_box;
	  BoundBox bbox(I->bounding_box);
//	  int start_x = bbox->x;
//	  int start_y = bbox->y;
	  int start_x = bbox.x;
	  int start_y = bbox.y;
//	  int width = bbox->width;
//	  int height = bbox->height;
	  int width = bbox.width;
	  int height = bbox.height;
//	  int end_x = start_x + width - 1;
//	  int end_y = start_y + width - 1;
    if(PX_SUBSET_SIZE > width*height) {
      cout<<"PX_SUBSET_SIZE more than bounding box pixels"<<endl;
      throw;
    }

	  // put bounding box pixels into a vector, then shuffle that vector
	  vector<int> px_idx(height*width);
	  for(int k=0;k<height*width;k++) {
	    px_idx[k] = k;
	  }
	  std::random_shuffle(px_idx.begin(),px_idx.end());
	  // select some pixels within bounding box
	  for(int j=0;j<PX_SUBSET_SIZE;j++) {
        int box_idx = px_idx[j];
	    // convert to 2D coord within box
	    int box_x = box_idx % width;
	    int box_y = box_idx / width;
	    // convert box_coord to image_coord
	    int x = box_x + start_x;
	    int y = box_y + start_y;
		/*
	    PixelInfo * px_info = new PixelInfo;
	    px_info->x = x;
	    px_info->y = y;
	    px_info->depth = I->image_depth[y][x];
	    // create pixel object
	    Pixel * px = new Pixel(img_index,info,image_table_);
            input_px.push_back(px);
        */

      PixelInfo px_info;
	    px_info.x = x;
	    px_info.y = y;
	    px_info.depth = I->image_depth[y][x];
	    // create pixel object
	    Pixel px(img_index, px_info, image_table_);
        input_px.push_back(px);
	  }
	}/////added "}"

    return input_px;
}
//*****************************************************************************
vector <Offset> RandomForest::splitpixel(Tree * tmptree,Node * tmpnode,Pixel tmppixel,ImageTable* test_image_table)
{
  vector <Offset> label;
  while (tmpnode-> left_child_ != NULL) {  // while tmpnode not leaf node
            ////////////////
    if (tmptree->FeatureValue(tmppixel, tmpnode->u_, tmpnode->v_,test_image_table) <
        tmpnode->threshold_) {
        tmpnode = tmpnode->left_child_;              
      } 
      else {
        tmpnode = tmpnode->right_child_;            
      }
  }
  label = tmpnode->get_label();

  for(int i=0; i< label.size(); i++){
    label.at(i).x = label.at(i).x+tmppixel.info_.x;
    label.at(i).y = label.at(i).y+tmppixel.info_.y;
    label.at(i).depth = label.at(i).depth + tmppixel.info_.depth;
  }

  return label;
}

//*****************************************************************************
 vector <PixelInfo> RandomForest::average(vector<vector<PixelInfo> > joints_set) 
 {
 // does average method for joints separately 
  // average on joint locations
  const unsigned int Njoints = joints_set[0].size();
  const unsigned int Npixels = joints_set.size();

  vector<PixelInfo> L(Njoints);

  for(unsigned int j=0;j<Njoints;++j) {

    double mu_x = 0.0;
    double mu_y = 0.0;
    double mu_depth = 0.0;

    for(unsigned int i=0;i<Npixels;++i) {
      mu_x += joints_set[i][j].x;
      mu_y += joints_set[i][j].y;
      mu_depth += joints_set[i][j].depth;
    }

    mu_x /= Npixels;
    mu_y /= Npixels;
    mu_depth /= Npixels;

    L[j].x = mu_x;
    L[j].y = mu_y;
    L[j].depth = mu_depth;
  }
  return L;
}


//to do:  predict the labels for all the images contained in test_image_table using  
//the random forest we get
//*****************************************************************************
vector <vector<PixelInfo> > RandomForest::Predict(ImageTable* test_image_table)
{
  if (trees_.size() == 0) {
    cout << "the regressor hasn't been trained yet" << endl;
    cout << "begin building random forest..." << endl;
    BuildForest();
  }

  // hint: call Node::get_label() function to get the label_ of the leaf nodes
  ImageEntry *tmpentry;
  Tree *tmptree;
  Node *tmpnode;
  vector<vector<PixelInfo> > joints_set;
 // Pixel tmppixel;
  vector<Offset> label;
  vector<vector<PixelInfo> > images_joints;
  // for each image
  for (int i = 0; i< test_image_table->images_.size(); i++) {
    tmpentry = test_image_table->get_image(i);
    // iter the forest;
    for (int j = 0; j < trees_.size(); j++) {
      tmptree = trees_.at(j);
      // split the pixels to different leaf
      for (int x = tmpentry->bounding_box.x;
           x < tmpentry->bounding_box.width + tmpentry->bounding_box.x; x++) {
        for (int y = tmpentry->bounding_box.y;
             y < tmpentry->bounding_box.height + tmpentry->bounding_box.y;
             y++) {
          tmpnode = tmptree->root_;
          PixelInfo temp_pixel_info;//(x,y,tmpentry->image_depth[y][x]);
          temp_pixel_info.x=x;
          temp_pixel_info.y=y;
          temp_pixel_info.depth=tmpentry->image_depth[y][x];
          Pixel tmppixel(i, temp_pixel_info);
          vector <Offset> temp_label = splitpixel(tmptree,tmpnode,tmppixel,test_image_table);
          ///////////////meanshift first then average//////////////////////////
          joints_set.push_back(temp_label);
        }
      }
      
      vector <Offset> L=average(joints_set);
      if(j==0){
        images_joints.push_back(L);
      }
      else{
        for(int k=0;k<label.size();k++){
          images_joints.at(i).at(k).x=(images_joints.at(i).at(k).x*j+L.at(k).x)/(j+1);
          images_joints.at(i).at(k).y=(images_joints.at(i).at(k).y*j+L.at(k).y)/(j+1);
          images_joints.at(i).at(k).depth=(images_joints.at(i).at(k).depth*j+L.at(k).depth)/(j+1);
        }
      }

      joints_set.clear();
      
    }


  }
  return images_joints;


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
		
		double joints_error=0;
		for(int j=0; j<label_for_one_image.size(); j++){	
			joints_error+=fabs(prediction_of_ith_image.at(j).x - label_for_one_image.at(j).x);
			joints_error+=fabs(prediction_of_ith_image.at(j).y - label_for_one_image.at(j).y);
			joints_error+=fabs(prediction_of_ith_image.at(j).depth - label_for_one_image.at(j).depth);
		}
		error+=joints_error/label_for_one_image.size();//average joint error for one image
	
	}

	return error/predict_result.size();
}
//*****************************************************************************