#include <iostream>
#include "opencv2\opencv.hpp"

using namespace std;
using namespace cv;

int thresh = 200;

int main(int argc, char** argv)
{
	Mat image = imread("E:\\GAC_Files\\Markers\\SVMTraining\\blackMarker\\MarkerWhiteBG_PI_00000001.PNG", IMREAD_GRAYSCALE);
	Mat dst, dst_norm, dst_norm_scaled;
	dst = Mat::zeros(image.size(), CV_32FC1);

	/// Detector parameters
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;

	/// Detecting corners
	cornerHarris(image, dst, blockSize, apertureSize, k, BORDER_DEFAULT);

	/// Normalizing
	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(dst_norm, dst_norm_scaled);

	/// Drawing a circle around corners
	for (int j = 0; j < dst_norm.rows; j++)
	{
		for (int i = 0; i < dst_norm.cols; i++)
		{
			if ((int)dst_norm.at<float>(j, i) > thresh)
			{
				circle(image, Point(i, j), 5, Scalar(0), 2, 8, 0);
			}
		}
	}
	/// Showing the result
	namedWindow("image", WINDOW_AUTOSIZE);
	imshow("image", image);
	waitKey(0);
	destroyAllWindows();
	system("pause");
}