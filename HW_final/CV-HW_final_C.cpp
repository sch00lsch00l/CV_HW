#define _CRT_SECURE_NO_DEPRECATE
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <vector>
#include <iomanip>
#include <time.h>

using namespace std;
using namespace cv;

void thin(const Mat &src, Mat &dst, const int iterations)
{
	const int height = src.rows - 1;
	const int width = src.cols - 1;
	if (src.data != dst.data)
	{
		src.copyTo(dst);
	}


	int n = 0, i = 0, j = 0;
	Mat tmpImg;
	uchar *pU, *pC, *pD;
	bool isFinished = false;

	for (n = 0; n < iterations; n++)
	{
		dst.copyTo(tmpImg);
		isFinished = false;  
		//scna
		for (i = 1; i < height; i++)
		{
			pU = tmpImg.ptr<uchar>(i - 1);
			pC = tmpImg.ptr<uchar>(i);
			pD = tmpImg.ptr<uchar>(i + 1);
			for (int j = 1; j < width; j++)
			{
				if (pC[j] > 0)
				{
					int ap = 0;
					int p2 = (pU[j] > 0);
					int p3 = (pU[j + 1] > 0);
					if (p2 == 0 && p3 == 1)
					{
						ap++;
					}
					int p4 = (pC[j + 1] > 0);
					if (p3 == 0 && p4 == 1)
					{
						ap++;
					}
					int p5 = (pD[j + 1] > 0);
					if (p4 == 0 && p5 == 1)
					{
						ap++;
					}
					int p6 = (pD[j] > 0);
					if (p5 == 0 && p6 == 1)
					{
						ap++;
					}
					int p7 = (pD[j - 1] > 0);
					if (p6 == 0 && p7 == 1)
					{
						ap++;
					}
					int p8 = (pC[j - 1] > 0);
					if (p7 == 0 && p8 == 1)
					{
						ap++;
					}
					int p9 = (pU[j - 1] > 0);
					if (p8 == 0 && p9 == 1)
					{
						ap++;
					}
					if (p9 == 0 && p2 == 1)
					{
						ap++;
					}
					if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) > 1 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) < 7)
					{
						if (ap == 1)
						{
							if ((p2*p4*p6 == 0) && (p4*p6*p8 == 0))
							{
								dst.ptr<uchar>(i)[j] = 0;
								isFinished = true;
							}
						}
					}
				}

			} //scan end


			dst.copyTo(tmpImg);
			//scan 2
			for (i = 1; i < height; i++) 
			{
				pU = tmpImg.ptr<uchar>(i - 1);
				pC = tmpImg.ptr<uchar>(i);
				pD = tmpImg.ptr<uchar>(i + 1);
				for (int j = 1; j < width; j++)
				{
					if (pC[j] > 0)
					{
						int ap = 0;
						int p2 = (pU[j] > 0);
						int p3 = (pU[j + 1] > 0);
						if (p2 == 0 && p3 == 1)
						{
							ap++;
						}
						int p4 = (pC[j + 1] > 0);
						if (p3 == 0 && p4 == 1)
						{
							ap++;
						}
						int p5 = (pD[j + 1] > 0);
						if (p4 == 0 && p5 == 1)
						{
							ap++;
						}
						int p6 = (pD[j] > 0);
						if (p5 == 0 && p6 == 1)
						{
							ap++;
						}
						int p7 = (pD[j - 1] > 0);
						if (p6 == 0 && p7 == 1)
						{
							ap++;
						}
						int p8 = (pC[j - 1] > 0);
						if (p7 == 0 && p8 == 1)
						{
							ap++;
						}
						int p9 = (pU[j - 1] > 0);
						if (p8 == 0 && p9 == 1)
						{
							ap++;
						}
						if (p9 == 0 && p2 == 1)
						{
							ap++;
						}
						if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) > 1 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) < 7)
						{
							if (ap == 1)
							{
								if ((p2*p4*p8 == 0) && (p2*p6*p8 == 0))
								{
									dst.ptr<uchar>(i)[j] = 0;
									isFinished = true;
								}
            
							}
						}
					} //scan2 end
				}
			} 
			if (isFinished == false)
			{
				break;
			}
		}

	}
}


