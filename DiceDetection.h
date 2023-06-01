#pragma once
#include <vector>
class DiceDetection
{
public:
	DiceDetection();
	void startDetection(void (*callback)(const std::vector<int>&));
	void startDetectionWrapper(void (*callback)(const std::vector<int>&));
	void stop();
};

