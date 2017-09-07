#include <iostream>
#include "opencv2\opencv.hpp"

using namespace std;
using namespace cv;

float calibrationGridSideLength = 0.01905f;
Size chessBoardSize = Size(6, 9);
int fps = 1;

//This function returns the calibration grid coordinates w.r.t to its own plane
void createCalibrationGridCornerCoordinates(Size& boardSize, float& calibrationGridEdgeLength, vector<Point3f>& calibrationGridCorners)
{
	cout << "Inside createCalibrationGridCornerCoordinates() function\n";
	for (size_t i = 0; i < boardSize.height; i++)
	{
		for (size_t j = 0; j < boardSize.width; j++)
		{
			calibrationGridCorners.push_back(Point3f(i*calibrationGridEdgeLength, j*calibrationGridEdgeLength, 0.0f));
		}
	}
	cout << "Exiting createCalibrationGridCornerCoordinates() function\n";
}

void showChessBoardCornersFromImages(vector<Mat>& images, vector<vector<Point2f>>& chessboardcorners)
{
	cout << "Inside showChessBoardCornersFromImages() function\n";
	vector<Point2f> corners;
	vector<Mat>::iterator iter = images.begin();
	vector<vector<Point2f>>::iterator cornerIter = chessboardcorners.begin();
	for (;iter != images.end();iter++, cornerIter++)
	{
		namedWindow("Saved Images", WINDOW_AUTOSIZE);
		drawChessboardCorners(*iter, chessBoardSize, *cornerIter, true);
		imshow("Saved Images", *iter);
		waitKey(0);
	}
	cout << "Exiting showChessBoardCornersFromImages() function\n";
}
//This function detects the chessboard corners in the image pllane
void getChessBoardCornersFromImages(vector<Mat>& images, Size& calibrationBoardSize, vector<vector<Point2f>>& foundCorners, bool displayDetectedCorners)
{
	cout << "Inside getChessBoardCornersFromImages() function\n";
	bool found = false;
	vector<Point2f> detectedCorners;
	for (vector<Mat>::iterator i = images.begin(); i != images.end(); i++)
	{
		found = findChessboardCorners(*i, calibrationBoardSize, detectedCorners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
		if (found)
		{
			foundCorners.push_back(detectedCorners);
		}
		
	}
	//Display the detected corners from the images
	if (displayDetectedCorners)
	{
		showChessBoardCornersFromImages(images, foundCorners);
	}	
	cout << "Exiting getChessBoardCornersFromImages() function\n";
}

void cameraCalibration(vector<Mat>& images, Size& calibrationBoardSize, float& calibrationGridEdgeLength, Mat& cameraIntrinsics, Mat& cameraDistortions)
{
	cout << "Inside cameraCalibration() function\n";
	vector<Mat> rvecs, tvecs;
	//coordinates of chessboard corners w.r.t its image plane
	vector<vector<Point2f>> imagePoints;
	//coordinates of chessboard corners w.r.t its own plane
	vector<vector<Point3f>> objectPoints(1);
	//Detecting chessboard corners on the calibration images
	getChessBoardCornersFromImages(images, calibrationBoardSize, imagePoints, false);
	createCalibrationGridCornerCoordinates(calibrationBoardSize, calibrationGridEdgeLength, objectPoints[0]);
	objectPoints.resize(imagePoints.size(), objectPoints[0]);
	//calibrating camera
	double reprojectionerror = calibrateCamera(objectPoints, imagePoints, calibrationBoardSize, cameraIntrinsics, cameraDistortions, rvecs, tvecs);
	cout << reprojectionerror << endl;
	cout << "Exiting cameraCalibration() function\n";
}

bool saveCalibrationParameters(string& name, Mat& intrinsicMatrix, Mat& distortionMatrix)
{
	cout << "Inside saveCalibrationParameters() function\n";
	ofstream outStream(name);
	if (outStream)
	{
		uint16_t rows = intrinsicMatrix.rows;
		uint16_t columns = intrinsicMatrix.cols;

		outStream << rows << endl;
		outStream << columns << endl;
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < columns; j++)
			{
				outStream << intrinsicMatrix.at <double>(i, j) << endl;
			}
		}

		rows = distortionMatrix.rows;
		columns = distortionMatrix.cols;
		outStream << rows << endl;
		outStream << columns << endl;

		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < columns; j++)
			{
				outStream << distortionMatrix.at <double>(i, j) << endl;
			}
		}

		cout << "Exiting saveCalibrationParameters() function\n";
		return true;
	}
	cout << "Exiting saveCalibrationParameters() function\n";
	return false;
}

int main(int argc, char** argv[])
{
	cout << "Main loop started\n";
	Mat image;
	vector<Mat> calibrationImages;
	vector<vector<Point2f>> chessBoardCorners_CoordinatesinImages;
	vector<vector<Point3f>> chessBoardCorners_worldCoordinates;
	vector<Point3f> chessBoardCorners_Coordinates;
	Mat cameraIntrinsicCoefficients = Mat::eye(3, 3, CV_64F);
	Mat cameraDistortionCoefficients = Mat::zeros(8, 1, CV_64F);
	bool showCornersInImages = false;
	string filename = "cameraparams.xml";
	vector<String> imageFilesInDirectory;
	glob("*.png", imageFilesInDirectory, false);
	size_t imageCount = imageFilesInDirectory.size();
	cout << "Reading images for calibration\n";
	for (size_t i = 0; i < imageCount; i++)
	{
		calibrationImages.push_back(imread(imageFilesInDirectory[i],IMREAD_UNCHANGED));
	}
	cout << "Images for calibration acquired to Vector File\n";

	/*//Detecting chessboard corners from images
	cout << "Detecting chessboard corners from images\n";
	cout << "Leaving main\n";
	getChessBoardCornersFromImages(calibrationImages, chessBoardSize, chessBoardCorners_CoordinatesinImages, showCornersInImages);
	cout << "Returned to main\n";*/
	//calibrating the camera
	cout << "Calibrating camera\n";
	cout << "Leaving main\n";
	cameraCalibration(calibrationImages, chessBoardSize, calibrationGridSideLength, cameraIntrinsicCoefficients, cameraDistortionCoefficients);
	cout << "Returned to main\n";
	//writing the parameters to a xml
	cout << "Writing camera parameters to an xml file\n";
	cout << "Leaving main\n";
	saveCalibrationParameters(filename, cameraIntrinsicCoefficients, cameraDistortionCoefficients);
	cout << "Returned to main\n";
	cout << "Exiting main\n";
	system("pause");
	return 0;
}