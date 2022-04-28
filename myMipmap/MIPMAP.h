#pragma once
#include <opencv2/opencv.hpp>

class MIPMAP
{
public:
	MIPMAP(cv::Mat img) { base_data(img); };

	void CreateMIPMAP() {

	};
private:
	const cv::Mat base_data;
};

