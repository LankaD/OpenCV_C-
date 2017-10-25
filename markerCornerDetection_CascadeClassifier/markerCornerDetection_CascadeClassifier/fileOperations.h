#pragma once
#include "stdafx.h"
#include <iostream>
#include "opencv2\opencv.hpp"

using namespace std;
using namespace cv;

class fileOperations
{
public:
	//Default Constructors
	fileOperations();
	fileOperations(const string & filepath, bool & show);

	//Member Functions
	void readImage(const string & filepath);
	void showImage(bool show);
	/*//Accessors
	Mat getImage() const;
	//Mutators
	void setImage(Mat image);*/

	//Destructor
	~fileOperations();

private:
	Mat image;
	bool show;
	string filepath;
};

