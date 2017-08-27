//This program reads frames from webcam on laptop and displays to the user
#include <iostream>
#include <vector>
#include "opencv2\opencv.hpp"
#include <time.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	VideoCapture captureObj(0);
	vector< cv::Mat > capturedFrames;
	int  numOfFrames = 0;
	cout << "Enter the number of frames:";
	cin >> numOfFrames;
	time_t starttime,endtime;
	double fps = 0.0;
	string filename = "1.PNG";
	Mat frames;
		
	time(&starttime);
	//Verifying if the default camera is opened and the number of frames to be acquired is initialized
	if (captureObj.isOpened() && numOfFrames!=0)
	{
		cout << "Camera is open and capturing from camera\n";
		//Iterating a loop numOfFrames times to capture frames from camera
		for (int i=0;i<numOfFrames;i++)
		{
			captureObj.read(frames);
			//frames.copyTo(capturedFrames[i]);
			capturedFrames.push_back(frames.clone());
			namedWindow("Capture Inside Loop", WINDOW_AUTOSIZE);
			imshow("Capture Inside Loop", capturedFrames[i]);
			//Pressing ESC key would terminate the acquisition
			if (waitKey(30) == 27)
				break;
		}
		//destroyAllWindows();
	}
	else if (captureObj.isOpened())
			{
				cout << "The number of frames is set to '0' and hence an infinte loop is turned on\n";
				//Iterating an infinite loop to capture frames from camera and the loop exits by pushing the ESC key
				for (;;)
				{
					captureObj.read(frames);
					namedWindow("Capture Inside Loop", WINDOW_AUTOSIZE);
					imshow("Capture Inside Loop", frames);
					capturedFrames.push_back(frames.clone());
					//Pressing ESC key would terminate the acquisition
					if (waitKey(30) == 27)
						break;
				}
			}	
		 else
			{
				cout << "No input device recognized\n";
				system("pause");
				return -1;
			}
	time(&endtime);
	cout << "Time elapsed : " << difftime(endtime, starttime) << " seconds" << endl;
	captureObj.release();
	fps = numOfFrames / difftime(endtime, starttime);
	if (capturedFrames.size()>0)
	{
		namedWindow("Post Capturing", WINDOW_AUTOSIZE);
		//imwrite(filename, capturedFrames[0]);
		for (int i = 0;i < capturedFrames.size();i++)
		{
			namedWindow("Post Capturing", WINDOW_AUTOSIZE);
			imshow("Post Capturing", capturedFrames[i]);
			waitKey(30);
		}
	}
	waitKey(0);
	destroyAllWindows();
	if (numOfFrames)
	{
		cout << "fps : " << fps << endl;
	}
	system("pause");
	return 0;
}