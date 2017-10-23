#define _USE_MATH_DEFINES
#include <complex>
#include <iostream>
#include <valarray>
#include <math.h>
#include "opencv2\opencv.hpp"
#include "Functions.h"

using namespace std;
using namespace cv;
typedef std::complex<double> FFTArray;
typedef std::valarray<FFTArray> CArray;

Scalar color = Scalar(255, 255, 0);

int main(int argc, char*argv) {
	float uc = 50, vc = 50, r = 15, R = 0;
	const int M = 32;
	Mat inImage, outImage;
	string imageFilePath;
	float u[32] = { 0 }, v[32] = { 0 };
	vector<Point2f> pointsInsideSegment, pointsOnCircle, intialCornerPoints;
	float FFTMagnitude[M];
	Scalar intensity = 0;
	Point2f corner;
	float g = cos(M_PI);
	imageFilePath = "E:\\GAC_Files\\Markers\\SVMTraining\\blackMarker\\MarkerWhiteBG_PI_00000026.PNG";
	inImage = imread(imageFilePath, IMREAD_GRAYSCALE);
	
	//Corner detection
	double qualityLevel = 0.1;
	double minDistance = 2;
	int blockSize = 3;
	bool useHarrisDetector = true;
	double k = 0.04;
	goodFeaturesToTrack(inImage, intialCornerPoints, 10 , qualityLevel, minDistance, Mat(), blockSize, useHarrisDetector, k);

	FFTArray pixelIntensities[M];
	for (size_t i = 0; i < M; i++)
	{
		u[i] = (float)floor( r*cos(2 * M_PI*(i+1) / M) + uc);
		v[i] = (float)floor( r*sin(2 * M_PI*(i+1) / M) + vc);		
		pointsOnCircle.push_back(Point2f(u[i], v[i]));
		intensity = inImage.at<uchar>(Point(u[i], v[i]));
		pixelIntensities[i] = intensity.val[0];
		//circle(inImage, corners[i], 1, color, 1, 8, 0);
	}
	
	CArray FFTValues(pixelIntensities, M);
	// Forward FFT determination
	fft(FFTValues);
	for (int i = 0; i < M; ++i)
	{
		FFTMagnitude[i] = abs(FFTValues[i]);		
	}
	R = FFTMagnitude[2] - FFTMagnitude[1];
	cout << R << endl;
	/*std::cout << fftIntensity.at<float>(1, 1);
	cv::namedWindow("Image", WINDOW_GUI_NORMAL);
	cv::imshow("Image", padded);
	cv::waitKey(0);	
	cv::destroyAllWindows();*/
	system("pause");
	return 0;
}