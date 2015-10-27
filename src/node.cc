#include "node.h"
#include "math.h"
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>

//*****************************************************************************
Node::Node()
{
	parent_=NULL;
	left_child_=NULL;
	right_child_=NULL;
}
//*****************************************************************************


//*****************************************************************************
Node::~Node()
{
}
//*****************************************************************************

//calculate and return the entropy of the node itself
//*****************************************************************************
double Node::Entropy()
{
	return Entropy(pixels_);
}
//*****************************************************************************
double Node::Cal_DetCov(const vector<vector<int> > M) {
	const int q = M[0].size();     // q  = Njoints * 3
	const int Npixels = M.size();

        // convert 2D vector to cv::Mat
        cv::Mat matM(M.size(), M.at(0).size(), CV_64FC1);
        for(int i=0; i<matM.rows; ++i) {
          for(int j=0; j<matM.cols; ++j) {
            matM.at<double>(i, j) = M.at(i).at(j);
          }
        }
        
	// covariance matrix
        cv::Mat_<double> samples  = matM;
        cv::Mat covar, mean;
        cv::calcCovarMatrix(samples, covar, mean, CV_COVAR_NORMAL|CV_COVAR_ROWS);
        covar = covar/(Npixels-1);

        double det = cv::determinant(covar);
	return det;
}
//*****************************************************************************


