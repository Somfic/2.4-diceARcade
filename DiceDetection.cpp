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

enum ResultCode {
	Success = 0,
	DiceTooNearby = 1,
	TooManyDice = 2,
	TooLittleDice = 3,
	NotCalibrated = 4,
	InconsistentDiceCount = 5
};

bool isRunning = false;
bool isCalibrated = false;
VideoCapture cap(0);
float calibratedDistance = 0;
ResultCode validResult(vector<int>);
void erodeImage(Mat *originalImage, Mat *newImage);
void dilateImage(Mat *originalImage, Mat *newImage);
float calculateDistance(const KeyPoint& p1, const KeyPoint& p2);
bool checkAllSame(const std::queue<std::vector<int>>& queue);

queue<vector<int>> diceQueue;
const int maxQueueSize = 4;
const int totalDice = 2;
const float distanceTreshold = 3;

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
	

	// Set up SimpleBlobDetector parameters
	SimpleBlobDetector::Params params;
	params.filterByArea = true;
	params.minArea = 50;
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

	int numDice = 0;
	// Count the number of dice
	//int numDice = 0;
	int dice1 = 0;
	int dice2 = 0;
	float smallestDistance = 0;
	if (keypoints.size() > 0)
	{
		numDice = 1;
		// Sort keypoints based on the x-coordinate
		sort(keypoints.begin(), keypoints.end(), [](const KeyPoint& a, const KeyPoint& b) {
			return a.pt.x < b.pt.x;
			});

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

		if (isCalibrated == true) {
			smallestDistance = calibratedDistance;
		}
		
		KeyPoint referencePoint = keypoints[smallestDistanceIndex];
		for (int i = 0; i < keypoints.size(); i++)
		{
			KeyPoint currentPoint = keypoints[i];
			float distance = calculateDistance(currentPoint, referencePoint);
			
			cout << "distance: " << distance << endl ;
			if (smallestDistance * distanceTreshold < distance) {
				numDice = 2;
				dice1++;
				continue;
			}
		}
	}
	if ((isCalibrated == false) && (numDice == 1) && (keypoints.size() == 6)) {
		calibratedDistance = smallestDistance;
		isCalibrated = true;
	}

	vector<int> result = { dice1, (int)(keypoints.size() - dice1)};
	int resultCode = validResult(result);
	callback({ dice1, (int)(keypoints.size() - dice1), resultCode});
	
	// Print the number of dice and the number of blobs
	if (dice1 > 6 || keypoints.size() - dice1 > 6) {
		cout << "Error, dice may be too nearby: " << keypoints.size() << endl;
	}
	else {
		cout << "Number of blobs: " << keypoints.size() << endl;
		cout << "dice 1: " << dice1 << endl;
		cout << "dice 2: " << keypoints.size() - dice1 << endl;
		cout << "calibratedDistance: " << calibratedDistance << endl;
	}
	
	// Draw the keypoints on the original image
	Mat imageWithKeypoints;
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
	

	/*cv::Mat result = frame.clone();
	cv::drawContours(result, diceContours, -1, cv::Scalar(0, 255, 0), 2);
	cv::imshow("Result", result);*/

	cv::Mat resultImage = imageWithKeypoints.clone();
	for (const auto& rect : diceContours) {
		cv::rectangle(resultImage, rect, cv::Scalar(0, 255, 0), 2);
	}
	//imshow("Die Image", imageWithKeypoints);
	//imshow("binary Image", binaryImageWithKeypoints);
	//imshow("inverted image", invertedImage);
	//imshow("eroded Image", erodedImageWithKeypoints);
	//imshow("dilated Image", dilatedImageWithKeypoints);
	//imshow("canny Image", cannyImage);
	//imshow("Resulting image", resultImage);

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
		this_thread::sleep_for(std::chrono::milliseconds(100));

		//NEEDED TO SHOW IMAGES ON SEPARATE THREAD, REMOVE ON FINAL BUILD
		//cv::waitKey(0);
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

ResultCode validResult(vector<int> result) {
	/*if (isCalibrated == false) {
		return false;
	}
	if (result.size() == totalDice) {
		for (int i = 0; i < result.size(); i++) {
			if (result.at(i) < 1 || result.at(i) > 6) {
				return false;
			}
		}
	}
	else {
		return false;
	}*/

	diceQueue.push(result); // Add the vector to the queue

	if (diceQueue.size() > maxQueueSize) {
		diceQueue.pop(); // Remove the oldest vector if the queue exceeds the maximum size
	}
	if (isCalibrated == false) {
		return NotCalibrated;
	}
	else if (!checkAllSame(diceQueue)) {
		return InconsistentDiceCount;
	}
	else if (result.size() > totalDice) {
		return TooManyDice;
	} 
	else if (result.size() < totalDice) {
		return TooLittleDice;
	}
	for (int i = 0; i < result.size(); i++) {
		if (result.at(i) < 1 || result.at(i) > 6) {
			return DiceTooNearby;
		}
	}
	return Success;
}

bool checkAllSame(const std::queue<std::vector<int>>& queue) {
	if (queue.empty() || queue.size() < maxQueueSize) {
		return false; // If the queue is empty or not full it means there are no or too little times dice detected
	}

	const std::vector<int>& reference = queue.front();
	std::queue<std::vector<int>> tempQueue = queue; // Create a temporary queue

	while (!tempQueue.empty()) {
		const std::vector<int>& current = tempQueue.front();

		// Check if the current vector is different from the reference vector
		if (current != reference) {
			return false;
		}

		tempQueue.pop(); // Remove the processed vector from the temporary queue
	}

	return true; // All vectors in the queue are the same
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


