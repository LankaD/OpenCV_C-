#pragma once
#include "stdafx.h"
#include <iostream>
#include "fileOperations.h"
#include "opencv2\opencv.hpp"
#include "opencv2\objdetect\objdetect.hpp"

using namespace std;
using namespace cv;

class cvOperations
{
public:
	cvOperations();
	cvOperations(fileOperations & fileOperationsObject);
	~cvOperations();
	void detectObjects();
	//void findObjectCorners();
	void showImage(bool show);
	void showDetectedObjects(bool showObjects);
	Mat getImage();
	vector<Point2f> getCorners();
	vector<Rect> getBoundingBoxData();
	vector<Mat> getobjectImageSegments();
	string getclassifierFilePath();
	void setImage(Mat image);
	void setCorners(vector<Point2f> objectCorners);
	void setBoundingBoxData(vector<Rect> boudingBoxes);
	void setobjectImageSegments(vector<Mat> objectImageSegments);
	void setclassifierFilePath(const string classifierFilePath);	

private:
	fileOperations fileOperationsObject;
	Mat image;
	vector<Point2f> objectCorners;
	vector<Rect> boudingBoxes;
	vector<Mat> objectImageSegments;
	string classifierFilePath;
	bool show, showObjects;
};