//to do: calculate and return the entropy for pixels. say if we want to calculate the entropy for parent, then we feed all the pixels to it
//if we want to calculate the entropy for left child, we feed the pixels of the left child to it
//*****************************************************************************
double Node::Entropy(const vector <Pixel> pixels)
{
  	const unsigned int Njoints = pixels[0].label_.size();
	const unsigned int Npixels = pixels.size();
	const unsigned int q = Njoints * 3;
	vector<vector <int> > M(Npixels);
  	// Put all joint offsets from all pixels into matrix M, where each row is one pixel
	// no. of col = q = Njoints * 3
	for(unsigned int i=0;i<Npixels;++i) {
	  M[i].resize(q);
	  for(unsigned int j=0;j<Njoints;++j) {
            M[i][j*3] = pixels[i].label_[j].x;
            M[i][j*3+1] = pixels[i].label_[j].y;
            M[i][j*3+2] = pixels[i].label_[j].depth;
	  }
	}

	// calculate determinant of covariance
        double detcovar = Cal_DetCov(M);
	const double pi = 3.1415926535897;
	double c = q/ 2 *(1 + log(2*pi));
        return 0.5 * log(detcovar) + c; 	
}
//*****************************************************************************
double Node::SSE(const vector <Pixel> pixels)
{
        const unsigned int Njoints = pixels[0].label_.size();
        const unsigned int Npixels = pixels.size();
        const unsigned int q = Njoints * 3;

        double sse = 0.0;
        vector<vector<double> > mean_joint(Njoints, vector<double>(3));
        // get mean label for each joint
        for(int j=0;j<Njoints;++j) {
          for(int i=0;i<Npixels;++i) {
            mean_joint[j][0] += pixels[i].label_[j].x;
            mean_joint[j][1] += pixels[i].label_[j].y;
            mean_joint[j][2] += pixels[i].label_[j].depth;
          }
        }
        for(int j=0;j<Njoints;++j) {
            for(int d=0;d<3;++d) {
              mean_joint[j][d] /= (double)Npixels;
            }
        }

        for(int j=0;j<Njoints;++j) {
          for(int i=0;i<Npixels;++i) {
            sse += (mean_joint[j][0] - pixels[i].label_[j].x) * (mean_joint[j][0] -
                pixels[i].label_[j].x);
            sse += (mean_joint[j][1] - pixels[i].label_[j].y) * (mean_joint[j][1] -
                pixels[i].label_[j].y);
            sse += (mean_joint[j][2] - pixels[i].label_[j].depth) * (mean_joint[j][2] -
                pixels[i].label_[j].depth);
          }
        }
        return sse;
}
//*****************************************************************************
//to do: given the pixels in the node, calculate the label of it using meanshift method
//the function Tree::CalLeafLabel will call this function to calculate the labels
//for all the leaf nodes
//*****************************************************************************
void Node::MeanShift()
{
   	// does mean shift for all joint locations simultaneously (concat joint labels to Njointx3
	// dimensions)
	const unsigned int Njoints = pixels_[0].label_.size();
        const unsigned int Npixels = pixels_.size();
        const unsigned int Ndim = Njoints * 3;

	// mean shift parameters
	const double bandWidth = BANDWIDTH;
	const double bandSq = bandWidth * bandWidth;
	const double stopThresh = 1e-3*bandWidth;
	// concat all joint information to Ndim
	vector<vector<int> > dataPts(Npixels, vector<int>(Ndim));
        for(unsigned int i=0;i<Npixels;++i) {
	  for(unsigned int j=0;j<Njoints;++j) {
             dataPts[i][j*3]   = pixels_[i].label_[j].x;
             dataPts[i][j*3+1] = pixels_[i].label_[j].y;
             dataPts[i][j*3+2] = pixels_[i].label_[j].depth;
          }
        }

	vector<Offset> L(Njoints);
        unsigned int numInitPts = Npixels;
	vector<int> initPtInds;
	for(unsigned int i=0;i<Npixels;++i) {
	  initPtInds.push_back(i);
	}
	vector<int> beenVisitedFlag(Npixels);
	int numClust = 0;
	vector<vector<double> > clustCent;
        vector<vector<int> > clusterVotes;

	vector<double> myMean(Ndim);
	vector<double> myOldMean(Ndim);
	while (numInitPts > 0) {
          int tempInd = rand()%numInitPts;
	  int stInd = initPtInds[tempInd];
	  for(unsigned int i=0;i<Ndim;++i) {
	    myMean[i] = (double)dataPts[stInd][i];
	  }
	  vector<int> myMembers;
	  vector<int> thisClusterVotes(Npixels);  
	  while (1) {
            vector<double> sqDistToAll(Npixels);
	    for(unsigned int i=0;i<Npixels;++i) {
	      for(unsigned int j=0;j<Ndim;++j) {
		sqDistToAll[i] += (double)(dataPts[i][j] - myMean[j])*(dataPts[i][j] - myMean[j]);
              }
	    }
	    vector<int> inInds;
	    for(unsigned int i=0;i<Npixels;++i) {
	      if(sqDistToAll[i] < bandSq) {
		inInds.push_back(i);
	      }
	    }
	    for(unsigned int i=0;i<inInds.size();++i) {
	      thisClusterVotes[inInds[i]] = thisClusterVotes[inInds[i]] + 1;
	    }
	    myOldMean = myMean;
	    std::fill(myMean.begin(),myMean.end(),0.0);
	    for(unsigned int j=0;j<Ndim;++j) {
	      for(unsigned int i=0;i<inInds.size();++i) {
		myMean[j] += dataPts[inInds[i]][j];
	      }
	      myMean[j] /= inInds.size();
	    }
	    for(unsigned int i=0;i<inInds.size();++i) {
	      myMembers.push_back(inInds[i]);
	    }
	    for(unsigned int i=0;i<myMembers.size();++i) {
	      beenVisitedFlag[myMembers[i]] = 1;
	    }
	    // if mean does not move much, stop this cluster
	    double dist = 0.0;
	    for(unsigned int i=0;i<Ndim;++i) {
	      dist += (myMean[i] - myOldMean[i]) * (myMean[i] - myOldMean[i]);
	    }
	    dist = sqrt(dist);
	    if(dist < stopThresh) {
              // check for merge posibilities
	      int mergeWith = -1;
	      for(unsigned int c=0;c<numClust;++c) {
		double distToOther = 0.0;
		for(unsigned int i=0;i<Ndim;++i) {
		  distToOther += (myMean[i] - clustCent[c][i])*(myMean[i] - clustCent[c][i]);
		}
		distToOther = sqrt(distToOther);
		if(distToOther < bandWidth/2) {
		  mergeWith = c;
		  break;
		}
	      }

	      if(mergeWith != -1) {
		for(unsigned int i=0;i<Ndim;++i) {
		  clustCent[mergeWith][i] = 0.5 * (myMean[i] + clustCent[mergeWith][i]);
		}
		for(unsigned int i=0;i<Npixels;++i) {
		  clusterVotes[mergeWith][i] = clusterVotes[mergeWith][i] + thisClusterVotes[i];
		}
	      }else {
		numClust += 1;
		clustCent.push_back(myMean);
		clusterVotes.push_back(thisClusterVotes);
	      }

	      break;
	    }
	  }  // while(1)

	  initPtInds.clear();
	  for(unsigned int i=0;i<Npixels;++i) {
	    if(beenVisitedFlag[i] == 0) {
	      initPtInds.push_back(i);
	    }
	  }
	  numInitPts = initPtInds.size();
	} // while(numInitPts > 0)
	// determine data cluster membership
	vector<int> data2cluster(Npixels);
	for(unsigned int i=0;i<Npixels;++i) {
	  unsigned int maxVote = 0;
	  for(unsigned int c=0;c<numClust;++c) {
	    if(clusterVotes[c][i] > maxVote) {
	      data2cluster[i] = c;
	    }
	  }
	}

	// determine which cluster has most votes (K=1), to set as node label
	vector<int> clusterSize(numClust);
	for(unsigned int c=0;c<numClust;++c) {
	  for(unsigned int i=0;i<Npixels;++i) {
	    if(data2cluster[i] == c) {
	      clusterSize[c] = clusterSize[c] + 1;
	    }
	  }
	}

	int maxSize = 0;
	int biggestClust = 0;
	for(unsigned int c=0;c<numClust;++c) {
	  if(clusterSize[c] > maxSize) {
	    biggestClust = c;
	  }
	}


	for(unsigned int j=0;j<Njoints;++j) {
	  L[j].x = clustCent[biggestClust][j*3];
	  L[j].y = clustCent[biggestClust][j*3+1];
	  L[j].depth = clustCent[biggestClust][j*3+2];
	}

	//call set_label() to set label_ for the leaf node after calculating the label
	set_label(L);
}
//*****************************************************************************



//to do: given the pixels in the node, calculate the label of it using averaging method
//the function Tree::CalLeafLabel will call this function to calculate the labels
//for all the leaf nodes
//*****************************************************************************
void Node::Average()
{
  // does average method for joints separately 
  const unsigned int Njoints = pixels_[0].label_.size();
  const unsigned int Npixels = pixels_.size();

  vector<Offset> L(Njoints);

  for(unsigned int j=0;j<Njoints;++j) {

    double mu_x = 0.0;
    double mu_y = 0.0;
    double mu_depth = 0.0;

    for(unsigned int i=0;i<Npixels;++i) {
      mu_x += pixels_[i].label_[j].x;
      mu_y += pixels_[i].label_[j].y;
      mu_depth += pixels_[i].label_[j].depth;
    }

    mu_x /= Npixels;
    mu_y /= Npixels;
    mu_depth /= Npixels;

    L[j].x = mu_x;
    L[j].y = mu_y;
    L[j].depth = mu_depth;
  }

  //call set_label() to set label_ for the leaf node after calculating the label
  set_label(L);
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
void Node::set_uv(std::pair<int,int> u, std::pair<int,int> v)
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

