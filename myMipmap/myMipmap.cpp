
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include "MIPMAP.h"


int main()
{
	cv::Mat img = cv::imread("C:\\Users\\zhuwenhao\\Desktop\\pbrt\\pbrt-v3\\scenes\\butterfly.png", cv::IMREAD_UNCHANGED);
	const int width = img.cols;	//宽度
	const int height = img.rows;	//高度
	img.convertTo(img, CV_32FC3);

	//用来保存mipmap的数组
	std::vector<cv::Mat> imageArray;
	CreateMIPMAP(img, width, height, imageArray);
	CreateMIPMAP_PPM(imageArray);

	cv::Mat res = CreateTexture_mipmap(imageArray, 512, 325);

	CreatePPM("arbitrary.ppm", res);
//	imshow("new", res);
//	cv::waitKey(0);

	return 0;
}