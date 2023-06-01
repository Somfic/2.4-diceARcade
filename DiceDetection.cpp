#include "DiceDetection.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <queue>

using namespace cv;
using namespace std;

VideoCapture cap(0);
bool isRunning = false;
Mat imageWithKeypoints;

bool validResult(vector<int>);
void erodeImage(Mat *originalImage, Mat *newImage);
void dilateImage(Mat *originalImage, Mat *newImage);
float calculateDistance(const KeyPoint& p1, const KeyPoint& p2);

DiceDetection::DiceDetection() {

}

void DiceDetection::startDetection(void (*callback)(const std::vector<int>&))
{	
	// Check if the camera was opened successfully
	if (!cap.isOpened())
	{
		cout << "Error opening video stream or file" << endl;
		return;
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

	Mat morphedImage;
	morphologyEx(binaryImage, morphedImage, MORPH_GRADIENT, 1);
	//erodeImage(&invertedImage, &erodedImage);

	Mat dilatedImage;
	dilateImage(&invertedImage, &dilatedImage);

	//bitwise_not(morphedImage, morphedImage);
	bitwise_not(dilatedImage, dilatedImage);

	Mat cannyImage;
	int treshold = 50;
	int ratio = 3;
	blur(grayImage, cannyImage, Size(3, 3));
	Canny(cannyImage, cannyImage, treshold, treshold * ratio, 3);
	imshow("canny Image", cannyImage);

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
	
	vector<vector<Point>> contours;
	findContours(cannyImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	std::vector<cv::Rect> diceContours;
	for (const auto& contour : contours) {
		double area = cv::contourArea(contour);
		cv::Rect boundingRect = cv::boundingRect(contour);
		double aspectRatio = static_cast<double>(boundingRect.width) / boundingRect.height;
		if (area > 500 && aspectRatio >= 0.9 && aspectRatio <= 1.1) {  // Adjust the parameters as needed
			diceContours.push_back(boundingRect);
		}
	}

	int numDice = diceContours.size();
	std::cout << "Number of dice: " << numDice << std::endl;
	// Count the number of dice
	//int numDice = 0;
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

	vector<int> result = { dice1, (int)(keypoints.size() - dice1)};

	if (validResult(result)) {
		callback(result);
	}
	
	Mat binaryImageWithKeypoints;
	Mat erodedImageWithKeypoints;
	Mat dilatedImageWithKeypoints;
	for (auto& keypoint : keypoints) {
		keypoint.size *= 1.2; // Adjust the size multiplier as needed
	}
	drawKeypoints(frame, keypoints, imageWithKeypoints, Scalar(255, 0, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(binaryImage, keypoints, binaryImageWithKeypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(morphedImage, keypoints, erodedImageWithKeypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(dilatedImage, keypoints, dilatedImageWithKeypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	//Show the image with keypoints in a window
	//namedWindow("Die Image", WINDOW_NORMAL);
	//imshow("Die Image", imageWithKeypoints);
	//imshow("binary Image", binaryImageWithKeypoints);
	//imshow("inverted image", invertedImage);
	//imshow("morphed Image", erodedImageWithKeypoints);
	//imshow("dilated Image", dilatedImageWithKeypoints);

	cv::Mat resultImage = imageWithKeypoints.clone();
	for (const auto& rect : diceContours) {
		cv::rectangle(resultImage, rect, cv::Scalar(0, 255, 0), 2);
	}

	imshow("Resulting image", resultImage);

}

void DiceDetection::startDetectionWrapper(void (*callback)(const std::vector<int>&)) {
	if (isRunning) {
		std::cout << "already running" << std::endl;
		return;
	}

	auto savedCallback = callback;

	isRunning = true;
	while (isRunning)
	{
		startDetection(savedCallback);
		//this_thread::sleep_for(std::chrono::milliseconds(100));

		//NEEDED TO SHOW IMAGES ON SEPARATE THREAD, REMOVE ON FINAL BUILD
		cv::waitKey(0);
	}
	
}

void DiceDetection::stop() {
	isRunning = false;
	destroyAllWindows();
	cap.release();
}

float calculateDistance(const KeyPoint& p1, const KeyPoint& p2) {
	return sqrt(pow(p1.pt.x - p2.pt.x, 2) + pow(p1.pt.y - p2.pt.y, 2));
}

bool validResult(vector<int> result) {
	if (result.size() == 2) {
		if (result.at(0) > 0 && result.at(0) < 7) {
			if (result.at(1) > 0 && result.at(1) < 7) {
				return true;
			}
		}
	}
	return false;
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


