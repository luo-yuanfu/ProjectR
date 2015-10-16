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
	
	while(trees_.size()<MAX_TREE_NUM)
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
	vector<Pixel> input_px;

  // for each tree, sample from image with replacement
  const unsigned int Ntraining = image_table_->images_.size();
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
  while (tmpnode != NULL) {
            ////////////////
    if (tmptree->FeatureValue(tmppixel, tmpnode->u_, tmpnode->v_,test_image_table) <
        tmpnode->threshold_) {
        label = tmpnode->get_label();
        tmpnode = tmpnode->left_child_;              
      } 
      else {
        label = tmpnode->get_label();
        tmpnode = tmpnode->right_child_;            
      }
  }
  return label;
}
vector <Offset> RandomForest::meanshift(vector <Offset> label,vector<vector<PixelInfo> > joints_set)
{
    const unsigned int Njoints = label.size();
    const unsigned int Npixels = joints_set.size();
    const unsigned int Ndim = Njoints * 3;

    // mean shift parameters
    const double bandWidth = BANDWIDTH;
    const double bandSq = bandWidth * bandWidth;
    const double stopThresh = 1e-3 * bandWidth;
    // concat all joint information to Ndim
    vector<vector<int> > dataPts(Npixels, vector<int>(Ndim));
    for (unsigned int i = 0; i < Npixels; ++i) {
      for (unsigned int j = 0; j < Njoints; ++j) {
        dataPts[i][j * 3] = joints_set.at(i).at(j).x;
        dataPts[i][j * 3 + 1] = joints_set.at(i).at(j).y;
        dataPts[i][j * 3 + 2] = joints_set.at(i).at(j).depth;
      }
    }
    vector<Offset> L(Njoints);
    unsigned int numInitPts = Npixels;
    vector<int> initPtInds;
    for (unsigned int i = 0; i < Npixels; ++i) {
      initPtInds.push_back(i);
    }
    vector<int> beenVisitedFlag(Npixels);
    int numClust = 0;
    vector<vector<double> > clustCent;
    vector<vector<int> > clusterVotes;

    vector<double> myMean(Ndim);
    vector<double> myOldMean(Ndim);
    while (numInitPts > 0) {
      int tempInd = rand() % numInitPts;
      int stInd = initPtInds[tempInd];
      for (unsigned int i = 0; i < Ndim; ++i) {
        myMean[i] = (double)dataPts[stInd][i];
      }
      vector<int> myMembers;
      vector<int> thisClusterVotes(Npixels);
      while (1) {
        vector<double> sqDistToAll(Npixels);
        for (unsigned int i = 0; i < Npixels; ++i) {
          for (unsigned int j = 0; j < Ndim; ++i) {
            sqDistToAll[i] += (double)(dataPts[i][j] - myMean[j]) *
                              (dataPts[i][j] - myMean[j]);
          }
        }
        vector<int> inInds;
        for (unsigned int i = 0; i < Npixels; ++i) {
          if (sqDistToAll[i] < bandSq) {
            inInds.push_back(i);
          }
        }
        for (unsigned int i = 0; i < inInds.size(); ++i) {
          thisClusterVotes[i] = thisClusterVotes[i] + 1;
        }
        myOldMean = myMean;
        for (unsigned int j = 0; j < Ndim; ++j) {
          for (unsigned int i = 0; i < inInds.size(); ++i) {
            myMean[j] += dataPts[inInds[i]][j];
          }
          myMean[j] /= inInds.size();
        }
        for (unsigned int i = 0; i < inInds.size(); ++i) {
          myMembers.push_back(inInds[i]);
        }
        for (unsigned int i = 0; i < myMembers.size(); ++i) {
          beenVisitedFlag[myMembers[i]] = 1;
        }
        // if mean does not move much, stop this cluster
        double dist = 0.0;
        for (unsigned int i = 0; i < Ndim; ++i) {
          dist += (myMean[i] - myOldMean[i]) * (myMean[i] - myOldMean[i]);
        }
        dist = sqrt(dist);
        if (dist < stopThresh) {
          // check for merge posibilities
          int mergeWith = 0;
          for (unsigned int c = 0; c < numClust; ++c) {
            double distToOther = 0.0;
            for (unsigned int i = 0; i < Ndim; ++i) {
              distToOther +=
                  (myMean[i] - clustCent[c][i]) * (myMean[i] - clustCent[c][i]);
            }
            if (distToOther < bandWidth / 2) {
              mergeWith = c;
              break;
            }
          }

          if (mergeWith > 0) {
            for (unsigned int i = 0; i < Ndim; ++i) {
              clustCent[mergeWith][i] =
                  0.5 * myMean[i] + clustCent[mergeWith][i];
              for (unsigned int i = 0; i < Npixels; ++i) {
                clusterVotes[mergeWith][i] =
                    clusterVotes[mergeWith][i] + thisClusterVotes[i];
              }
            }
          } else {
            numClust += 1;
            clustCent.push_back(myMean);
            clusterVotes.push_back(thisClusterVotes);
          }

          break;
        }
      }  // while(1)

      initPtInds.clear();
      for (unsigned int i = 0; i < Npixels; ++i) {
        if (beenVisitedFlag[i] == 0) {
          initPtInds.push_back(i);
        }
      }
      numInitPts = initPtInds.size();
    }  // while(numInitPts > 0)
    // determine data cluster membership
    vector<int> data2cluster(Npixels);
    for (unsigned int i = 0; i < Npixels; ++i) {
      unsigned int maxVote = 0;
      for (unsigned int c = 0; c < numClust; ++c) {
        if (clusterVotes[c][i] > maxVote) {
          data2cluster[i] = c;
        }
      }
    }

    // determine which cluster has most votes (K=1), to set as node label
    vector<int> clusterSize(numClust);
    for (unsigned int c = 0; c < numClust; ++c) {
      for (unsigned int i = 0; i < Npixels; ++i) {
        if (data2cluster[i] == c) {
          clusterSize[c] = clusterSize[c] + 1;
        }
      }
    }

    int maxSize = 0;
    int biggestClust = 0;
    for (unsigned int c = 0; c < numClust; ++c) {
      if (clusterSize[c] > maxSize) {
        biggestClust = c;
      }
    }

    for (unsigned int j = 0; j < Njoints; ++j) {
      L[j].x = clustCent[biggestClust][j * 3];
      L[j].y = clustCent[biggestClust][j * 3 + 1];
      L[j].depth = clustCent[biggestClust][j * 3 + 2];
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
  for (int i = 0; test_image_table->images_.size(); i++) {
    tmpentry = test_image_table->get_image(i);
    // iter the forest;
    for (int j = 0; i < trees_.size(); i++) {
      tmptree = trees_.at(j);
      // split the pixels to different leaf
      for (int x = tmpentry->bounding_box.x;
           x < tmpentry->bounding_box.width + tmpentry->bounding_box.x; x++) {
        for (int y = tmpentry->bounding_box.y;
             y < tmpentry->bounding_box.height + tmpentry->bounding_box.y;
             y++) {
          tmpnode = tmptree->root_;
          Pixel tmppixel(i, {x,y,tmpentry->image_depth[y][x]});
          splitpixel(tmptree,tmpnode,tmppixel,test_image_table);
          ///////////////meanshift first then average//////////////////////////
          joints_set.push_back(label);
        }
      }
      vector <Offset> L=meanshift(label,joints_set);
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