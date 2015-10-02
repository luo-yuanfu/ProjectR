#include "pixel.h"


Pixel::Pixel()
{

}

Pixel::Pixel(int image_index, PixelInfo info, ImageTable *image_table): info_(info_), image_index_(image_index)
{
	CalLabel(image_table);
}

//delete the dynamic allocated memory if there is any
Pixel::~Pixel()
{

}

//calculate the labels for the pixels
void Pixel::CalLabel(ImageTable *image_table)
{
	ImageEntry * image_entry=image_table->get_image(image_index_);
	if(image_entry==NULL){
		return;
	}
	for(int i=0; i<image_entry->joints.size(); i++){

		Offset joint_offset;
		joint_offset.x = image_entry->joints.at(i).x - info_.x;
		joint_offset.y = image_entry->joints.at(i).y - info_.y;
		joint_offset.depth = image_entry->joints.at(i).depth - info_.depth;
		label_.push_back(joint_offset);
	}
	
}