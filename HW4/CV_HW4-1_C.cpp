#define _CRT_SECURE_NO_DEPRECATE
#include <cstdio>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>
#include <opencv2\tracking\tracker.hpp>



using namespace std;
using namespace cv;

int main()
{
	Rect2d roi;
	Mat frame;
	Ptr<TrackerKCF> tracker = TrackerKCF::create();  //1可以 2直接去背景
	//Ptr<TrackerMIL> tracker = TrackerMIL::create();  //2一半跑掉
	//Ptr<TrackerTLD> tracker= TrackerTLD::create();   //3 正面大致都抓得到 背面悲劇
	//Ptr<TrackerMedianFlow> tracker = TrackerMedianFlow::create();  //不行 後面會跑掉
	//Ptr<TrackerBoosting> tracker= TrackerBoosting::create();  //後面偶爾變成抓手 或是跑掉
	//Ptr<TrackerMOSSE> tracker = TrackerMOSSE::create();


	string video = "1.avi"; 

	VideoWriter writer;
	writer.open("output1.avi", CV_FOURCC('X', 'V', 'I', 'D'), 30, Size(640, 480));

	VideoCapture cap(video);
	if (!cap.isOpened())
	{
		return 0;
	}
	cout << "按 c(小寫模式) 跳下一禎影像" << endl;
	cout << "按 q(小寫模式) 框ROI" << endl;
	cout << "按空白鍵開始tracking" << endl;

	cap >> frame;
	while (1)
	{
		char key = waitKey(1);
		if (key == 'c') 
		{
			cap >> frame;
		}
		if (key == 'q')
		{
			break;
		}
		imshow("first", frame);
	}

	cv::destroyWindow("first");

	roi = selectROI("tracker", frame);

	if (roi.width == 0 || roi.height == 0)
		return 0;

	tracker->init(frame, roi);

	// perform the tracking process
	printf("tracking開始~\n");
	for (;; )
	{
		// get frame from the video
		cap >> frame;

		// stop the program if no more images
		if (frame.rows == 0 || frame.cols == 0) {
			cv::destroyWindow("tracker");
			break;
		}

		// update the tracking result
		tracker->update(frame, roi);

		// draw the tracked object
		rectangle(frame, roi, Scalar(0, 0, 255), 2, 1);

		// show image with the tracked object
		imshow("tracker", frame);
		writer << frame;
		
		if (char(waitKey(1)) == 'q') {
			cv::destroyWindow("tracker");
			break;
		}
	}
	return 0;
}
