#include <stdio.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\opencv.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	DWORD startTime = GetTickCount();
	Mat image, imageSegment;
	
	static int i = 0, j = 0;
	CascadeClassifier detect_marker;
	vector<Rect>boxes;
	double a, b;
	
	argv[1] = "D:\\IPCT_Testdaten\\Airbus_A320\\2012-07-26_flight_283\\2012-07-26_14-32-23\\cam_1\\0001.tiff";
	argv[2] = "D:\\IPCT_Marker_Detection_Development\\MATLAB_Files\\XML_Files\\markerDetectorAnyBackground.xml";
	argv[3] = "D:\\IPCT_Projects_Tests\\Airbus_A320\\cps\\cam_1\\cps_img00001.dat";
	argv[4] = "D:\\temp\\markerSizes.dat";

	static int maximumMarkerSize = 88;
	static int minimumMarkerSize = 25;
	const int x = maximumMarkerSize+10;
	detect_marker.load(argv[2]);
	image = imread(argv[1], IMREAD_GRAYSCALE);
	
	ifstream infile(argv[3]);
	ofstream outfile(argv[4]);
	while (infile >> a >> b)
	{
		Rect roi((int)a - (maximumMarkerSize / 2), (int)b - (maximumMarkerSize / 2), maximumMarkerSize, maximumMarkerSize);
		imageSegment = image(roi);
		detect_marker.detectMultiScale(imageSegment, boxes, 1.1, 3, 0, Size(10, 10), Size(x, x));
		if (boxes.size()==1)
		{
 			for (vector<Rect>::iterator r = boxes.begin(); r != boxes.end(); ++r)
			{
				Rect detectedObjectBoundary = (*r);
				detectedObjectBoundary.x += (int)a - (maximumMarkerSize / 2);
				detectedObjectBoundary.y += (int)b - (maximumMarkerSize / 2);
				rectangle(image, detectedObjectBoundary, Scalar(0, 0, 0), 2, 8, 0);
				if (detectedObjectBoundary.width >= detectedObjectBoundary.height)
				{
					outfile << detectedObjectBoundary.width << endl;
				}
				else
				{
					outfile << detectedObjectBoundary.height << endl;
				}
			}
			++i;
			//Displaying the detected objects
			/*namedWindow("Displaying Detetcted Objects", CV_WINDOW_FREERATIO);
			imshow("Displaying Detetcted Objects", image);*/	
		}
		else
		{
			++j;
			outfile << (int)(maximumMarkerSize+minimumMarkerSize)/2 << endl;
		}
		
	}
	waitKey(0);
	infile.close();
	outfile.close();
	destroyAllWindows();
	DWORD endTime = GetTickCount();
	cout << endTime - startTime << "ms\n";
	cout << "Number of markers detected: "<< i << endl;
	cout << "Number of markers not detected: " << j << endl;
	system("pause");
}