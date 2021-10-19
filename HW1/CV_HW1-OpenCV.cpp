#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include "stdlib.h"
#include "math.h"
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp> 

using namespace std;
using namespace cv;

int main() {
	char open_file[30];
	char write_file[30];

	cout << "請輸入檔案名稱(要打副檔名)：" << endl;
	cin >> open_file;

	if (open_file == NULL) {
		puts("Loading File Error!");
		system("pause");
		exit(0);
	}

	char *input_image = open_file;

	cv::Mat INPUT_IM = cv::imread(input_image, 1);;  
	cv::Mat transpose_IM = cv::imread(input_image, 1);;  
	cv::Mat change_IM = imread(input_image,1);  
	cv::Mat change_color_IM = imread(input_image, 1);  
	transpose(INPUT_IM, transpose_IM);
	flip(transpose_IM, change_IM, 1);
	cvtColor(change_IM, change_color_IM, COLOR_BGR2RGB);
	

	cout << "請輸入輸出之檔案名稱(要打副檔名)：" << endl;
	cin >> write_file;
	if (write_file == NULL) {
		puts("Loading File Error!");
		system("pause");
		exit(0);
	}
	cv::imshow("右旋90度+換RGB通道", change_color_IM);
	cv::imwrite(write_file, change_color_IM);

	cvWaitKey();
	return 0;

}
