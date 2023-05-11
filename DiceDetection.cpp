#include "DiceDetection.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

VideoCapture cap(1);

DiceDetection::DiceDetection() {

}

int DiceDetection::startDetection()
{
	// Check if the camera was opened successfully
	if (!cap.isOpened())
	{
		cout << "Error opening video stream or file" << endl;
		return -1;
	}


	// Capture a frame from the camera
	Mat frame;
	cap.read(frame);

	// Convert the frame to grayscale
	Mat grayImage;
	cvtColor(frame, grayImage, COLOR_BGR2GRAY);

	// Threshold the grayscale image to create a binary image
	Mat binaryImage;
	threshold(grayImage, binaryImage, 50, 255, THRESH_BINARY);

	// Set up SimpleBlobDetector parameters
	SimpleBlobDetector::Params params;
	params.filterByArea = true;
	params.minArea = 100;
	params.maxArea = 5000;
	params.filterByCircularity = true;
	params.minCircularity = 0.1;
	params.filterByConvexity = false;
	params.filterByInertia = false;

	// Create a SimpleBlobDetector object with the parameters
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

	// Detect blobs in the binary image
	vector<KeyPoint> keypoints;
	detector->detect(binaryImage, keypoints);

	// Count the number of dice
	int numDice = 0;
	for (size_t i = 0; i < keypoints.size(); i++)
	{
		if (keypoints[i].size > 50)
		{
			numDice++;
		}
	}

	// Print the number of dice and the number of blobs
	cout << "Number of dice: " << numDice << endl;
	cout << "Number of blobs: " << keypoints.size() << endl;

	// Draw the keypoints on the original image
	Mat imageWithKeypoints;
	drawKeypoints(frame, keypoints, imageWithKeypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	// Show the image with keypoints in a window
	namedWindow("Die Image", WINDOW_NORMAL);
	imshow("Die Image", imageWithKeypoints);
	imshow("Die Image2", binaryImage);

	// Press ESC to exit
	//char c = waitKey(1);
	//if (c == 27)
	//{
	//    break;
	//}


// Release the camera and close all windows
//cap.release();
	

	return keypoints.size();
}

void DiceDetection::stop() {
	destroyAllWindows();
	cap.release();
}

