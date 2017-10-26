#include "stdafx.h"
#include "cvOperations.h"

cvOperations::cvOperations()
{
}

cvOperations::cvOperations(fileOperations & fileOperationsObject)
{
	this->fileOperationsObject = fileOperationsObject;
	this->image = fileOperationsObject.getImage();
}

cvOperations::~cvOperations()
{
}

void cvOperations::detectObjects()
{
	CascadeClassifier objectDetector;
	if (fileOperationsObject.checkifFileExists(getclassifierFilePath()))
	{
		cout << "Classifier file read successfully\n";
		objectDetector.load(getclassifierFilePath());
		objectDetector.detectMultiScale(this->image, this->boudingBoxes, 1.1, 3, 0, Size(10, 10));
		cout << "Number of objects detected : " << this->boudingBoxes.size() << endl;
	}
	else
	{
		cout << "Something is wrong with the entered classifier file path. The file(s) is not found. Exiting...\n";
		system("pause");
		exit(0);
	}
}


void cvOperations::showImage(bool show)
{
	this->show = show;
	if (show)
	{
		namedWindow("Image", WINDOW_NORMAL);
		imshow("Image", this->image);
		waitKey(0);
	}
}

void cvOperations::showDetectedObjects(bool showObjects)
{
	this->showObjects = showObjects;
	if (showObjects)
	{
		for (vector<Rect>::iterator iter = this->boudingBoxes.begin();iter != this->boudingBoxes.end();iter++)
		{
			Rect objectBoundary = *iter;
			rectangle(this->image, objectBoundary, Scalar(0, 0, 0), 2, 8, 0);
		}
		showImage(1);
	}
}

Mat cvOperations::getImage()
{
	return image;
}

vector<Point2f> cvOperations::getCorners()
{
	return objectCorners;
}

vector<Rect> cvOperations::getBoundingBoxData()
{
	return boudingBoxes;
}

vector<Mat> cvOperations::getobjectImageSegments()
{
	return objectImageSegments;
}

string cvOperations::getclassifierFilePath()
{
	return classifierFilePath;
}

void cvOperations::setImage(Mat image)
{
	this->image = image;
}

void cvOperations::setCorners(vector<Point2f> objectCorners)
{
	this->objectCorners = objectCorners;
}

void cvOperations::setBoundingBoxData(vector<Rect> boudingBoxes)
{
	this->boudingBoxes = boudingBoxes;
}

void cvOperations::setobjectImageSegments(vector<Mat> objectImageSegments)
{
	this->objectImageSegments = objectImageSegments;
}

void cvOperations::setclassifierFilePath(const string classifierFilePath)
{
	this->classifierFilePath = classifierFilePath;
}
