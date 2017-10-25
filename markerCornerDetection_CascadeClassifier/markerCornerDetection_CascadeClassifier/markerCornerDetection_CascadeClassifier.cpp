// markerCornerDetection_CascadeClassifier.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include "opencv2\opencv.hpp"
#include "fileOperations.h"


using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
	DWORD startTime = GetTickCount(), endTime;
	Mat image;
	string imageFilePath = "E:\\GAC_Files\\Markers\\SVMTraining\\blackMarker\\MarkerWhiteBG_PI_00000026.PNG";
	fileOperations fh;
	fh.readImage(imageFilePath);
	fh.showImage(0);
	destroyAllWindows();
	endTime = GetTickCount();
	cout << endTime - startTime << "ms\n";
	system("pause");
    return 0;
}

