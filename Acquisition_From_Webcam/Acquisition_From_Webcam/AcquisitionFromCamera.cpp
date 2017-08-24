//This program reads frames from webcam on laptop and displays to the user
#include <iostream>
#include "opencv2\opencv.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	Mat frames;
	VideoCapture captureObj(0);

	if (captureObj.isOpened())
	{
		cout << "Camera is open\n";
		for (;;)
		{
			captureObj.read(frames);
			namedWindow("Captured Frame", WINDOW_FREERATIO);
			imshow("Captured Frame", frames);
			//Pressing ESC key would terminate the acquisition
			if (waitKey(30) == 27)
				break;
		}
	}
	else
	{
		cout << "No input device recognized.\n";
		return -1;
	}
	//system("pause");
	captureObj.release();
	return 0;
}