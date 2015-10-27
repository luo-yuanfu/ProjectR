#include "tree.h"
#include <stdlib.h>
//*****************************************************************************
Tree::Tree() {
  root_ = NULL;
  image_table_ = NULL;
}
//*****************************************************************************

// to do: delete all the nodes in the tree
//*****************************************************************************
Tree::~Tree() { FreeNode(root_); }
//*****************************************************************************
void Tree::FreeNode(Node* cur_node) {
  if (cur_node->left_child_ != NULL) {  // cur_node is not leaf node
    FreeNode(cur_node->left_child_);
    FreeNode(cur_node->right_child_);
    delete cur_node;
  } else {
    // cur_node is leaf node, to delete node
    delete cur_node;
  }
}
//*****************************************************************************

// to do: calculate and return feature value for pixel using u and v
// need to use image_table_ to get the pixels
//*****************************************************************************
double Tree::FeatureValue(Pixel piexl, std::pair<int, int> u,
                          std::pair<int, int> v) {
  int x = piexl.info_.x;
  int y = piexl.info_.y;
  int depth = piexl.info_.depth;
  int imgIdx = piexl.image_index_;
  int ux = x + u.first;
  int uy = y + u.second;
  int vx = x + v.first;
  int vy = y + v.second;

  //*** To edit based on ImageType defined in ImageTable Class
  // ImageType I = image_table_->images_[imgIdx]->image;
  // const unsigned int I_height = I.size();
  // const unsigned int I_width = I[0].size();
  int** I = image_table_->images_[imgIdx]->image_depth;
  const unsigned int I_height = image_table_->images_[imgIdx]->height;
  const unsigned int I_width = image_table_->images_[imgIdx]->width;
  int depth_u, depth_v;
  if (ux < 0 || uy < 0 || ux > I_width - 1 ||
      uy > I_height - 1) {  // pixel u out of image boundary
    depth_u = 255;          // treat as background (white)
  } else {
    depth_u = I[uy][ux];
  }
  if (vx < 0 || vy < 0 || vx > I_width - 1 ||
      vy > I_height - 1) {  // pixel v out of image boundary
    depth_v = 255;          // treat as background (white)
  } else {
    depth_v = I[vy][vx];
  }
  return ((double)(depth_u - depth_v));
}
//*****************************************************************************

// to do: calculate and return feature value for pixel using u and v
// need to use image_table_ to get the pixels
//*****************************************************************************
double Tree::FeatureValue(Pixel piexl, std::pair<int, int> u,
                          std::pair<int, int> v, ImageTable* test_image_table) {
  int x = piexl.info_.x;
  int y = piexl.info_.y;
  int depth = piexl.info_.depth;
  int imgIdx = piexl.image_index_;
  int ux = x + u.first;
  int uy = y + u.second;
  int vx = x + v.first;
  int vy = y + v.second;

  //*** To edit based on ImageType defined in ImageTable Class
  // ImageType I = image_table_->images_[imgIdx]->image;
  // const unsigned int I_height = I.size();
  // const unsigned int I_width = I[0].size();
  int** I = test_image_table->images_[imgIdx]->image_depth;
  const unsigned int I_height = test_image_table->images_[imgIdx]->height;
  const unsigned int I_width = test_image_table->images_[imgIdx]->width;
  int depth_u, depth_v;
  if (ux < 0 || uy < 0 || ux > I_width - 1 ||
      uy > I_height - 1) {  // pixel u out of image boundary
    depth_u = 255;          // treat as background (white)
  } else {
    depth_u = I[uy][ux];
  }
  if (vx < 0 || vy < 0 || vx > I_width - 1 ||
      vy > I_height - 1) {  // pixel v out of image boundary
    depth_v = 255;          // treat as background (white)
  } else {
    depth_v = I[vy][vx];
  }
  return ((double)(depth_u - depth_v));
}
//*****************************************************************************

// init root node
//*****************************************************************************
void Tree::InitRoot(vector<Pixel> pixels) {
  root_ = new Node();
  root_->pixels_ = pixels;
}
//*****************************************************************************

// build the tree, and calculate the labels for the leaf nodes
//*****************************************************************************
void Tree::ConstructTree() {
  BuildTree(1, root_);
  CalLeafLabel(root_);
}
//*****************************************************************************

//*****************************************************************************

