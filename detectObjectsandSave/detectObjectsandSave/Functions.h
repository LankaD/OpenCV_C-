#pragma once
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\opencv.hpp"

using namespace std;
using namespace cv;

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//This funciton checks if files exist in the directory
bool checkPath(const string& filepath);

//This funciton checks number of files in the directory
void fileCount(const string& filepath, int& fileCount);

//This function loads images from a file directory
void loadImagesFromDirectory(const string& filepath, vector<Mat>& imageFiles);

//This function recognizes objects using object classifier and returns the object bounding rectangle
void markerClassifierDetection(vector<Mat>& images, const string& classifierPath, vector<vector<Rect>>& boundingBoxes, CascadeClassifier& markerDetector, vector<vector<Point2f>>& cornerGuesses, bool saveImageSegments, bool showDetectedObjects, const string& saveSegmentDirectoryPath, int &filenumber, const int& flipImage, const int& flipFlag);

//This function determines corners in an image segment
void cornersDetection(const Mat& image, bool showDetectedCorners);

//This function extracts ROI from an image when the bounding rectangle origin, width and height is given
void extractObjectROI(const Mat& image, vector<Rect>& boundingBoxInfo, bool showROI);

//This function returns an initial x-corner guess of the detected marker
void cornerGuessInSegment(Mat& image, vector<Rect>& boundingBox, vector<Point2f>& cornerGuesses, bool showCornerPoints);
#endif
