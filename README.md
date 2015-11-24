================================================================================
DESCRIPTION
================================================================================
This repository contains the source code for the final project of CS5340 at NUS, in Semester 1, 2015. It implements the zebrafish pose estimation from depth images using random forest.

You can read a better formatted readme file in the following github link:
https://github.com/luo-yuanfu/ProjectR



================================================================================
REQUIREMENTS
================================================================================
Operating systems: Linux, Mac OS X

Tested Compilers: gcc/g++ 4.2.1 or above

Library requirement: OpenCV

General: GNU make is required for building



================================================================================
USAGES
================================================================================
Go to the root directory of this package, run the following to build the package:

	make

Then run the following command to train random forest and get the pose estimations given by the random forest:

	./bin/random_forest_regressor

The above command will write training error in the command line.

If you want to print the predictions for the testing images, use following command:

	./bin/random_forest_regressor -p

When you are testing many images, you may want to output those predictions into a file, e.g., result.txt, then you can run the following command:

	./bin/random_forest_regressor -p > result.txt

If you want to change the training images or testing images, you can edit train/extract_train.sh and test/extract_test.sh respectively. These two files will generate the training images and testing images in train/train_set and test/test_set for the random forest. 

You can tune the parameters for random forest in include/parameters.dat. See the following PARAMETER EXPLANATION section for the explanations of the parameters



========================================================================
PARAMETER EXPLANATION
========================================================================
```
MAX_TREE_NUM			number of the tree of the random forest
IMG_SUBSET_SIZE			number of images used for each tree
PX_SUBSET_SIZE			number of pixels sampled from one image for training
MAX_TREE_DEPTH			maximum tree depth
SAMPLE_UV_NUM 			number of uv vector pairs sampled to split the node of the tree 
SAMPLE_THRESHOLD_NUM 	number of threshold sampled to split the node of the tree
UV_RANGE 				the range of uv vector
BANDWIDTH	 			parameter for finding cluster membership for meanshift
DELTA					border allowance for bounding box
MIN_LEAF_SIZE			minimum no. Of pixels in leaf node as ratio of number of root node pixels
```



========================================================================
PACKAGE CONTENTS 
========================================================================
```
Makefile                  Makefile for compiling the source code
README.md                 Overview
include                   Header files
src                       Source files
train/pic 				  All the training images
train/train_set			  The images used to train random forest
train/extract_train.sh 	  Shell file used to extract images from train/pic to train/train_set for training
test/pic 				  All the testing images
test/test_set			  The images used for testing
test/extract_test.sh 	  Shell file used to extract images from test/pic to test/test_set for training
```


========================================================================
OTHERS
========================================================================
The current parameter setting in include/parameters.dat is used for generating the best result we got. For a quick run, you can set the parameters as following.
And you also need to edit the train/extract_train.sh and test/extract_test.sh files to extract the correct number of images. (10 training images and 10 testing images for this case)
```
MAX_TREE_NUM		2
IMG_SUBSET_SIZE		4
PX_SUBSET_SIZE		100
MAX_TREE_DEPTH		5
SAMPLE_UV_NUM 		30
SAMPLE_THRESHOLD_NUM 	30
UV_RANGE 		30
BANDWIDTH	 	0.75
DELTA			0.05
MIN_LEAF_SIZE		0.005
```