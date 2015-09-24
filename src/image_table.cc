#include "image_table.h"

//*****************************************************************************
ImageTable::ImageTable()
{

}
//*****************************************************************************


//to do: delete memory if have dynamic allocated
//*****************************************************************************
ImageTable::~ImageTable()
{

}
//*****************************************************************************



//*****************************************************************************
ImageTable::ImageTable(string path)
{
	LoadImages(path);
}
//*****************************************************************************


//load all images from path
//*****************************************************************************
void ImageTable::LoadImages(string path)
{
	//to do: load all the images into "images_" from the directory "path"
	//including their labels and assign image id to them
}
//*****************************************************************************


//get image whose index is `index
//*****************************************************************************
ImageEntry * ImageTable::get_image(int index)
{
	if(index<0 || index >= images_.size())
	{
		cout<<"wrong image index"<<endl;
		return NULL;
	}

	return images_.at(index);
}
//*****************************************************************************