#include "MIPMAP.h"

//创建图片的mipmap数组
void CreateMIPMAP(const cv::Mat &img, int curW, int curH, std::vector<cv::Mat> &imageArray) {
	while (curW > 0 && curH > 0) {
		imageArray.emplace_back(CreateMini(img, curW, curH));
		//std::cout << i++ << std::endl;
		curW /= 2;
		curH /= 2;
	}
}

//创建单张texture
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

//利用mipmap创建texture
cv::Mat CreateTexture_mipmap(const std::vector<cv::Mat> &imageArray, const int width, const int height) {
	cv::Mat res(height, width, CV_32FC3, cv::Scalar(0, 0, 0));
	float DD = std::max(width, height);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			float uv[2] = { (float)(i + 1) / height, (float)(j + 1) / width };
			res.at<cv::Vec3f>(i, j) = LookUp(uv, imageArray, DD);
		}
	}
	return res;
}

//生成MIPMAP的ppm文件
void CreateMIPMAP_PPM(const std::vector<cv::Mat> &imageArray) {
	for (int i = 0; i < imageArray.size(); i++) {
		std::string str = "texture" + std::to_string(i) + ".ppm";
		CreatePPM(str, imageArray[i]);
	}
}
// 生成ppm文件
void CreatePPM(const std::string name, const cv::Mat &tmpImage) {
	std::ofstream outfile;
	outfile.open(name);
	outfile << "P3" << std::endl
		<< tmpImage.cols << " " << tmpImage.rows << std::endl
		<< 255 << std::endl;

	for (int i = 0; i < tmpImage.rows; i++) {
		for (int j = 0; j < tmpImage.cols; j++) {

			//cv里存颜色是(B,G,R),转成ppm需要调整顺序
			outfile << round(tmpImage.at<cv::Vec3f>(i, j)[2]) << " "
				<< round(tmpImage.at<cv::Vec3f>(i, j)[1]) << " "
				<< round(tmpImage.at<cv::Vec3f>(i, j)[0])
				<< std::endl;
		}
	}
	outfile.close();
}

//层级线性过滤
cv::Vec3f LookUp(float uv[], const std::vector<cv::Mat> &imageArray, const float DD) {
	cv::Vec3f res;
	//二维空间，uv梯度均匀,直接用分辨率求
	float level = std::max(imageArray.size() - 1 - log2(DD), 0.0f);

	cv::Vec3f up = LookUp(uv, imageArray[ceil(level)]);
	cv::Vec3f down = LookUp(uv, imageArray[floor(level)]);
	float t = level - floor(level);
	return (1 - t)*down + t * up;
}

//双线性过滤
cv::Vec3f LookUp(float uv[], const cv::Mat &img) {
	float *ptr = uv;
	float row = (*ptr) * (float)img.rows;
	ptr++;
	float col = (*ptr) * (float)img.cols;

	int right = round(col);
	int left = std::max(right - 1, 0);
	int down = round(row);
	int up = std::max(down - 1, 0);
	right = std::min(img.cols - 1, right);
	down = std::min(img.rows - 1, down);

	//第一层插值：X-axis
	float t = (col - left - 0.5);

	//auto x = typename(img.at());
	cv::Vec3f y1 = Interpolation(img.at<cv::Vec3f>(up, left), img.at<cv::Vec3f>(up, right), t); //up插值
	cv::Vec3f y2 = Interpolation(img.at<cv::Vec3f>(down, left), img.at<cv::Vec3f>(down, right), t);	//down插值

	//第二层插值：Y-axis
	t = (row - up - 0.5);
	return Interpolation(y1, y2, t);

};

//自定义插值
template <class T>
cv::Vec3f Interpolation(const T &x1, const T &x2, const float t) {
	cv::Vec3b res;
	res[0] = (1.0 - t)*(float)x1[0] + t * (float)x2[0];
	res[1] = (1.0 - t)*(float)x1[1] + t * (float)x2[1];
	res[2] = (1.0 - t)*(float)x1[2] + t * (float)x2[2];
	//std::cout << res << std::endl;

	return res;
};
