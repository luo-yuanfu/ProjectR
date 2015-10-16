#include "image_table.h"

//*****************************************************************************
ImageTable::ImageTable()
{
	delta=0.05;				//default 0.05
}

ImageTable::ImageTable(float _delta)
{
	delta=_delta;
}
//*****************************************************************************

ImageTable::ImageTable(string path)
{
	delta=0.05;
	LoadImages(path);
}

//*****************************************************************************
ImageTable::~ImageTable()
{
	for(vector<ImageEntry *>::iterator iter=this->images_.begin();iter!=this->images_.end();++iter)
	{
		for(int i = 0;i < (*iter)->height; i++)
		{
			delete[] (*iter)->image_depth[i];
		}

		delete [] ((*iter)->image_depth);

	//	(*iter)->joints.clear();
	//	vector<PixelInfo>().swap((*iter)->joints);
	}
//	this->images_.clear();
//	vector<ImageEntry *>().swap(this->images_);
}
//*****************************************************************************

BoundBox ImageTable::CalcBoundBox(Mat image, vector<PixelInfo> joints)
{
	BoundBox bbox;
	int xmax,ymax,xmin,ymin;
	xmax=ymax=0;
	xmin=ymin=0x7fffffff;
	for(vector<PixelInfo>::iterator iter=joints.begin();iter!=joints.end();++iter)
	{

		int x = iter->x;
		int y = iter->y;
		if(xmax < x)
			xmax= x;
		if(xmin > x)
			xmin = x;
		if(ymax < y)
			ymax= y;
		if(ymin > y)
			ymin = y;
	}
	int col_delta=(int)(image.cols*delta);
	int row_delta=(int)(image.rows*delta);
	xmax+=col_delta;
	ymax+=row_delta;
	xmin-=col_delta;
	ymin-=row_delta;

	bbox.x = xmin < 0 ? 0 : xmin;
	bbox.y = ymin < 0 ? 0 : ymin;
	xmax = xmax > image.cols ? image.cols : xmax;
	ymax = ymax > image.rows ? image.rows : ymax;

	bbox.width = xmax - bbox.x;
	bbox.height = ymax - bbox.y;
	//cout << xmax << '\t' << ymax << '\t' << xmin << '\t' << ymin << endl;   //for debug
	return bbox;
}

BoundBox ImageTable::CalcBoundBox(Mat image)
{
	BoundBox bbox;
	Mat binary=image.clone();
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//cvtColor(binary, binary, CV_BGR2GRAY );
	threshold(binary, binary, 200, 255, THRESH_BINARY);

	//namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	//imshow( "Contours", binary );
	//waitKey();

	findContours(binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	Mat drawing = Mat::zeros(binary.size(), CV_8UC3 );
	int largei=0;
	int size=0;
    //RNG rng(12345);
	for( int i = 0; i < contours.size(); i++ )
	{
		approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
	    boundRect[i] = boundingRect( Mat(contours_poly[i]));
	    //Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
	   // drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
	    if( i!=0  && (size < boundRect[i].area()))
	    {
	    	size=boundRect[i].area();
	    	largei=i;
	    }
	}

	//Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
	//rectangle( drawing, boundRect[largei].tl(), boundRect[largei].br(), color, 2, 8, 0 );

// for debug
	//namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	//imshow( "Contours", drawing );
	//waitKey();

	int col_delta=(int)(image.cols*delta);
	int row_delta=(int)(image.rows*delta);

	int xmax = boundRect[largei].br().x + col_delta;
	int ymax = boundRect[largei].br().y + row_delta;
	int xmin = boundRect[largei].tl().x - col_delta;
	int ymin = boundRect[largei].tl().y - row_delta;

	bbox.x = xmin < 0 ? 0 : xmin;
	bbox.y = ymin < 0 ? 0 : ymin;

	xmax = xmax > image.cols ? image.cols : xmax;
	ymax = ymax > image.rows ? image.rows : ymax;

	bbox.width = xmax - bbox.x;
	bbox.height = ymax - bbox.y;

	//cout << xmax << '\t' << ymax << '\t' << xmin << '\t' << ymin << endl; // for debug

	return bbox;
}

//load all images from path
//*****************************************************************************
void ImageTable::LoadImages(string Iname)
{
	ImageEntry *newImage = new ImageEntry;
	//newImage->image=imread(Iname);
	Mat image=imread(Iname,CV_LOAD_IMAGE_GRAYSCALE);
	newImage->image_depth=new int*[image.rows];
	newImage->height = image.rows;
	newImage->width = image.cols;
	for (int r = 0; r < image.rows; ++r)
	{
		newImage->image_depth[r] = new int[image.cols];
		for (int c = 0; c < image.cols; ++c)
			newImage->image_depth[r][c] = (int)image.at<uchar>(r, c);
	}
	//BoundBox CalcBoundBox(Mat image, vector<PixelInfo> joints);
	newImage->bounding_box=CalcBoundBox(image);
	ImageTable::images_.push_back(newImage);
}
//*****************************************************************************

void ImageTable::LoadImages(string Ipath, string Lpath, string Iext)
{
	ifstream lable;
	lable.open(Lpath.c_str(),ios::in); //open and read the label file
	if(lable.fail())
	{
		cout << "cannot open the lable file" << endl;
		exit(-1);
	}
	string linestr;
	for (int i=0;getline(lable, linestr);i++)
	{
		ImageEntry *newImage = new ImageEntry;

		stringstream Iname;
		Iname << Ipath << i << Iext;
		//newImage->image=imread(Iname.str());
		Mat image = imread(Iname.str(),CV_LOAD_IMAGE_GRAYSCALE);
		newImage->image_depth = new int*[image.rows];
		newImage->height = image.rows;
		newImage->width = image.cols;
		for (int r = 0; r < image.rows; ++r)
		{
			newImage->image_depth[r] = new int[image.cols];
			for (int c = 0; c < image.cols; ++c)
				newImage->image_depth[r][c] = (int)image.at<uchar>(r, c);
		}
		istringstream iss(linestr);
		int tmp;
		while (iss >> tmp)
		{
			PixelInfo newJoint;
			newJoint.x = tmp;
			iss >> newJoint.y;
			newJoint.depth = image.at<uchar>(newJoint.y,(newJoint.x-1)*3);
			newImage->joints.push_back(newJoint);
		}
		newImage->bounding_box=CalcBoundBox(image, newImage->joints);
		ImageTable::images_.push_back(newImage);
	}
}

ImageEntry * ImageTable::get_image(int index)
{
	if(index<0 || index >= images_.size())
	{
		cout<<"wrong image index"<<endl;
		exit(-1);
	}

	return images_.at(index);
}


