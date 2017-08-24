/*This is a header file for the read_Image cpp file that reads an image in its default format and returns the same*/
#pragma once
#ifndef READ_IMAGE_H
#define READ_IMAGE_H
#include <iostream>
#include "opencv2\opencv.hpp"

using namespace std;
using namespace cv;

//Declaring the read_Image class with its data members and member functions
class read_Image
{
public:
	read_Image();
	read_Image(const Mat& image);
	~read_Image();
	Mat getImage();
	void setImage(const Mat& image);
	void readImage(const string& filePath);
	void displayImage(const Mat& imageOut);

private:
	Mat image_In;
};

#endif // !READ_IMAGE_H