// detectObjectsandSave.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Functions.h"
/*#include <stdio.h>
#include <iostream>
#include <windows.h>
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\opencv.hpp"*/

using namespace std;
using namespace cv;

//main starts here
int main(int argc, char** argv)
{
	static int filenumber = 0;
	int flipImage = 0, flipFlag = 0;
	DWORD startTime = GetTickCount();
	Mat image;
	vector<Mat> images;
	CascadeClassifier markerDetector;
	vector<vector<Rect>> boundingBoxes;
	vector<Point2f> corners;
	vector<vector<Point2f>>cornerGuesses;
	bool filePathIsCorrect_images = 0, filePathIsCorrect_classifier = 0;
	//The input arguments are the absolute path of image file and the marker classifier XML file
	argc = 2;
	//Absolute path of images directory
	const string imagesFilepath = "E:\\GAC_Files\\Images_From_Test_Runs\\run_30\\Cam1";
	
	//Absolute path of classifier file
	const string classifierFilePath = "E:\\GitHub\\Masters_Work\\XML_Files\\markerDetectionAnyBackground_Mark6.xml";

	//Absolute path to save image segments
	const string saveSegmentDirectoryPath = "E:\\GAC_Files\\Markers\\ImagesFromNewData";

	fileCount(saveSegmentDirectoryPath, filenumber);
	filePathIsCorrect_images = checkPath(imagesFilepath);

	if (filePathIsCorrect_images)
	{
		loadImagesFromDirectory(imagesFilepath, images);
		markerClassifierDetection(images, classifierFilePath, boundingBoxes, markerDetector, cornerGuesses, false, true, saveSegmentDirectoryPath, filenumber, flipImage, flipFlag);
	}
	else
	{
		cout << "Something is wrong with the entered path. The image files or the marker classifier XML file is not found.\n";
	}
	destroyAllWindows();
	DWORD endTime = GetTickCount();
	cout << "\nTime elapsed :" << endTime - startTime << "ms\n";
	//system("pause");
}
//main ends here