// build decision tree
//*****************************************************************************
void Tree::BuildTree(int depth, Node* cur_node) {
  if (depth > MAX_TREE_DEPTH) return;

  // if there are no enough pixels in the node, we can stop going further
  if (cur_node->pixels_.size() < 0.005 * root_->pixels_.size()) return;

  double max_info_gain = 0;  // the max information gained
  std::pair<int, int> best_u, best_v;
  double best_threshold = 0;

  // find the best u v and threshold to split the pixels. currently select the
  // best from SAMPLE_NUM randomly sampled <u,v,threshold>
  for (int iter = 0; iter < SAMPLE_UV_NUM; iter++) {
    std::pair<int, int> u, v;
    double threshold;
    vector<Pixel> left_pixel;
    vector<Pixel> right_pixel;

    RandomSample(u, v);

    // determine range of features, then decide threshold values
    double max_feat = FeatureValue(cur_node->pixels_.at(0), u, v);
    double min_feat = FeatureValue(cur_node->pixels_.at(0), u, v);
    for (int i = 1; i < cur_node->pixels_.size(); i++) {
      double curr_feat = FeatureValue(cur_node->pixels_.at(i), u, v);
      if (curr_feat < min_feat) min_feat = curr_feat;
      if (curr_feat > max_feat) max_feat = curr_feat;
    }
    int sample_threshold_num = SAMPLE_THRESHOLD_NUM;
    // ensure SAMPLE_THRESHOLD_NUM < (max_feat - min_feat)
    if (SAMPLE_THRESHOLD_NUM > (max_feat - min_feat + 1)) {
      std::cerr
          << __FILE__ << __LINE__
          << "SAMPLE_THRESHOLD_NUM is more than range of features. Aborting. "
          << std::endl;
      sample_threshold_num = int(max_feat - min_feat + 1);
      //	throw;
    }
    int feat_range = max_feat - min_feat + 1;
    std::vector<int> candidate_threshold(feat_range);
    for (int t = 0; t < feat_range; ++t) {
      candidate_threshold[t] = min_feat + t;
    }
    std::random_shuffle(candidate_threshold.begin(), candidate_threshold.end());
    int sample = 0;
    for (int iter2 = 0; iter2 < sample_threshold_num; iter2++) {

      int success = 0;
      double iter_threshold;

      do {
        if (sample > feat_range - 1) {
          std::cerr << __FILE__ << __LINE__
                    << " threshold samples exceed feature range. Abort."
                    << std::endl;
          throw;
        }

        iter_threshold = candidate_threshold[sample];

        for (int i = 0; i < cur_node->pixels_.size(); i++) {
          if (FeatureValue(cur_node->pixels_.at(i), u, v) < iter_threshold)
            left_pixel.push_back(cur_node->pixels_.at(i));
          else
            right_pixel.push_back(cur_node->pixels_.at(i));
        }

        // check both left and right nodes have more than 1 pixel
        if (left_pixel.size() > 1 && right_pixel.size() > 1) {
          threshold=iter_threshold;
          success = 1;
          sample++;
        } else {
          sample++;
        }

      } while (success == 0);

     // double info_gain = InformationGain(cur_node, left_pixel, right_pixel);
      double info_gain=NegativeSSE(cur_node, left_pixel, right_pixel);
      if (info_gain > max_info_gain) {
        max_info_gain = info_gain;
        best_u = u;
        best_v = v;
        best_threshold = threshold;
      }

      left_pixel.clear();
      right_pixel.clear();
    }
  }

  Node* left_child = new Node();
  Node* right_child = new Node();
  for (int i = 0; i < cur_node->pixels_.size(); i++) {
    if (FeatureValue(cur_node->pixels_.at(i), best_u, best_v) < best_threshold)
      left_child->pixels_.push_back(cur_node->pixels_.at(i));
    else
      right_child->pixels_.push_back(cur_node->pixels_.at(i));
  }

  // link children and parent
  left_child->set_parent(cur_node);
  right_child->set_parent(cur_node);
  cur_node->set_left_child(left_child);
  cur_node->set_right_child(right_child);

  // store u,v,and threshold for current node
  cur_node->set_uv(best_u, best_v);
  cur_node->set_threshold(best_threshold);

  // build subtree for left and right children
  BuildTree(depth + 1, cur_node->left_child_);
  BuildTree(depth + 1, cur_node->right_child_);
}
//*****************************************************************************

// calculate the information gain
//*****************************************************************************
double Tree::InformationGain(Node* cur_node, vector<Pixel> left_pixel,
                             vector<Pixel> right_pixel) {
  if (left_pixel.size() == 0 || right_pixel.size() == 0) {
    std::cerr << __FILE__ << __LINE__
              << "either left or right split has no pixel. Aborting. "
              << std::endl;
    throw;
  }
  return cur_node->Entropy() -
         (left_pixel.size() / double(cur_node->pixels_.size()) *
              cur_node->Entropy(left_pixel) +
          right_pixel.size() / double(cur_node->pixels_.size()) *
              cur_node->Entropy(right_pixel));
}
//*****************************************************************************

// calculate the -ve of sum of squared error
//*****************************************************************************
double Tree::NegativeSSE(Node* cur_node, vector<Pixel> left_pixel,
                         vector<Pixel> right_pixel) {
  if (left_pixel.size() == 0 || right_pixel.size() == 0) {
    std::cerr << __FILE__ << __LINE__
              << "either left or right split has no pixel. Aborting. "
              << std::endl;
    throw;
  }
  return -(left_pixel.size() / double(cur_node->pixels_.size()) *
               cur_node->SSE(left_pixel) +
           right_pixel.size() / double(cur_node->pixels_.size()) *
               cur_node->SSE(right_pixel));
}
//*****************************************************************************

// to do: randomly sample a legal pair <u,v>
//*****************************************************************************
void Tree::RandomSample(std::pair<int, int>& u, std::pair<int, int>& v) {
  const int max_uv = UV_RANGE;  // +- 10 pixels boundary to search for uv
  u.first = rand() % (2 * max_uv + 1) - max_uv;
  u.second = rand() % (2 * max_uv + 1) - max_uv;
  v.first = rand() % (2 * max_uv + 1) - max_uv;
  v.second = rand() % (2 * max_uv + 1) - max_uv;
}
//*****************************************************************************

// to do: calculate the labels for the leaf node
// hint: can use the same way I build the tree to find the leaf nodes and
// calculate
// their labels, and will need to redefine the parameters for this function.
// call Node::MeanShift or Node::Average function here
//*****************************************************************************
void Tree::CalLeafLabel(Node* cur_node) {
  // assume when building tree, when splitting, always create left and right
  // child
  if (cur_node->left_child_ != NULL) {  // cur_node is not leaf node
    CalLeafLabel(cur_node->left_child_);
    CalLeafLabel(cur_node->right_child_);
  } else {
// cur_node is a leaf node
#ifdef MEANSHIFT
    cur_node->MeanShift();
#elif defined AVERAGE
    cur_node->Average();
#endif
  }
}
//*****************************************************************************

// set image_table_
//*****************************************************************************
void Tree::set_image_table(ImageTable* image_table) {
  image_table_ = image_table;
}
//*****************************************************************************
