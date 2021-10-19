#define _CRT_SECURE_NO_DEPRECATE
#include <opencv2/opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include "stdlib.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main()
{
	cv::Mat src_img, img_bool, labels, stats, centroids, img_color, img_gray;
	char open_file[30];
	char write_file[30];
	long time = clock();

	cout << "請輸入檔案名稱(要打副檔名)：" << endl;
	cin >> open_file;
	char *input_image = open_file;
	if ((src_img = cv::imread(input_image, 0)).empty())
	{
		cout << "load image error！";
		return -1;
	}
	cv::threshold(src_img, img_bool, 128, 255, cv::THRESH_OTSU);
	int nccomps = cv::connectedComponentsWithStats(
		img_bool,
		labels,     
		stats, 
		centroids
	);

	//Coloring if the area>1000
	vector<cv::Vec3b> colors(nccomps);
	colors[0] = cv::Vec3b(0, 0, 0); 
	for (int i = 1; i < nccomps; i++) {
		colors[i] = cv::Vec3b(rand() % 256, rand() % 256, rand() % 256);
	}

	//Coloring by label
	img_color = cv::Mat::zeros(src_img.size(), CV_8UC3);
	for (int y = 0; y < img_color.rows; y++)
	{
		for (int x = 0; x < img_color.cols; x++)
		{
			int label = labels.at<int>(y, x);
			CV_Assert(0 <= label && label <= nccomps); 
			img_color.at<cv::Vec3b>(y, x) = colors[label];
		}
	}
	
	for (int i = 1; i < nccomps; ++i) {                   //bounding box
		if (stats.at<int>(i, cv::CC_STAT_AREA) > 1000) {
			int *param = stats.ptr<int>(i);

			int x = param[cv::ConnectedComponentsTypes::CC_STAT_LEFT];
			int y = param[cv::ConnectedComponentsTypes::CC_STAT_TOP];
			int height = param[cv::ConnectedComponentsTypes::CC_STAT_HEIGHT];
			int width = param[cv::ConnectedComponentsTypes::CC_STAT_WIDTH];

			cv::rectangle(img_color, cv::Rect(x, y, width, height), cv::Scalar(0, 0, 255), 2);
		}
	}
	// drawing the center of gravity
	for (int i = 1; i < nccomps; ++i) {
		if (stats.at<int>(i, cv::CC_STAT_AREA) > 1000) {
			double *param = centroids.ptr<double>(i);
			int x = static_cast<int>(param[0]);
			int y = static_cast<int>(param[1]);
			cv::circle(img_color, cv::Point(x, y), 3, cv::Scalar(0, 0, 255), -1);
		}
	}

	//caculated the area and marked the number
	for (int i = 1; i < nccomps; ++i) {
		if (stats.at<int>(i, cv::CC_STAT_AREA) > 1000) {
			int *param = stats.ptr<int>(i);
			std::cout << "area " << i << " = " << param[cv::ConnectedComponentsTypes::CC_STAT_AREA] << std::endl;

			int x = param[cv::ConnectedComponentsTypes::CC_STAT_LEFT];
			int y = param[cv::ConnectedComponentsTypes::CC_STAT_TOP];
			std::stringstream num;
			num << i;
			cv::putText(img_color, num.str(), cv::Point(x + 5, y + 20), cv::FONT_HERSHEY_COMPLEX, 0.7, cv::Scalar(0, 255, 255), 2);
		}
	}

	printf("程式總花費時間：%dms\n\n", clock() - time);

	cout << "請輸入輸出之檔案名稱(要打副檔名)：" << endl;
	cin >> write_file;
	if (write_file == NULL) {
		puts("Loading File Error!");
		system("pause");
		exit(0);
	}

	cv::imwrite(write_file, img_color);
	cv::imshow("結果圖", img_color);
	cv::waitKey();

	return 0;
}