void thinImage(Mat & srcImg) {
	vector<Point> deleteList;
	int neighbourhood[9];
	int nl = srcImg.rows;
	int nc = srcImg.cols;
	bool inOddIterations = true;
	while (true) {
		for (int j = 1; j < (nl - 1); j++) {
			uchar* data_last = srcImg.ptr<uchar>(j - 1);
			uchar* data = srcImg.ptr<uchar>(j);
			uchar* data_next = srcImg.ptr<uchar>(j + 1);
			for (int i = 1; i < (nc - 1); i++) {
				if (data[i] == 255) {
					int whitePointCount = 0;
					neighbourhood[0] = 1;
					if (data_last[i] == 255) neighbourhood[1] = 1;
					else  neighbourhood[1] = 0;
					if (data_last[i + 1] == 255) neighbourhood[2] = 1;
					else  neighbourhood[2] = 0;
					if (data[i + 1] == 255) neighbourhood[3] = 1;
					else  neighbourhood[3] = 0;
					if (data_next[i + 1] == 255) neighbourhood[4] = 1;
					else  neighbourhood[4] = 0;
					if (data_next[i] == 255) neighbourhood[5] = 1;
					else  neighbourhood[5] = 0;
					if (data_next[i - 1] == 255) neighbourhood[6] = 1;
					else  neighbourhood[6] = 0;
					if (data[i - 1] == 255) neighbourhood[7] = 1;
					else  neighbourhood[7] = 0;
					if (data_last[i - 1] == 255) neighbourhood[8] = 1;
					else  neighbourhood[8] = 0;
					for (int k = 1; k < 9; k++) {
						whitePointCount += neighbourhood[k];
					}
					if ((whitePointCount >= 2) && (whitePointCount <= 6)) {
						int ap = 0;
						if ((neighbourhood[1] == 0) && (neighbourhood[2] == 1)) ap++;
						if ((neighbourhood[2] == 0) && (neighbourhood[3] == 1)) ap++;
						if ((neighbourhood[3] == 0) && (neighbourhood[4] == 1)) ap++;
						if ((neighbourhood[4] == 0) && (neighbourhood[5] == 1)) ap++;
						if ((neighbourhood[5] == 0) && (neighbourhood[6] == 1)) ap++;
						if ((neighbourhood[6] == 0) && (neighbourhood[7] == 1)) ap++;
						if ((neighbourhood[7] == 0) && (neighbourhood[8] == 1)) ap++;
						if ((neighbourhood[8] == 0) && (neighbourhood[1] == 1)) ap++;
						if (ap == 1) {
							if (inOddIterations && (neighbourhood[3] * neighbourhood[5] * neighbourhood[7] == 0)
								&& (neighbourhood[1] * neighbourhood[3] * neighbourhood[5] == 0)) {
								deleteList.push_back(Point(i, j));
							}
							else if (!inOddIterations && (neighbourhood[1] * neighbourhood[5] * neighbourhood[7] == 0)
								&& (neighbourhood[1] * neighbourhood[3] * neighbourhood[7] == 0)) {
								deleteList.push_back(Point(i, j));
							}
						}
					}
				}
			}
		}
		if (deleteList.size() == 0)
			break;
		for (size_t i = 0; i < deleteList.size(); i++) {
			Point tem;
			tem = deleteList[i];
			uchar* data = srcImg.ptr<uchar>(tem.y);
			data[tem.x] = 0;
		}
		deleteList.clear();

		inOddIterations = !inOddIterations;
	}
}

