#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <io.h>
#include <string>
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\opencv.hpp"

using namespace std;
using namespace cv;

inline bool fileExists(const string& name) 
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void markerClassifierDetection(const string& imagePath, const string& classifierPath, vector<Rect>& boundingBoxInfo, CascadeClassifier& markerDetector, Mat& image)
{
	markerDetector.load(classifierPath);
	image = imread(imagePath, IMREAD_GRAYSCALE);
	markerDetector.detectMultiScale(image, boundingBoxInfo, 1.1, 3, 0, Size(10, 10));
	if (boundingBoxInfo.size()>0)
	{
		for (vector<Rect>::iterator r = boundingBoxInfo.begin(); r != boundingBoxInfo.end(); ++r)
		{
			Rect objectBoundary = (*r);
			rectangle(image, objectBoundary, Scalar(0, 0, 0), 2, 8, 0);
		}
	}
}

int main(int argc, char** argv)
{
	Mat image;
	CascadeClassifier markerDetector;
	vector<Rect> boundingBoxes;
	vector<Point2f> corners;
	//The input arguments are the absolute path of image file and the marker classifier XML file
	argc = 2;
	//Absolute path of image file
	argv[1] = "E:\\GAC_Files\\Images_From_Test_Runs\\2017-07-26_flight_282\\JCAM34_Seq_216\\cam_1\\frame_000001.bmp";
	//Absolute path of classifier file
	argv[2] = "E:\\GitHub\\Masters_Work\\XML_Files\\markerDetectorAnyBackground.xml";
	if (argc>=2)
	{
		if ((fileExists(argv[1])) && (fileExists(argv[2])))
		{
			markerClassifierDetection(argv[1], argv[2], boundingBoxes, markerDetector, image);
			namedWindow("Image", WINDOW_NORMAL);
			imshow("Image", image);
			waitKey(0);
		}
		else
		{
			cout << "Invalid files at directory path. Please verify the files exist.\n";
			system("pause");
		}
		
	}
	else
	{
		cout << "Insufficient input arguments. Please enter the absolute path of image file and the marker classifier XML file.\n";
		system("pause");
	}
	destroyAllWindows();
}