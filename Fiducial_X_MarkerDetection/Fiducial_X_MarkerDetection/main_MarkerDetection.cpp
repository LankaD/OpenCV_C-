#include <stdio.h>
#include <iostream>
#include <windows.h>
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\opencv.hpp"
#include "Functions.h"

using namespace std;
using namespace cv;

inline bool fileExists(const string& name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

//main starts here
int main(int argc, char** argv)
{
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
	const string imagesFilepath = "E:\\GAC_Files\\Images_From_Test_Runs\\2017-07-26_flight_282\\JCAM34_Seq_216\\cam_1";
	//argv[3] = "E:\\GAC_Files\\Images_From_Test_Runs\\2017-07-26_flight_282\\JCAM34_Seq_216\\cam_2\\frame_000001.bmp";
	//Absolute path of classifier file
	const string classifierFilePath = "E:\\GitHub\\Masters_Work\\XML_Files\\markerDetectorAnyBackground.xml";
	filePathIsCorrect_images = checkPath(imagesFilepath);
	filePathIsCorrect_classifier = fileExists(classifierFilePath);

	if (filePathIsCorrect_images && filePathIsCorrect_classifier)
	{
		loadImagesFromDirectory(imagesFilepath, images);
		markerClassifierDetection(images, classifierFilePath, boundingBoxes, markerDetector, cornerGuesses, false);
	}
	else
	{
		cout << "Something is wrong with the entered path. The image files or the marker classifier XML file is not found.\n";
	}
	destroyAllWindows();
	DWORD endTime = GetTickCount();
	cout << endTime - startTime << "ms\n";
	system("pause");
}
//main ends here