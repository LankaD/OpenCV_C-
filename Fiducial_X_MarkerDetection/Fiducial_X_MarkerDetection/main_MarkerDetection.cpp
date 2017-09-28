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

void markerClassifierDetection(const string& imagePath, const string& classifierPath, vector<Rect>& boundingBoxInfo, CascadeClassifier& markerDetector, Mat& image, bool showDetectedObjects)
{
	markerDetector.load(classifierPath);
	image = imread(imagePath, IMREAD_GRAYSCALE);
	markerDetector.detectMultiScale(image, boundingBoxInfo, 1.1, 3, 0, Size(10, 10));
	if ((boundingBoxInfo.size()>0) && showDetectedObjects==true)
	{		
		for (vector<Rect>::iterator r = boundingBoxInfo.begin(); r != boundingBoxInfo.end(); ++r)
		{
			Rect objectBoundary = (*r);
			rectangle(image, objectBoundary, Scalar(0, 0, 0), 2, 8, 0);			
		}
		namedWindow("Image", WINDOW_NORMAL);
		imshow("Image", image);
		waitKey(0);
	}
	else
	{
		if ((boundingBoxInfo.size() == 0))
		{
			cout << "No objects detected\n";
		}
	}
}

void cornersDetection(const Mat& image, bool showDetectedCorners)
{
	//Parameters for Shi-Tomasi corner detector
	Mat image_Out;
	vector<Point2f> corners;
	double qualityLevel = 0.01;
	double minDistance = 5;
	int blockSize = 3, maxCorners = 10;
	bool useHarrisDetector = false;
	double k = 0.04;
	image.copyTo(image_Out);
	GaussianBlur(image, image_Out, Size(5, 5), 5, 5);
	//Apply corner detection
	goodFeaturesToTrack(image_Out, corners, maxCorners, qualityLevel, minDistance, Mat(), blockSize, useHarrisDetector, k);
	
	//Drawing detected corners
	for (int i = 0; i < corners.size(); i++)
	{
		circle(image, corners[i], 1, Scalar(0,0,255), -1, 8, 0);
	}	
}

void extractObjectROI(const Mat& image, vector<Rect>& boundingBoxInfo, bool showROI)
{
	for (vector<Rect>::iterator r = boundingBoxInfo.begin(); r != boundingBoxInfo.end(); ++r)
	{
		Rect ROI(r->x, r->y, r->width, r->height);
		Mat imageROI = image(ROI);
		cornersDetection(imageROI, true);
		namedWindow("ROI", WINDOW_KEEPRATIO);
		imshow("ROI", imageROI);
		waitKey(0);
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
			markerClassifierDetection(argv[1], argv[2], boundingBoxes, markerDetector, image, false);
			extractObjectROI(image, boundingBoxes, true);
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