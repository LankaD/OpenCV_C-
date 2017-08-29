#include <iostream>
#include "opencv2\opencv.hpp"
#include <time.h>
#include <string.h>

using namespace std;
using namespace cv;

Size chessBoardSize = Size(6, 9);
int fps = 1;

void showChessBoardCornersFromImages(vector<Mat>& images, vector<vector<Point2f>>& chessboardcorners)
{
	vector<Point2f> corners;
	bool found = false;
	vector<Mat>::iterator iter = images.begin();
	vector<vector<Point2f>>::iterator cornerIter = chessboardcorners.begin();
	for (;iter != images.end();iter++, cornerIter++)
	{
		namedWindow("Saved Images", WINDOW_AUTOSIZE);
		drawChessboardCorners(*iter, chessBoardSize, *cornerIter, true);
		imshow("Saved Images", *iter);
		waitKey(0);
	}
}

void saveImages(vector<Mat>& images)
{
	int n1,n2;
	srand(time(NULL));
	string filename;
	for (vector<Mat>::iterator iter = images.begin(); iter!= images.end(); iter++)
	{	
		n1 = rand() % 10000 + 11;
		n2 = rand() % 10000 - 7;
		filename = "Image_" + to_string(n1) + to_string(n2) + ".png";
		imwrite(filename, *iter);
		filename.clear();
	}
	return;
}

int main(int argc, char** argv[])
{
	VideoCapture vidFrames(0);
	Mat frames, drawToFrame;
	vector<Mat>savedImagesOfDetectedCorners;
	vector<Point2f> corners;
	vector<vector<Point2f>> chessboardcorners;
	bool found = false;
	while (true)
	{
		if (!vidFrames.isOpened())
			break;
		vidFrames.read(frames);
		namedWindow("Grab from Camera", WINDOW_AUTOSIZE);
		found = findChessboardCorners(frames, chessBoardSize, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
		frames.copyTo(drawToFrame);
		drawChessboardCorners(drawToFrame, chessBoardSize, corners, found);
		if (found)
		{
			imshow("Grab from Camera", drawToFrame);
		}
		else
		{
			imshow("Grab from Camera", frames);
		}
		char c = waitKey(fps);
		switch (c)
		{
		case ' ':
			//save images to a vector
			if (found)
			{
				Mat temp;
				frames.copyTo(temp);
				savedImagesOfDetectedCorners.push_back(temp);
				chessboardcorners.push_back(corners);
			}
			break;
		case 13:
			//calibrate camera
			break;
		case 27:
			destroyAllWindows();
			break;
		}
		if (c == 27)
		{
			break;
		}
	}
	//saving all the images with detected chess board corners
	saveImages(savedImagesOfDetectedCorners);
	//Displaying images with detected chess board corners
	showChessBoardCornersFromImages(savedImagesOfDetectedCorners, chessboardcorners);
	destroyAllWindows();
	//system("pause");
	return 0;
}