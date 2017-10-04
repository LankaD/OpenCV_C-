#include <stdio.h>
#include <iostream>
#include <fstream>
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

	Mat image;
	CascadeClassifier detect_marker;
	//Declaring a container of vector type to store the object bounding rectangle
	vector<Rect> boxes;
	//Initialising an output file stream object
	ofstream boundingboxdata;

	//argv[1] = "D:\\Markers_UsedForTraining\\SVMTraining\\blackMarker\\37.png";
	argv[1] = "D:\\Markers_UsedForTraining\\X_corners\\temp\\Capture.png";
	//argv[1] = "D:\\IPCT_Testdaten\\Cobra_prop\\pictures\\cam_2\\FT_000296.bmp";
	//Absolute path of the trained XML file
	//argv[2] = "D:\\IPCT_Marker_Detection_Development\\MATLAB_Files\\XML_Files\\markerDetectorWhiteBG.xml";
	argv[2] = "D:\\Markers_UsedForTraining\\X_corners\\temp\\xmlfiles\\xcornerDetector.xml";
	//Absolute path of the directory to which the data must be written
	argv[3] = "D:\\IPCT_Marker_Detection_Development\\Data\\DatamarkerPosition.txt";
	argc = 3;
	image = imread(argv[1], IMREAD_GRAYSCALE);
	cv::namedWindow("Displaying Detetcted Objects", WINDOW_AUTOSIZE);
	cv::imshow("Displaying Detetcted Objects", image);
	cv::waitKey();
	if (argc > 1)
	{
		//Extracting from the vector through iterator and plotting the detected objects 
		//boxes = markerdetection(argv[2], argv[1], image, boxes, detect_marker);
		
		detect_marker.load(argv[2]);
		cv::namedWindow("Displaying Detetcted Objects", WINDOW_AUTOSIZE);
		cv::imshow("Displaying Detetcted Objects", image);
		//Detecting the markers on the image provided and storing these in the boxes vector of type rectangle
		detect_marker.detectMultiScale(image, boxes, 1.1, 3, 0);

 		if ((boxes.size() >= 1))
		{
			Rect *p = boxes.data();
			boundingboxdata.open(argv[3]);
			boundingboxdata << p->x << "\t" << p->y << "\t" << p->x + p->width << "\t" << p->y + p->height << endl;
			boundingboxdata.close();
			for (vector<Rect>::iterator r = boxes.begin(); r != boxes.end(); ++r)
			{
				Rect detectedObjectBoundary = (*r);
				rectangle(image, detectedObjectBoundary, Scalar(0, 0, 0), 2, 8, 0);
			}

			//Displaying the detected objects
			cv::namedWindow("Displaying Detetcted Objects", WINDOW_AUTOSIZE);
			cv::imshow("Displaying Detetcted Objects", image);
			cv::waitKey();
			cv::destroyAllWindows();			
			//system("pause");
		}

		else
		{
			cout << "No Object found\n";
			system("pause");
			return -1;
			
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