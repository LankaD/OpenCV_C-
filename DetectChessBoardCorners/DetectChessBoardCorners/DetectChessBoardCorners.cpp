#include <iostream>
#include "opencv2\opencv.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv[])
{
	VideoCapture vidFrames(0);
	Mat frames, drawToFrame;
	Size chessBoardSize = Size(6, 9);
	vector<Point2f> corners;
	int fps = 20;

	while (true)
	{
		if (!vidFrames.isOpened())
			break;
		vidFrames.read(frames);
		bool found = false;
		namedWindow("Chess Board Corners", WINDOW_AUTOSIZE);
		found = findChessboardCorners(frames, chessBoardSize, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
		frames.copyTo(drawToFrame);
		drawChessboardCorners(drawToFrame, chessBoardSize, corners, found);
		if (found)
		{
			imshow("Chess Board Corners", drawToFrame);
		}
		else
		{
			imshow("Chess Board Corners", frames);
		}
		char c = waitKey(1000/fps);
		if (c == 27)
			return -1;
		}
		
	return 0;
}