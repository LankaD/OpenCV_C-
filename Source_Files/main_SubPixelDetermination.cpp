#include <iostream>
#include "opencv2\opencv.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	Mat image = imread("D:\\Markers_UsedForTraining\\Markers_Any_Background\\Positive\\PI_00007449.png",IMREAD_GRAYSCALE);
	vector<Point2f> corners;
	corners.push_back(Point2f(52, 51));
	// Setting the parameters to find the refined corners
	Size winSize = Size(3, 3);
	Size zeroZone = Size(-1, -1);
	TermCriteria criteria = TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 40, 0.001);
	//Calculating the refined corner locations
	cornerSubPix(image, corners, winSize, zeroZone, criteria);
	cout << corners << endl;
	system("pause");
}