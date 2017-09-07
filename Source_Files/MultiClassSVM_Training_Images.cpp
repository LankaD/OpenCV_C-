/*labels
	0	- black marker on white background
	1	- white marker on black background
	-1	- background other than marker
*/
#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

bool checkPath(const string& filepath);
void loadImagesFromDirectory(const string& filepath, vector<Mat>& imageFiles, vector<Mat>& trainingImages);
void images2SVMType(vector<Mat>& trainingImages, Mat& trainingData);
void trainMultiClassSVM(const vector<Mat>& features, const vector<int>& labels);
void svmPredictImage(vector<Mat>& testImages, Size& imageSize);
Size imageSize(100, 100);

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

void loadImagesFromDirectory(const string& filepath, vector<Mat>& imageFiles, vector<Mat>& trainingImages)
{
	Mat image;
	vector<String> filesInFolder;
	// Reading set of images from a folder
	glob(filepath, filesInFolder, false);
	for (size_t i = 0; i < filesInFolder.size(); i++)
	{
		image = imread(filesInFolder[i], IMREAD_GRAYSCALE);
		resize(image, image, imageSize);
		imageFiles.push_back(image);
		trainingImages.push_back(image);
	}
}


void trainMultiClassSVM(const Mat& trainingdata, const vector<int>& labels)
{
	CvSVMParams parameters;
	parameters.svm_type = CvSVM::C_SVC;
	CvSVM svm;
	svm.train(trainingdata, Mat(labels), Mat(), Mat());
	svm.save("svm_imagesUsed.xml");
}

void svmPredictImage(vector<Mat>& testImages, Size& imageSize)
{
	vector<Mat> featuredescriptors;
	CvSVM svmTest;
	Mat svmReadableImage, testImage;
	testImage = testImages[0];
	resize(testImage, testImage, imageSize);	
	svmReadableImage = featuredescriptors[0];
	svmReadableImage = svmReadableImage.reshape(0, 1);
	cout << "Loading SVM XML\n";
	svmTest.load("svm_trial.xml");
	cout << "SVM loaded\n";
	int response = svmTest.predict(svmReadableImage);
	if (response == 0)
	{
		cout << "Black Marker with white background\n";
	}
	else if (response == 1)
		 {
			 cout << "White Marker with black background\n";
		 }
		 else
		 {
			 cout << "Background image\n";
		 }
}

int main(int argc, char**argv)
{
	vector<Mat> testImages;
	Mat testImage, trainingData;
	//Declaring variables
	vector<Mat> blackMarkerImages, whiteMarkerImages, backgroundImages, trainingImages;
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
		loadImagesFromDirectory(blackMarkerImages_filepath, blackMarkerImages, trainingImages);
		labels.assign(blackMarkerImages.size(), 0);
		cout << "The total number of black marker images loaded are: " << blackMarkerImages.size() << endl;
	
		loadImagesFromDirectory(whiteMarkerImages_filepath, whiteMarkerImages, trainingImages);
		labels.insert(labels.end(), whiteMarkerImages.size(), +1);
		cout << "The total number of white marker images loaded are: " << whiteMarkerImages.size() << endl;
	
		loadImagesFromDirectory(backgroundImages_filepath, backgroundImages, trainingImages);
		labels.insert(labels.end(), backgroundImages.size(), -1);
		cout << "The total number of background images loaded are: " << backgroundImages.size() << endl;
	}
	images2SVMType(trainingImages, trainingData);
	trainMultiClassSVM(trainingData, labels);
	system("pause");
	return 0;
}

void images2SVMType(vector<Mat>& trainingImages, Mat& trainingData)
{
	Mat image;
	vector<Mat> temp;
	int r = 0;
	vector<Mat>::iterator i = trainingImages.begin();
	for (; i < trainingImages.end(); i++)
	{
		image = *i;
		vector<float> array(image.rows*image.cols);
		/*if (image.isContinuous())
			cout << "yes";*/
		image = image.reshape(0, 1);
		array.assign((float*)image.datastart, (float*)image.dataend);
		temp.push_back(image);		
	}
	trainingImages = temp;

	 int rows = (int)temp.size();
	 int cols = (int)max(temp[0].cols, temp[0].rows);
	Mat tmp(1, cols, CV_32FC1); // used for transposition if needed
	trainingData = Mat(rows, cols, CV_32FC1);
	vector< Mat >::iterator itr = temp.begin();
	vector< Mat >::iterator end = temp.end();
	for (int i = 0; itr != end; ++itr, ++i)
	{
		CV_Assert(itr->cols == 1 ||
			itr->rows == 1);
		if (itr->cols == 1)
		{
			transpose(*(itr), tmp);
			tmp.copyTo(trainingData.row(i));
		}
		else if (itr->rows == 1)
		{
			itr->copyTo(trainingData.row(i));
		}
	}
	

}