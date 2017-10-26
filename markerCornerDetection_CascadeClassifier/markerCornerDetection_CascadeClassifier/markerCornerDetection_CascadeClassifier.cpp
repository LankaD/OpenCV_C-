// markerCornerDetection_CascadeClassifier.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include "opencv2\opencv.hpp"
#include "fileOperations.h"
#include "cvOperations.h"


using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	DWORD startTime = GetTickCount(), endTime;
	Mat image;
	const string imageFilePath = "E:\\GAC_Files\\Images_From_Test_Runs\\PW6glider\\03_MEASUREMENT_images\\cam_1\\frame_000001.bmp";
	const string classifierFilePath = "E:\\GitHub\\Masters_Work\\XML_Files\\markerDetectorAnyBackground.xml";
	fileOperations fileIOobject;
	cvOperations imageOperationsObject;
	fileIOobject.readImage(imageFilePath);
	fileIOobject.showImage(0);
	imageOperationsObject.setImage(fileIOobject.getImage());
	image = imageOperationsObject.getImage();
	imageOperationsObject.showImage(0);
	imageOperationsObject.setclassifierFilePath(classifierFilePath);
	imageOperationsObject.detectObjects();
	imageOperationsObject.showDetectedObjects(0);
	
	endTime = GetTickCount();
	destroyAllWindows();
	cout << "Time elapsed: " << endTime - startTime << "ms\n";
	system("pause");
    return 0;
}