int main()
{
	while (1)
	{
		string input;
		cout << "Please insert the video" << endl;
		cin >> input;

		VideoCapture video(input);
		if (!video.isOpened()) {
			return -1;
		}
		Size videoSize = Size((int)video.get(CV_CAP_PROP_FRAME_WIDTH), (int)video.get(CV_CAP_PROP_FRAME_HEIGHT));
		namedWindow(input, CV_WINDOW_AUTOSIZE);

		VideoWriter writer;
		writer.open("output.avi", CV_FOURCC('X', 'V', 'I', 'D'), 30, Size(640, 480));

		Mat videoFrame;
		int frame_number;
		vector<Vec4i> lines;
		vector< Vec2f > lines2;

		double time_spent_avg = 0.0;
		int count = 0;
		clock_t begin;
		clock_t end;

		char Data[40];
		char OriFile[] = "data.txt";    //Input Image name
		FILE *File;
		File = fopen(OriFile, "w");

		double fps = video.get(CV_CAP_PROP_FPS);
		cout << "Frames per second using video.get(CV_CAP_PROP_FPS) : " << fps << endl;
		while (true) {
			video >> videoFrame;

			if (videoFrame.empty())
			{
				break;
			}


			frame_number = video.get(CV_CAP_PROP_POS_FRAMES); // retrieves the current frame number
			count = count + 1;

			Mat hough_result = Mat::zeros(videoFrame.rows, videoFrame.cols, CV_8UC3);
			begin = clock();
			//----------------RGB to Gray----------------
			Mat frame_binary;
			cvtColor(videoFrame, frame_binary, cv::COLOR_RGB2GRAY);
			//imshow("Gray", frame_binary);
			//----------------threshold----------------
			threshold(frame_binary, frame_binary, 245.0, 255.0, THRESH_BINARY);
			//imshow("Thresholding", frame_binary);
			//----------------dilation_1----------------
			Mat Struct = getStructuringElement(MORPH_CROSS, Size(3, 3));
			dilate(frame_binary, frame_binary, Mat());
			dilate(frame_binary, frame_binary, Mat());
			//----------------closing_1----------------
			morphologyEx(frame_binary, frame_binary, MORPH_CLOSE, Mat());
			//----------------dilation_2----------------
			dilate(frame_binary, frame_binary, Mat());
			//----------------closing_2----------------
			morphologyEx(frame_binary, frame_binary, MORPH_CLOSE, Mat());
			//----------------dilation_3----------------
			dilate(frame_binary, frame_binary, Mat());
			//----------------closing_3----------------
			morphologyEx(frame_binary, frame_binary, MORPH_CLOSE, Mat());
			//----------------dilation_4----------------
			dilate(frame_binary, frame_binary, Mat());
			//----------------closing_4----------------
			morphologyEx(frame_binary, frame_binary, MORPH_CLOSE, Mat());
			//imshow("closing_4", frame_binary);
			//----------------erode_1----------------
			erode(frame_binary, frame_binary, Mat());
			imshow("填滿", frame_binary);
			////----------------thinning----------------
			thinImage(frame_binary);
			//imshow("thin", frame_binary);
			//----------------dilation_5----------------
			dilate(frame_binary, frame_binary, Struct, Point(-1, -1), 2);
			imshow("thin+dilation_5", frame_binary);
			////----------------openning----------------
			Mat hline = getStructuringElement(MORPH_RECT, Size(videoFrame.cols / 16, 1), Point(-1, -1)); //remove stright line
			Mat vline = getStructuringElement(MORPH_RECT, Size(1, videoFrame.rows / 30), Point(-1, -1)); //remove horezontal line
			morphologyEx(frame_binary, frame_binary, CV_MOP_OPEN, vline); //openning
			//----------------dilation_5----------------
			dilate(frame_binary, frame_binary, Mat());
			imshow("濾橫條", frame_binary);
			
			//----------------find & draw the line----------------//
			HoughLinesP(frame_binary, lines, 1, CV_PI / 180, 150, 100, 60);

			vector<float>  m, delta_y;
			float rho[20], theta[20], rho_all[20], theta_all[20], delta_x[20], delta_x_all[20];
			//printf("線條數：%d\n",lines.size());
			if (lines.size()>11)
			{
				for (size_t i = 0; i < lines.size(); i++)
				{
					Vec4i l = lines[i];
					line(hough_result, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
					sprintf(Data, " %d %d %d %d %d \n", frame_number, (l[0], l[1], l[2], l[3]));
					fwrite(Data, 1, sizeof(Data), File);
				}
			}
			else
			{		
				for (size_t i = 0; i < lines.size(); i++)
				{
				
					// Get points from the vector
					cv::Point2f p1(lines[i][0], lines[i][1]);
					cv::Point2f p2(lines[i][2], lines[i][3]);

					rho_all[i] = (abs(p2.x*p1.y - p2.y*p1.x) / cv::norm(p2 - p1));
					theta_all[i] = (-atan2((p2.x - p1.x), (p2.y - p1.y)));
					if (theta_all[i] < -(CV_PI - 2))
					{
						theta_all[i] = (-atan2((p2.x - p1.x), (p2.y - p1.y))) + CV_PI;
					}

					if (frame_number == 1 && abs(theta_all[i]) < 0.2)
					{

						//Compute 'rho' and 'theta'
						rho[i] = rho_all[i];
						theta[i] = theta_all[i];

						//cout << rho[i] << endl;

						float a = cos(theta[i]), b = sin(theta[i]);
						float x0 = a * rho[i], y0 = b * rho[i];
						cv::Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
						cv::Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
						delta_x[1] = (abs(pt2.x + pt1.x)) / 2;

						cv::line(hough_result, pt1, pt2, cv::Scalar(255, 255, 0), 1, CV_AA);

						sprintf(Data, " %d %d %d %d %d \n", frame_number, pt1.x, pt1.y, pt2.x, pt2.y);
						fwrite(Data, 1, sizeof(Data), File);
					}
					else if (abs(theta_all[i] - theta[1]) <= 0.3 && abs(rho[1] - rho_all[i]) < 25.0)
					{
						//cout << "差值" << abs(theta_all[i] - theta[2]) << endl;
						float a = cos(theta_all[i]), b = sin(theta_all[i]);
						float x0 = a * rho_all[i], y0 = b * rho_all[i];

						rho[1] = rho_all[i];
						theta[1] = theta_all[i]; 

						cv::Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
						cv::Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));

						cv::line(hough_result, pt1, pt2, cv::Scalar(255, 255, 0), 1, CV_AA);

						sprintf(Data, " %d %d %d %d %d \n", frame_number, pt1.x, pt1.y, pt2.x, pt2.y);
						fwrite(Data, 1, sizeof(Data), File);
					}
					else {
						//cout << "error" << abs(theta_all[i] - theta[0]) << endl;
					}
				}
			}

			dilate(hough_result, hough_result, Mat(), Point(-1, -1), 2);
			erode(hough_result, hough_result, Mat(), Point(-1, -1), 2);
			imshow("houghline", hough_result);
			//----------------Result----------------//
			videoFrame = videoFrame - hough_result;
			imshow(input, videoFrame);
			writer << videoFrame;

			end = clock();
			double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
			time_spent_avg = time_spent + time_spent_avg;
			//printf("each frame execution time:%f seconds \n", time_spent);
			//waitKey(0);
			
			if (waitKey(33) == 27)
			{
				break;
			}
		}
		time_spent_avg = time_spent_avg / count;
		printf("Average execution time: %f seconds \n \n", time_spent_avg);
		cout << "----------------------------------------------" << endl;
		cvWaitKey(0);
		fclose(File);
	}
	return 0;
}
