#include <stdio.h>
#include <iostream>
#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "lbp.hpp"
#include "histogram.hpp"

using namespace std;
using namespace cv;

int main(int argc, const char *argv[])
{
	/*int deviceId = 0;
	if(argc > 1)
	deviceId = atoi(argv[1]);

	VideoCapture cap(deviceId);

	if(!cap.isOpened())
	{
	cerr << "Capture Device ID " << deviceId << " cannot be opened." << endl;
	system("pause");
	return -1;
	}*/

	// matrices used
	Mat frame, image1, image2; // always references the last frame
	Mat dst; // image after preprocessing
	Mat lbp; // lbp image

	String imagefile1 = "D:\\Markers_UsedForTraining\\Markers_Any_Background\\Positve_Mark4\\00000010.png";
	//imagefile1 = "D:\\IPCT_Testdaten\\UW9\\Seq_17_turn\\cam_1\\frame_000269.bmp";
	imagefile1 = "D:\\Markers_UsedForTraining\\New_Marker_Design\\23.png";

	String imagefile2 = "D:\\Markers_UsedForTraining\\New_Marker_Design\\bg1.png";

	image1 = imread(imagefile1, IMREAD_GRAYSCALE);
	image2 = imread(imagefile2, IMREAD_GRAYSCALE);
	resize(image1, image1, Size(100, 100));
	//bitwise_not(image1, image1);
	resize(image2, image2, Size(100, 100));
	//bitwise_not(image2, image2);
	//final image
	addWeighted(image1, 1, image2, 0.2, 0.0, frame);
	//namedWindow("final", CV_WINDOW_FREERATIO);
	//imshow("final", frame);
	//waitKey(0);
	// initial values
	int radius = 1;
	int neighbors = 8;

	// windows
	namedWindow("original", CV_WINDOW_FREERATIO);
	namedWindow("lbp", CV_WINDOW_FREERATIO);


	// just to switch between possible lbp operators
	vector<string> lbp_names;
	lbp_names.push_back("Extended LBP"); // 0
	lbp_names.push_back("Fixed Sampling LBP"); // 1
	lbp_names.push_back("Variance-based LBP"); // 2
	int lbp_operator = 2;

	bool running = true;
	while (running)
	{
		/*cap >> frame;*/
		/*frame = imread(image, IMREAD_GRAYSCALE);*/
		//bitwise_not(frame, dst);
		dst = frame;
		//cvtColor(frame, dst, CV_BGR2GRAY);
		GaussianBlur(dst, dst, Size(7, 7), 5, 3, BORDER_CONSTANT); // tiny bit of smoothing is always a good idea
		// comment the following lines for original size
		/*resize(frame, frame, Size(), 0.5, 0.5);
		resize(dst,dst,Size(), 0.5, 0.5);*/
		//
		switch (lbp_operator)
		{
		case 0:
			lbp::ELBP(dst, lbp, radius, neighbors); // use the extended operator
			break;
		case 1:
			lbp::OLBP(dst, lbp); // use the original operator
			break;
		case 2:
			lbp::VARLBP(dst, lbp, radius, neighbors);
			break;
		}
		// now to show the patterns a normalization is necessary
		// a simple min-max norm will do the job...
		normalize(lbp, lbp, 0, 255, NORM_MINMAX, CV_8UC1);

		imshow("original", frame);
		imshow("lbp", lbp);

		char key = (char)waitKey(0);

		// exit on escape
		if (key == 27)
			running = false;

		//  	// to make it a bit interactive, you can increase and decrease the parameters
		//  	switch(key) 
		//{
		//  		case 'q': case 'Q':
		//  			running=false;
		//  			break;
		//  		// lower case r decreases the radius (min 1)
		//  		case 'r':
		//  			radius-=1;
		//  			radius = std::max(radius,1);
		//  			cout << "radius=" << radius << endl;
		//  			break;
		//  		// upper case r increases the radius (there's no real upper bound)
		//  		case 'R':
		//  			radius+=1;
		//  			radius = std::min(radius,32);
		//  			cout << "radius=" << radius << endl;
		//  			break;
		//  		// lower case p decreases the number of sampling points (min 1)
		//  		case 'p':
		//  			neighbors-=1;
		//  			neighbors = std::max(neighbors,1);
		//  			cout << "sampling points=" << neighbors << endl;
		//  			break;
		//  		// upper case p increases the number of sampling points (max 31)
		//  		case 'P':
		//  			neighbors+=1;
		//  			neighbors = std::min(neighbors,31);
		//  			cout << "sampling points=" << neighbors << endl;
		//  			break;
		//  		// switch between operators
		//  		case 'o': case 'O':
		//  			lbp_operator = (lbp_operator + 1) % 3;
		//  			cout << "Switched to operator " << lbp_names[lbp_operator] << endl;
		//  			break;
		//  		case 's': case 'S':
		//  			imwrite("original.jpg", frame);
		//  			imwrite("lbp.jpg", lbp);
		//  			cout << "Screenshot (operator=" << lbp_names[lbp_operator] << ",radius=" << radius <<",points=" << neighbors << ")" << endl;
		//  			break;
		//  		default:
		//  			break;
		//}
	}
	/*system("pause");*/
	return 0; // success
}
