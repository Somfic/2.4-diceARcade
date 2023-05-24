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
float calculateDistance(const KeyPoint& p1, const KeyPoint& p2);

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

	Mat invertedImage;
	bitwise_not(binaryImage, invertedImage);

	Mat erodedImage;
	erodeImage(&invertedImage, &erodedImage);

	Mat dilatedImage;
	dilateImage(&invertedImage, &dilatedImage);

	bitwise_not(erodedImage, erodedImage);
	bitwise_not(dilatedImage, dilatedImage);

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
	int dice1 = 0;
	int dice2 = 0;
	if (keypoints.size() > 0)
	{
		numDice = 1;
		// Sort keypoints based on the x-coordinate
		sort(keypoints.begin(), keypoints.end(), [](const KeyPoint& a, const KeyPoint& b) {
			return a.pt.x < b.pt.x;
			});

		float smallestDistance = 0;
		int smallestDistanceIndex = 0;

		// Find smallest distance between 2 keypoints
		KeyPoint lastPoint = keypoints[keypoints.size() - 1];
		for (int j = 0; j < keypoints.size(); j++) {
			KeyPoint referencePoint = keypoints[j];
			for (int i = 0; i < keypoints.size(); i++) {
				KeyPoint currentPoint = keypoints[i];
				float distance = calculateDistance(currentPoint, referencePoint);

				if (smallestDistance == 0 || smallestDistance > distance && distance != 0) {
					smallestDistance = distance;
					smallestDistanceIndex = j;
				}
					
			}
		}
		cout << "smallest distance " << smallestDistance << endl;
		KeyPoint referencePoint = keypoints[smallestDistanceIndex];
		for (int i = 0; i < keypoints.size(); i++)
		{
			KeyPoint currentPoint = keypoints[i];
			float distance = calculateDistance(currentPoint, referencePoint);
			
			cout << "distance: " << distance << endl ;
			if (smallestDistance *3 < distance) {

				numDice = 2;
				dice1++;
				continue;
			}
		}
	}

	// Print the number of dice and the number of blobs
	
	cout << "Number of blobs: " << keypoints.size() << endl;
	cout << "dice 1: " << dice1 << endl;
	cout << "dice 2: " << keypoints.size() - dice1 << endl;
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
	imshow("binary Image", binaryImageWithKeypoints);
	imshow("inverted image", invertedImage);
	imshow("eroded Image", erodedImageWithKeypoints);
	imshow("dilated Image", dilatedImageWithKeypoints);

	return keypoints.size();
}

void DiceDetection::stop() {
	destroyAllWindows();
	cap.release();
}

float calculateDistance(const KeyPoint& p1, const KeyPoint& p2) {
	return sqrt(pow(p1.pt.x - p2.pt.x, 2) + pow(p1.pt.y - p2.pt.y, 2));
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
	int dilation_size = 2;
	int dilation_elem = MORPH_ELLIPSE;
	Mat element = getStructuringElement(dilation_type,
		Size(2 * dilation_size + 1, 2 * dilation_size + 1),
		Point(dilation_size, dilation_size));
	dilate(*originalImage, *newImage, element);
}


