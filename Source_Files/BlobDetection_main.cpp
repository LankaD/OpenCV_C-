#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	// Storage for blobs
	vector<KeyPoint> keypoints;
	// Read image
	Mat im = imread("D:\\Markers\\Markers_With_White_Backgrounds\\Positive\\IMG_00059092.png", IMREAD_GRAYSCALE);
	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;

	//Change colors
	params.filterByColor = false;
	params.blobColor = 0;

	// Change thresholds
	params.minThreshold = 0;
	params.maxThreshold = 255;

	// Filter by Area.
	params.filterByArea = true;
	params.minArea = 1;
	//params.maxArea = 50;
	
	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.5;

	// Filter by Convexity
	params.filterByConvexity = false;
	params.minConvexity = 0.5;

	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0;


#if CV_MAJOR_VERSION < 3   // If you are using OpenCV 2

	// Set up detector with params
	SimpleBlobDetector detector(params);

	// Detect blobs
	detector.detect(im, keypoints);
#else 

	// Set up detector with params
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

	// Detect blobs
	detector->detect(im, keypoints);
#endif 

	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
	// the size of the circle corresponds to the size of blob

	/*Mat im_with_keypoints;
	drawKeypoints(im, keypoints, im_with_keypoints, Scalar(0, 255, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	// Show blobs
	namedWindow("keypoints", WINDOW_NORMAL);
	imshow("keypoints", im_with_keypoints);
	waitKey(0);*/
	system("pause");
}
