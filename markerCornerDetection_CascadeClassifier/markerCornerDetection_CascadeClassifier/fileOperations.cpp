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
	if (checkifFileExists(filepath))
	{
		cout << "Image read successfully\n";
		this->image = imread(filepath, IMREAD_GRAYSCALE);
	}
	else
	{
		cout << "Something is wrong with the entered file path. The file(s) is not found. Exiting...\n";
		system("pause");
		exit(0);
	}
}

void fileOperations::showImage(bool show)
{
	this->show = show;
	if (show)
	{
		namedWindow("Image", WINDOW_AUTOSIZE);
		imshow("Image", this->image);
		waitKey(0);
	}	
}
bool fileOperations::checkifFileExists(const string & filepath)
{
	struct stat buffer;
	return (stat(filepath.c_str(), &buffer) == 0);
}
Mat fileOperations::getImage() const
{
	return image;
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
