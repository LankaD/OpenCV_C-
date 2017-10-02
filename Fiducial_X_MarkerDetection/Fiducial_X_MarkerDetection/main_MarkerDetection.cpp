#include <stdio.h>
#include <iostream>
#include <windows.h>
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\opencv.hpp"


using namespace std;
using namespace cv;

inline bool fileExists(const string& name) 
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

//his funciton checks if files exist in the directory
bool checkPath(const string& filepath);
//This function loads images from a file directory
void loadImagesFromDirectory(const string& filepath, vector<Mat>& imageFiles);
//This function recognizes objects using object classifier and returns the object bounding rectangle
void markerClassifierDetection(vector<Mat>& images, const string& classifierPath, vector<vector<Rect>>& boundingBoxes, CascadeClassifier& markerDetector, vector<vector<Point2f>>& cornerGuesses, bool showDetectedObjects);
//This function determines corners in an image segment
void cornersDetection(const Mat& image, bool showDetectedCorners);
//This function extracts ROI from an image when the bounding rectangle origin, width and height is given
void extractObjectROI(const Mat& image, vector<Rect>& boundingBoxInfo, bool showROI);
//This function returns an initial x-corner guess of the detected marker
void cornerGuessInSegment(Mat& image, vector<Rect>& boundingBox, vector<Point2f>& cornerGuesses, bool showCornerPoints);

//main starts here
int main(int argc, char** argv)
{
	DWORD startTime = GetTickCount();
	Mat image;
	vector<Mat> images;
	CascadeClassifier markerDetector;
	vector<vector<Rect>> boundingBoxes;
	vector<Point2f> corners;
	vector<vector<Point2f>>cornerGuesses;
	bool filePathIsCorrect_images = 0, filePathIsCorrect_classifier = 0;
	//The input arguments are the absolute path of image file and the marker classifier XML file
	argc = 2;
	//Absolute path of images directory
	const string imagesFilepath = "E:\\GAC_Files\\Images_From_Test_Runs\\2017-07-26_flight_282\\JCAM34_Seq_216\\cam_1";
	//argv[3] = "E:\\GAC_Files\\Images_From_Test_Runs\\2017-07-26_flight_282\\JCAM34_Seq_216\\cam_2\\frame_000001.bmp";
	//Absolute path of classifier file
	const string classifierFilePath = "E:\\GitHub\\Masters_Work\\XML_Files\\markerDetectorAnyBackground.xml";
	filePathIsCorrect_images = checkPath(imagesFilepath);
	filePathIsCorrect_classifier = fileExists(classifierFilePath);

	if (filePathIsCorrect_images && filePathIsCorrect_classifier)
	{
		loadImagesFromDirectory(imagesFilepath, images);
		markerClassifierDetection(images, classifierFilePath, boundingBoxes, markerDetector, cornerGuesses, false);
	}
	else
	{
		cout << "Something is wrong with the entered path. The image files or the marker classifier XML file is not found.\n";
	}
	destroyAllWindows();
	DWORD endTime = GetTickCount();
	cout << endTime - startTime << "ms\n";
	system("pause");
}
//main ends here

//This funciton checks if files exist in the directory
bool checkPath(const string& filepath)
{
	DWORD checkFileExists = GetFileAttributesA(filepath.c_str());
	if (checkFileExists == INVALID_FILE_ATTRIBUTES)
	{
		return 0;
	}

	else if (checkFileExists & FILE_ATTRIBUTE_DIRECTORY)
	{
		return 1;
	}
}

//This function loads images from a file directory
void loadImagesFromDirectory(const string& filepath, vector<Mat>& images)
{
	Mat image;
	vector<String> filesInFolder;
	// Reading set of images from a folder
	glob(filepath, filesInFolder, false);
	for (size_t i = 0; i < filesInFolder.size(); i++)
	{
		image = imread(filesInFolder[i], IMREAD_GRAYSCALE);
		images.push_back(image);
	}
}

