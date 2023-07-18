#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
#include <vector>


using namespace std;
using namespace cv;

#ifdef _DEBUG
// the debug version that corresponds to the opencv_world420d.dll:
#pragma comment(lib, "opencv_world480d.lib")
#else
// the release version that corresponds to the opencv_world420.dll:
#pragma comment(lib, "opencv_world480.lib")
#endif

//exercize 3
void showPicture() {
	string path = R"(D:\Ditsa\Pictures\chihuahua-puppy.jpeg)";
	Mat image = cv::imread(path);

	cv::resize(image, image, cv::Size(500, 350));
	cv::imshow("MyImage", image);

	int key = cv::waitKey();

	while (key != 27) {

		double rows = image.rows, cols = image.cols;
		key = cv::waitKey();

		if (key == 's') {
			rows /= 1.1;
			cols /= 1.1;
		}
		if (key == 'l') {
			rows *= 1.1;
			cols *= 1.1;
		}

		cv::resize(image, image, cv::Size(cols, rows));
		cv::imshow("MyImage", image);
	}
}

void saveFloat32Image(const cv::Mat& image, const std::string& path) {
    std::ofstream file(path, std::ios::binary);

    if (!file.is_open())
        throw "Failed to open file for writing";

    int32_t width = image.cols;
    int32_t height = image.rows;

    file.write((char*)(&width), sizeof width);
    file.write((char*)(&height), sizeof height);
	file.write(reinterpret_cast<const char*>(image.ptr()), sizeof(float) * image.total());

	file.flush();
    file.close();
}

Mat loadFloat32Image(const std::string& path) {
	ifstream file(path, ios::binary);

	if (!file.is_open())
		throw "Failed to open file for reading";

	int32_t width = 0;
	int32_t height = 0;

	file.read(reinterpret_cast<char*>(&width), sizeof(int32_t));
	file.read(reinterpret_cast<char*>(&height), sizeof(int32_t));

	vector<float> data(width * height);
	file.read(reinterpret_cast<char*>(data.data()), sizeof(float) * data.size());

	file.close();

	Mat image = Mat(height, width, CV_32FC1, data.data()).clone();
	return image;
}

void sumBGR(Mat image) {
	uint32_t sumR = 0, sumG = 0, sumB = 0;
	for (int row = 0; row < image.rows; row++) {
		for (int col = 0; col < image.cols; col++) {
			uchar* row_data = image.ptr(row);
			if ((col + 1) % 3 == 0)
				sumR += *(row_data + col);
			else
				if ((col + 1) % 3 == 1)
					sumB += *(row_data + col);
				else
					sumG += *(row_data + col);
		}
	}

	std::cout << sumR << std::endl;
	std::cout << sumB << std::endl;
	std::cout << sumG << std::endl;
}

void blurrImage(Mat image) {
	vector<Mat> bgr_channels;
	split(image, bgr_channels);
	GaussianBlur(bgr_channels[0], bgr_channels[1], Size(11, 11), 5);
	GaussianBlur(bgr_channels[2], bgr_channels[0], Size(11, 11), 5);

	Mat afterBlurringBR;
	cv::merge(bgr_channels, afterBlurringBR);
	imshow("colorImage", image);
	imshow("afterBlurringBR", afterBlurringBR);
	waitKey();
}

int main()
{
	//showPicture();
	Mat grayImage = cv::imread(R"(D:\Ditsa\Pictures\GrayImage.bmp)", cv::IMREAD_GRAYSCALE);
	Mat convertFloatGrayImage;
	grayImage.convertTo(convertFloatGrayImage, CV_32FC1, 1.0 / 255);

	if (grayImage.empty()) {
		throw "Failed to load image";
	}

	saveFloat32Image(convertFloatGrayImage, "float_image.bin");

	Mat loadedImage = loadFloat32Image("float_image.bin");

	Mat convertIntImage;
	loadedImage.convertTo(convertIntImage, CV_8UC1, 255);

	cv::imshow("Original Image", grayImage);
	cv::imshow("Loaded and Converted Image", convertIntImage);

	cv::waitKey(0);

	Mat colorImage = cv::imread(R"(D:\Ditsa\Pictures\yellow_flower.png)");
	sumBGR(colorImage);
	blurrImage(colorImage);

	return 0;
}

