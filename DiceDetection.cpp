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

void erodeImage(Mat *originalImage, Mat *newImage);
void dilateImage(Mat *originalImage, Mat *newImage);

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
	
	threshold(grayImage, binaryImage, 100, 255, THRESH_BINARY);

	Mat erodedImage;
	erodeImage(&binaryImage, &erodedImage);

	Mat dilatedImage;
	dilateImage(&binaryImage, &dilatedImage);



	// Set up SimpleBlobDetector parameters
	SimpleBlobDetector::Params params;
	params.filterByArea = true;
	params.minArea = 100;
	params.maxArea = 5000;
	params.filterByCircularity = true;
	params.minCircularity = 0.8;
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
	Mat binaryImageWithKeypoints;
	Mat erodedImageWithKeypoints;
	Mat dilatedImageWithKeypoints;
	drawKeypoints(frame, keypoints, imageWithKeypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(binaryImage, keypoints, binaryImageWithKeypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(erodedImage, keypoints, erodedImageWithKeypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(dilatedImage, keypoints, dilatedImageWithKeypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	// Show the image with keypoints in a window
	namedWindow("Die Image", WINDOW_NORMAL);
	imshow("Die Image", imageWithKeypoints);
	imshow("Die Image2", binaryImageWithKeypoints);
	imshow("eroded Image", erodedImageWithKeypoints);
	imshow("dilated Image", dilatedImageWithKeypoints);

	return keypoints.size();
}

void DiceDetection::stop() {
	destroyAllWindows();
	cap.release();
}

void erodeImage(Mat *originalImage, Mat *newImage) {
	int erosion_size = 2;
	int erosion_type = MORPH_ELLIPSE;
	Mat element = getStructuringElement(erosion_type,
		Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		Point(erosion_size, erosion_size));
	erode(*originalImage, *newImage, element);
}

void dilateImage(Mat *originalImage, Mat *newImage) {
	int dilation_type = 2;
	int dilation_size = 5;
	int dilation_elem = MORPH_ELLIPSE;
	Mat element = getStructuringElement(dilation_type,
		Size(2 * dilation_size + 1, 2 * dilation_size + 1),
		Point(dilation_size, dilation_size));
	dilate(*originalImage, *newImage, element);
}


