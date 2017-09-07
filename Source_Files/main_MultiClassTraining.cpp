/*labels
	0	- black marker on white background
	1	- white marker on black background
	-1	- background other than marker
*/

#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using namespace cv;

bool checkPath(const string& filepath);
void loadImagesFromDirectory(const string& filepath, vector<Mat>& imageFiles);
void computeHOGFeatures(const vector<Mat>& images, vector<Mat>& HOGFeatures, const Size& wSize);
void convert_to_ml(const vector<Mat > & train_samples, Mat& trainData);
void trainMultiClassSVM(const vector<Mat>& features, const vector<int>& labels);

bool checkPath(const string& filepath)
{
	DWORD checkFileExists = GetFileAttributesA(filepath.c_str());
	if (checkFileExists == INVALID_FILE_ATTRIBUTES)
	{
		cout << "Something is wrong with the entered path\n";
		return 0;
	}

	else if (checkFileExists & FILE_ATTRIBUTE_DIRECTORY)
	{
		return 1;
	}
}

void loadImagesFromDirectory(const string& filepath, vector<Mat>& imageFiles)
{
	Mat image;
	vector<String> filesInFolder;
	// Reading set of images from a folder
	glob(filepath, filesInFolder, false);
	for (size_t i = 0; i < filesInFolder.size(); i++)
	{
		image = imread(filesInFolder[i], IMREAD_UNCHANGED);
		imageFiles.push_back(image);
	}
}

void computeHOGFeatures(const vector<Mat>& images, vector<Mat>& HOGFeatures, const Size& wSize)
{
	HOGDescriptor hog;
	hog.winSize = wSize;
	hog.winSize = wSize;
	hog.blockSize = Size(16, 16);
	hog.blockStride = Size(8, 8);
	hog.cellSize = Size(8, 8);
	hog.nbins = 9;
	//hog.winSigma = -1;
	//hog.L2HysThreshold = 0.2;
	//hog.gammaCorrection = true,
	//hog.nlevels = 64;
	Mat image;
	vector<float> featureVector;
	vector<Point> location;

	for (vector<Mat>::const_iterator iter = images.begin(); iter != images.end(); iter++)
	{
		image = *iter;

		if (image.channels() == 3)
		{
			cvtColor(image, image, CV_BGR2GRAY);
		}
		hog.compute(image, featureVector, Size(8, 8), Size(8, 8));
		HOGFeatures.push_back(Mat(featureVector).clone());		
	}	
}

/*
* Convert training/testing set to be used by OpenCV Machine Learning algorithms.
* TrainData is a matrix of size (#samples x max(#cols,#rows) per samples), in 32FC1.
* Transposition of samples are made if needed.
*/
void convert_to_ml(const vector<Mat > & train_samples, Mat& trainData)
{
	const int rows = (int)train_samples.size();
	const int cols = (int)max(train_samples[0].cols, train_samples[0].rows);
	Mat tmp(1, cols, CV_32FC1); // used for transposition if needed
	trainData = Mat(rows, cols, CV_32FC1);
	vector< Mat >::const_iterator itr = train_samples.begin();
	vector< Mat >::const_iterator end = train_samples.end();
	for (int i = 0; itr != end; ++itr, ++i)
	{
		CV_Assert(itr->cols == 1 ||
			itr->rows == 1);
		if (itr->cols == 1)
		{
			transpose(*(itr), tmp);
			tmp.copyTo(trainData.row(i));
		}
		else if (itr->rows == 1)
		{
			itr->copyTo(trainData.row(i));
		}
	}
}


void trainMultiClassSVM(const vector<Mat>& features, const vector<int>& labels)
{
	Mat traindata;
	convert_to_ml(features, traindata);
	CvSVM svm;
	/*CvSVMParams params;
	params.coef0 = (0, 0);
	params.degree = 3;
	param*/
	svm.train(traindata, Mat(labels), Mat(), Mat());
	svm.save("svm_trial.xml");
}

int main(int argc, char**argv)
{
	//Declaring variables
	vector<Mat> blackMarkerImages, whiteMarkerImages, backgroundImages, blackMarkerHOGFeatures, whiteMarkerHOGFeatures,  backgroundImagesHOGFeatures, featuredescriptors;
	Size wSize = Size(96, 96);
	vector <int> labels; 
	bool filePathIsCorrect_blackMarker = 0, filePathIsCorrect_whiteMarker = 0, filePathIsCorrect_background = 0;

	const string blackMarkerImages_filepath = "D:\\Markers_UsedForTraining\\SVMTraining\\blackMarker";//black markers with white background
	const string whiteMarkerImages_filepath = "D:\\Markers_UsedForTraining\\SVMTraining\\whiteMarker";//white markers with black background
	const string backgroundImages_filepath = "D:\\Markers_UsedForTraining\\SVMTraining\\background";//background images
	
	//Initializing file paths
	filePathIsCorrect_blackMarker = checkPath(blackMarkerImages_filepath);//black markers with white background
	filePathIsCorrect_whiteMarker = checkPath(whiteMarkerImages_filepath);//white markers with black background
	filePathIsCorrect_background = checkPath(backgroundImages_filepath); //background images

	//File path validation and reading image files from valid file paths'
	if (filePathIsCorrect_blackMarker && filePathIsCorrect_whiteMarker && filePathIsCorrect_background)
	{
		loadImagesFromDirectory(blackMarkerImages_filepath, blackMarkerImages);
		labels.assign(blackMarkerImages.size(), 0);
		cout << "The total number of black marker images loaded are: " << blackMarkerImages.size() << endl;
	
		loadImagesFromDirectory(whiteMarkerImages_filepath, whiteMarkerImages);
		labels.insert(labels.end(), whiteMarkerImages.size(), +1);
		cout << "The total number of white marker images loaded are: " << whiteMarkerImages.size() << endl;
	
		loadImagesFromDirectory(backgroundImages_filepath, backgroundImages);
		labels.insert(labels.end(), backgroundImages.size(), -1);
		cout << "The total number of background images loaded are: " << backgroundImages.size() << endl;
	}

	//Computing the HOG Features for the images
	computeHOGFeatures(blackMarkerImages, featuredescriptors, Size(96, 96));//black markers with white background
	computeHOGFeatures(whiteMarkerImages, featuredescriptors, Size(96, 96));//white markers with black background
	computeHOGFeatures(backgroundImages, featuredescriptors, Size(96, 96));//background images
	trainMultiClassSVM(featuredescriptors, labels);
	system("pause");
	return 0;
}
