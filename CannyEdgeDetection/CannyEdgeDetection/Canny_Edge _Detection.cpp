/*This program implements the Canny Edge Detector from the OpenCV library. The input can be given as an argument or as a filepath in
the run-time*/

#include <iostream>
#include <string>
#include "opencv2\opencv.hpp"
#include "read_Image.h"

using namespace std;
using namespace cv;

read_Image imageObject;
Mat grayImage, edgeImage;

int main(int argc, char** argv) {
	string filePath;
	if (argc > 1)
	{
		filePath = argv[1];
	}
	else
	{
		cout << "Enter the absolute file path of the image" << endl;
		cin >> filePath;
	}

	imageObject.readImage(filePath);
	/*Here we are displaying the read and processed image in two different windows. However they both can be displayed in the same
	window using the displayImage() function of the same read_Image class*/
	namedWindow("Image", WINDOW_AUTOSIZE);
	imshow("Image", imageObject.getImage());
	cvtColor(imageObject.getImage(), grayImage, COLOR_BGR2GRAY);
	Canny(grayImage, edgeImage, 100, 200, 5, true);
	imageObject.displayImage(edgeImage);
	waitKey(0);
	destroyAllWindows();
	return 0;
	edgeImage.at<uchar>(4, 5);
}

