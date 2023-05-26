#pragma once
class DiceDetection
{
public:
	DiceDetection();
	void startDetection(int* result);
	void startDetectionWrapper(int* result);
	void stop();
};

