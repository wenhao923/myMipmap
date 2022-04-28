
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <fstream>

cv::Vec3f Interpolation(const cv::Vec3b &x1, const cv::Vec3b &x2, const float t);
cv::Vec3f LookUp(float uv[], const cv::Mat &img);
void CreatePPM(const char* name, const cv::Mat &tmpImage, const int newWidth, const int newHeight);
cv::Mat CreateMini(const cv::Mat &img, const int newWidth, const int newHeight);

int main()
{
	cv::Mat img = cv::imread("C:\\Users\\zhuwenhao\\Desktop\\pbrt\\pbrt-v3\\scenes\\butterfly.png", cv::IMREAD_UNCHANGED);

	const int width = img.cols;	//宽度
	const int height = img.rows;	//高度

	//创建小的纹理
	cv::Mat tmpImage = CreateMini(img, width / 2, height / 2);

	//生成图片文件
	CreatePPM("texture.ppm", tmpImage, tmpImage.cols, tmpImage.rows);

	//imshow("new", tmpImage);
	//cv::waitKey(0);

	return 0;
}

cv::Mat CreateMini(const cv::Mat &img, const int newWidth, const int newHeight) {
	cv::Mat res(newHeight, newWidth, CV_32FC3, cv::Scalar(0, 0, 0));

	for (int i = 0; i < newHeight; i++) {
		for (int j = 0; j < newWidth; j++) {
			float uv[2] = { (float)(i + 1) / newHeight, (float)(j + 1) / newWidth };
			res.at<cv::Vec3f>(i, j) = LookUp(uv, img);
		}
	}
	return res;
}

void CreatePPM(const char* name, const cv::Mat &tmpImage, const int newWidth, const int newHeight) {
	// 生成ppm文件
	std::ofstream outfile;
	outfile.open(name);
	outfile << "P3" << std::endl
		<< newWidth << " " << newHeight << std::endl
		<< 255 << std::endl;

	for (int i = 0; i < newHeight; i++) {
		for (int j = 0; j < newWidth; j++) {

			//cv里存颜色是(B,G,R),转成ppm需要调整顺序
			outfile << tmpImage.at<cv::Vec3f>(i, j)[2] << " "
				<< tmpImage.at<cv::Vec3f>(i, j)[1] << " "
				<< tmpImage.at<cv::Vec3f>(i, j)[0]
				<< std::endl;
		}
	}
	outfile.close();
}

cv::Vec3f LookUp(float uv[], const cv::Mat &img) {
	float *ptr = uv;
	float row = (*ptr) * (float)img.rows;
	ptr++;
	float col = (*ptr) * (float)img.cols;
	
	int right = round(col);
	int left = std::max(right - 1, 0);
	int down = round(row);
	int up = std::max(down - 1, 0);
	right = std::min(img.cols-1, right);
	down = std::min(img.rows-1, down);

	//第一层插值：X-axis
	float t = (col - left - 0.5);
	cv::Vec3f y1 = Interpolation(img.at<cv::Vec3b>(up, left), img.at<cv::Vec3b>(up, right), t); //up插值
	cv::Vec3f y2 = Interpolation(img.at<cv::Vec3b>(down, left), img.at<cv::Vec3b>(down, right), t);	//down插值

	//第二层插值：Y-axis
	t = (row - up - 0.5);
	return Interpolation(y1, y2, t);

};

cv::Vec3f Interpolation(const cv::Vec3b &x1, const cv::Vec3b &x2, const float t) {
	cv::Vec3b res;
	res[0] = (1.0 - t)*(float)x1[0] + t * (float)x2[0];
	res[1] = (1.0 - t)*(float)x1[1] + t * (float)x2[1];
	res[2] = (1.0 - t)*(float)x1[2] + t * (float)x2[2];
	//std::cout << res << std::endl;

	return res;
};
