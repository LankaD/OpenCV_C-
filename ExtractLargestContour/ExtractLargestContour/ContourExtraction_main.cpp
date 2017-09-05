#include <iostream>
#include "opencv2\opencv.hpp"

using namespace std;
using namespace cv;

const int minThreshold = 0, maxThreshold = 255;
Scalar color = Scalar(0, 0, 255);
int erosion_size = 1;
int dilation_size = 1;
int dilation_type1 = MORPH_RECT, dilation_type2 = MORPH_CROSS, dilation_type3 = MORPH_ELLIPSE;
Mat element = getStructuringElement(dilation_type2, Size(2 * dilation_size + 1, 2 * dilation_size + 1));

void findAllContours(Mat& image, vector<vector<Point>>& contours)
{
	std::cout << "Entered findAllContours()\n";
	Mat edgeImage;
	Canny(image, edgeImage, minThreshold, maxThreshold, 3, true);
	findContours(edgeImage, contours, CV_RETR_TREE, CV_CHAIN_APPROX_TC89_L1);
	std::cout << "Exiting findAllContours()\n";
}

void findlargestContour(Mat& image, vector<vector<Point>>& contours, vector<Point>& largestContour, size_t& sizeOfContour, size_t& maxsizeOfContour, int& index)
{
	std::cout << "Entered findlargestContour()\n";
	std::cout << "Leaving findlargestContour\n";
	findAllContours(image, contours);
	std::cout << "Returned to findlargestContour\n";
	index = 0;
	maxsizeOfContour = contours[0].size();
	for (size_t i = 1; i < contours.size(); i++)
	{
		sizeOfContour = contours[i].size();
		if ((sizeOfContour > maxsizeOfContour)) //&& (contourArea(contours[i], true) < 0))
		{
			maxsizeOfContour = sizeOfContour;
			index = i;
		}
		largestContour = contours[index];
	}
	std::cout << "Exiting findlargestContour()\n";
}

void drawAllContours(Mat& contourImage, vector<vector<Point>>& contours)
{
	for (size_t i = 0; i < contours.size(); i++)
	{
		drawContours(contourImage, contours, i, color, 1, 8);
	}

}

int main(int argc, char** argv)
{
	std::cout << "Main started\n";
	Mat image, contourImage = Mat::zeros(image.size(), CV_8UC3);
	vector<vector<Point>> contours;
	vector<Point> largestContour;
	vector<Point> smallContours;
	int indexOfLargestContour = 0;
	size_t sizeOfContour = 0, maxsizeOfContour;
	//String filenames = "D:\\Markers\\Markers_With_White_Backgrounds\\Positive\\IMG_00085930.png";
	//image = imread(filenames, IMREAD_UNCHANGED);
	String folder = "D:\\Markers\\Markers_With_White_Backgrounds\\Positive";
	vector<String> filenames;
	vector<Mat> images;

	//Reading set of images from a folder
	glob(folder, filenames, false);
	//cout << filenames[0] <<endl;
	std::cout << "Loading images from folder\n";
	for (size_t i = 30000; i < 30001/*filenames.size()*/; i++)
	{
		image = imread(filenames[i], IMREAD_UNCHANGED);
		images.push_back(image);
	}
	std::cout << "Images loaded\n";
	std::cout << "Iteration through images started\n";
	static int i = 0, bad = 0;

	for (vector<Mat>::iterator imageIterator = images.begin(); imageIterator != images.end(); imageIterator++, i++)
	{
		cout << "Contour extraction for image " << i + 1 << endl;
		cout << endl;
		image = *imageIterator;
		//Copying the original image to display contours
		image.copyTo(contourImage);
		cvtColor(image, image, CV_BGR2GRAY);
		Mat grayImage = image.clone();
		//Applying convolution filters to eliminate the loosely connected areas
		GaussianBlur(image, image, Size(1, 1), 10, 10);
		//Applying OTSU's threshold
		threshold(image, image, 50, 255, CV_THRESH_OTSU);
		//erode(image, image, element, Point(-1, -1), 1);
		//Applying the morphological dialtion to reduce the small black regions in the image
		dilate(image, image, element, Point(-1, -1), 1);

		/*namedWindow("threshold", CV_WINDOW_FREERATIO);
		imshow("threshold",image);
		waitKey(0);*/
		/*for (size_t i = 0; i < contours.size(); i++)
		{
		smallContours = contours[i];
		sizeOfContour = smallContours.size();
		if (sizeOfContour < 30)
		{
		//contours.pop_back();
		contours.erase(contours.begin() + i);
		sizeOfContour = 0;
		}
		}*/
		Mat labels, img_color, stats;
		std::cout << "Leaving main\n";
		findlargestContour(image, contours, largestContour, sizeOfContour, maxsizeOfContour, indexOfLargestContour);
		std::cout << "Returned to main\n\n";
		double area = contourArea(contours[indexOfLargestContour]);
		drawContours(contourImage, contours, indexOfLargestContour, color, 1, 8);
		namedWindow("Largest Contour", CV_WINDOW_FREERATIO);
		imshow("Largest Contour", contourImage);

		char c = waitKey(0);

		if (c == ' ')
		{
			bad++;
		}
		/*drawAllContours(contourImage, contours);
		namedWindow("Contours", WINDOW_AUTOSIZE);
		imshow("Contours", contourImage);
		waitKey(0);*/
		cv::destroyAllWindows();

	}
	/*std::cout << "Iteration through images ended\n";
	std::cout << "Main ended\n";
	cv::destroyAllWindows();*/
	cout << "Wrong detections: " << bad << endl;
	//system("pause");

}
