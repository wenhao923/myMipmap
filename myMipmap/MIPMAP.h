#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <fstream>

#include <vector>
#include <string>

template <class T>
cv::Vec3f Interpolation(const T &, const T &, const float);
cv::Vec3f LookUp(float[], const cv::Mat &);
cv::Vec3f LookUp(float[], const std::vector<cv::Mat> &, const float);
void CreatePPM(const std::string, const cv::Mat &);
cv::Mat CreateMini(const cv::Mat &, const int, const int);
void CreateMIPMAP_PPM(const std::vector<cv::Mat> &);
void CreateMIPMAP(const cv::Mat &, int, int, std::vector<cv::Mat> &);
cv::Mat CreateTexture_mipmap(const std::vector<cv::Mat> &, const int, const int);


