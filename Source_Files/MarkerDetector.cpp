#include <stdio.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\opencv.hpp"


using namespace std;
using namespace cv;

/*vector<Rect> markerdetection(const string& filepath_of_XML, const string& imagefilepath, Mat& image, vector<Rect> boxes, CascadeClassifier detect_marker)
{
	image = imread(imagefilepath, IMREAD_UNCHANGED);
	detect_marker.load(filepath_of_XML);
	//Detecting the markers on the image provided and storing these in the boxes vector of type rectangle
	detect_marker.detectMultiScale(image, boxes, 1.1, 3, 0, Size(15, 15));
	return boxes;
}*/

int main(int argc, char** argv) {
	
	DWORD startTime = GetTickCount();
	Mat image;
	const int x = 100;
	CascadeClassifier detect_marker, detect_xCorner;

	static int flag = 0, cornerFlag = 0, minFlag = 25;
	//Declaring a container of vector type to store the object bounding rectangle
	vector<Rect> boxes, xcornerBoxes;
	//Initialising an output file stream object
	ofstream boundingboxdata;

	//argv[1] = "D:\\IPCT_Testdaten\\UW9\\Seq_17_turn\\cam_1\\frame_000269.bmp";
	//argv[1] = "D:\\IPCT_Testdaten\\Cobra_wing\\meas_img\\run_30\\Cam1\\Cam1RH_Seq30_11h22m18s725ms_0000.bmp";
	//argv[1] = "D:\\IPCT_Testdaten\\PW6glider\\03_MEASUREMENT_images\\cam_2\\frame_000001.bmp";
	//argv[1] = "D:\\IPCT_Testdaten\\Airbus_A320\\2017-07-26_flight_282\\JCAM34_Seq_216\\cam_2\\frame_000010.bmp";
	argv[1] = "D:\\IPCT_Testdaten\\Airbus_A320\\2012-07-26_flight_283\\2012-07-26_14-32-23\\cam_2\\0001.tiff";
	//argv[1] = "D:\\IPCT_Testdaten\\Cobra_prop\\pictures\\cam_2\\FT_000296.bmp";
	//argv[1] = "D:\\Markers_UsedForTraining\\X_corners\\temp\\Capture.png";
	//argv[1] = "D:\\Markers_UsedForTraining\\SVMTraining\\whiteMarker\\1.png";
	//Absolute path of the trained XML file
	//argv[2] = "D:\\IPCT_Marker_Detection_Development\\MATLAB_Files\\XML_Files\\markerDetectorWhiteBG.xml";
	argv[2] = "D:\\IPCT_Marker_Detection_Development\\MATLAB_Files\\XML_Files\\markerDetectorAnyBackground.xml";	
	//Absolute path of the directory to which the data must be written
	argv[3] = "D:\\IPCT_Marker_Detection_Development\\Data\\DatamarkerPosition.txt";
	argv[4] = "D:\\IPCT_Marker_Detection_Development\\MATLAB_Files\\XML_Files\\xcornerDetector.xml";
	argc = 3; 
	
	if (argc > 1)
	{		
		//Extracting from the vector through iterator and plotting the detected objects 
		//boxes = markerdetection(argv[2], argv[1], image, boxes, detect_marker);
		image = imread(argv[1], IMREAD_ANYDEPTH);
		detect_marker.load(argv[2]);
		detect_xCorner.load(argv[4]);
		//Detecting the markers on the image provided and storing these in the boxes vector of type rectangle
		detect_marker.detectMultiScale(image, boxes, 1.1, 3, 0, Size(10, 10), Size(x,x));
		Rect *p = boxes.data();
		if ((boxes.size() >= 1) && (image.rows >= 10) && (image.rows >= 10))
		{
			
			boundingboxdata.open(argv[3]);
			boundingboxdata << p->x << "\t" << p->y << "\t" << p->x + p->width << "\t" << p->y + p->height << endl;
			boundingboxdata.close();

			/*cv::namedWindow("Displaying Actual Image", WINDOW_AUTOSIZE);
			cv::imshow("Displaying Actual Image", image);
			cv::waitKey();*/

			Rect roi(p->x, p->y, p->width, p->width);
			Mat subImage = image(roi);
			/*namedWindow("Sub-Image", WINDOW_NORMAL);
			imshow("Sub-Image", subImage);
			cv::waitKey();*/
			detect_xCorner.detectMultiScale(subImage, xcornerBoxes, 1.1, 3, 0);

			//Detecting Markers
			for (vector<Rect>::iterator r = boxes.begin(); r != boxes.end(); ++r)
			{
				flag = ((r->width) > flag) ? (r->width) : flag;
				minFlag = ((r->width) < minFlag) ? (r->width) : minFlag;
				/*if (flag<(r->width))
				{
					flag = r->width;
				}*/
				Rect detectedObjectBoundary = (*r);
  				rectangle(image, detectedObjectBoundary, Scalar(0, 0, 0), 2, 8, 0);				
			}
			putText(image, "Marker", Point2f(10, 10), CV_FONT_HERSHEY_PLAIN, 0.8, Scalar(255, 255, 255));

			//Detecting X-corners
			if (xcornerBoxes.size()>=1)
			{
				putText(image, "X-Corner", Point2f(30, 30), CV_FONT_HERSHEY_PLAIN, 0.8, Scalar(255, 255, 255));
				for (vector<Rect>::iterator r = xcornerBoxes.begin(); r != xcornerBoxes.end(); ++r)
				{
					if (cornerFlag<(r->width))
					{
						cornerFlag = r->width;
					}
					Rect detectedObjectBoundary = (*r);
					rectangle(subImage, detectedObjectBoundary, Scalar(0, 0, 0), 2, 8, 0);
				}
			}
			
			//Displaying the detected objects
			cv::namedWindow("Displaying Detetcted Objects", CV_WINDOW_FREERATIO);
			cv::imshow("Displaying Detetcted Objects", image);
			cv::waitKey(0);
			cv::destroyAllWindows();
			cout << "Maximum marker bounding box size:" << flag <<endl;
			cout << "Minimum marker bounding box size:" << minFlag << endl;
			cout << "Corner bounding box size:" << cornerFlag << endl;
			DWORD endTime = GetTickCount();
			cout << endTime - startTime << "ms\n";
			system("pause");
		}

		else
		{
			return -1;
			system("pause");
		}

	}

	else
	{
		return -1;
		system("pause");
	}

	return 1;
	system("pause");
}