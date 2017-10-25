#include "stdafx.h"
#include "fileOperations.h"

using namespace std;

fileOperations::fileOperations()
{
}

fileOperations::fileOperations(const string & filepath, bool & show)
{
	this->filepath = filepath;
	this->show = show;
}

void fileOperations::readImage(const string & filepath)
{
	this->filepath = filepath;
	this->image = imread(filepath, IMREAD_GRAYSCALE);
}

void fileOperations::showImage(bool show)
{
	this->show = show;
	if (show)
	{
		namedWindow("Image", WINDOW_FREERATIO);
		imshow("Image", this->image);
		waitKey(0);
	}	
}
/*
Mat fileOperations::getImage() const
{
	return Mat();
}


void fileOperations::setImage(Mat image)
{
}*/

fileOperations::~fileOperations()
{
}
