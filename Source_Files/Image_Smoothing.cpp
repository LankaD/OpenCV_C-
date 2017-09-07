/*This program performs smoothing  operations depending upon the user choice of the type of smoothing to be performed*/

#include <iostream>
#include <string>
#include"opencv2\opencv.hpp"

using namespace std;
using namespace cv;

Mat image_In, image_Out;

void smoothImage(const Mat& image_In, const int& choice) {
	switch (choice)
	{
	case 1:
		GaussianBlur(image_In, image_Out, Size(5, 5), 5, 5);
		break;
	case 2:
		medianBlur(image_In, image_Out, 3);
		break;
	case 3:
		bilateralFilter(image_In, image_Out, 30, 150, 150);
		break;
	case 4:
		pyrDown(image_In, image_Out);
		break;
	}

}

int main(int argc, char** argv) {
	int choice = 0;
	static int flag = 0;
	string file;
	//image_In = imread("C:/Users/JAI/Pictures/PoloMod.JPG", IMREAD_UNCHANGED);

	do
	{
		if (argc > 1)
		{
			flag = 1;
			image_In = imread(argv[1], IMREAD_UNCHANGED);
		}
		else
		{
			cout << "Please try running the executable again by entering the absolute path of the image file as an argument." << endl;
			return -1;

		}
	} while (flag == 0);

	if (image_In.empty())
	{
		cout << "Image not available" << endl;
		system("pause");
		return -1;
	}
	cout << "Please select the type of smoothing you want to perform from the list below.\n 1. Gaussian Blur\n 2. Median Blur\n 3. Bilateral Filter\n 4. Downsample \n Enter the choice:  ";
	cin >> choice;
	smoothImage(image_In, choice);
	namedWindow("Input-Image", WINDOW_AUTOSIZE);
	namedWindow("Output-Image", WINDOW_AUTOSIZE);
	imshow("Input-Image", image_In);
	imshow("Output-Image", image_Out);
	waitKey(0);
	destroyAllWindows();
	return 0;
}