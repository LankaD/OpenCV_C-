/*This is the cpp file that reads an image in its default format and returns the same*/

#include <iostream>
#include "opencv2\opencv.hpp"
#include "read_Image.h"

using namespace std;
using namespace cv;


read_Image::read_Image()
{
}

read_Image::read_Image(const Mat& image)
{
	setImage(image);
}


Mat read_Image::getImage() {
	return image_In;
}

void read_Image::setImage(const Mat& image) {
	this->image_In = image;
}

void read_Image::readImage(const string& filePath)
{
	setImage(imread(filePath, IMREAD_UNCHANGED));
	//If the image is not available or the file location is not entered correctly then the program tells the user that something is wrong and then exits
	if (image_In.empty())
	{
		cout << "Image not available. Verify the file location and try again." << endl;
		system("pause");
		exit(0);
	}
}

void read_Image::displayImage(const Mat& imageOut)
{
	namedWindow("Output Image", WINDOW_AUTOSIZE);
	imshow("Output Image", imageOut);
}

read_Image::~read_Image() {
}