//This function recognizes objects using object classifier and returns the marker bounding rectangle along with an initial corner guess of the marker
void markerClassifierDetection(vector<Mat>& images, const string& classifierPath, vector<vector<Rect>>& boundingBoxes, CascadeClassifier& markerDetector, vector<vector<Point2f>>& cornerGuesses, bool showDetectedObjects)
{
	RNG rng(12345);
	Scalar color = Scalar(255, rng.uniform(0, 100), 0);
	Mat image;
	vector<Rect>boundingBoxInfo;
	vector<Point2f>cornersGuess;
	markerDetector.load(classifierPath);	
	for (vector<Mat>::iterator i = images.begin(); i != images.end(); i++)
	{
		image = *i;
		markerDetector.detectMultiScale(*i, boundingBoxInfo, 1.1, 3, 0, Size(20, 20));
		if (boundingBoxInfo.size()>0)
		{
			boundingBoxes.push_back(boundingBoxInfo);
			cornerGuessInSegment(*i, boundingBoxInfo, cornersGuess, false);
			cornerGuesses.push_back(cornersGuess);			
			if (showDetectedObjects)
			{
				for (vector<Rect>::iterator r = boundingBoxInfo.begin(); r != boundingBoxInfo.end(); ++r)
				{
					Rect objectBoundary = (*r);
					rectangle(*i, objectBoundary, color, 2, 8, 0);
				}
				//Drawing detected corners
				for (int i = 0; i < cornersGuess.size(); i++)
				{
					circle(image, cornersGuess[i], 2, color, 0, 8, 0);
				}
				namedWindow("Image", WINDOW_NORMAL);
				//cvtColor(*i, image, COLOR_GRAY2BGR);
				imshow("Image", image);
				waitKey(0);
			}
			cornersGuess.clear();
		}
		else
		{
			if ((boundingBoxInfo.size() == 0))
			{
				cout << "No objects detected\n";
			}
		}
	}
}

//This function returns an initial x-corner guess of the detected marker
void cornerGuessInSegment(Mat& image, vector<Rect>& boundingBox, vector<Point2f>& cornerGuesses, bool showCornerPoints)
{
	Point2f cornerGuess;
	float x = 0, y = 0;
	for (vector<Rect>::iterator c = boundingBox.begin(); c != boundingBox.end(); c++)
	{
		x = c->x + (c->width / 2);
		y = c->y + (c->height / 2);
		cornerGuess = Point2f(x, y);
		cornerGuesses.push_back(cornerGuess);
	}
}

//This function determines corners in an image segment
void cornersDetection(const Mat& image, bool showDetectedCorners)
{
	//Parameters for Shi-Tomasi corner detector
	Mat image_Out;
	vector<Point2f> corners;
	double qualityLevel = 0.01;
	double minDistance = 5;
	int blockSize = 3, maxCorners = 10;
	bool useHarrisDetector = false;
	double k = 0.04;
	image.copyTo(image_Out);
	GaussianBlur(image, image_Out, Size(5, 5), 5, 5);
	//Apply corner detection
	goodFeaturesToTrack(image_Out, corners, maxCorners, qualityLevel, minDistance, Mat(), blockSize, useHarrisDetector, k);

	//Drawing detected corners
	for (int i = 0; i < corners.size(); i++)
	{
		circle(image, corners[i], 1, Scalar(0, 0, 255), -1, 8, 0);
	}
}

//This function extracts ROI from an image when the bounding rectangle origin, width and height is given
void extractObjectROI(const Mat& image, vector<Rect>& boundingBoxInfo, bool showROI)
{
	for (vector<Rect>::iterator r = boundingBoxInfo.begin(); r != boundingBoxInfo.end(); ++r)
	{
		Rect ROI(r->x, r->y, r->width, r->height);
		Mat imageROI = image(ROI);
		cornersDetection(imageROI, showROI);
		if (showROI)
		{
			namedWindow("ROI", WINDOW_KEEPRATIO);
			imshow("ROI", imageROI);
			waitKey(0);
		}
	}
}