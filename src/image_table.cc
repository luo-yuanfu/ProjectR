#include "image_table.h"

ImageTable::ImageTable()
{

}

ImageTable::ImageTable(string path)
{
	LoadImages(string path);
}

void ImageTable::LoadImages(string path)
{
	//to do: load all the images into "images_" from the directory "path"
	//including their labels and assign image id to them
}

ImageEntry * ImageTable::get_image(int index)
{
	if(index<0 || index >= images_)
	{
		cout<<"wrong image index"<<endl;
		return NULL;
	}

	return images_.at(index);
}