#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <windows.h>
#include "BMP_Header.h"
#include "stdlib.h"
#include "math.h"
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#define NEW2D(H, W, TYPE) (TYPE **)new2d(H, W, sizeof(TYPE))

Bitmap_File_Header BMP_head;
Palette_RGB RGB_color[256];//256調色盤
Bitmap_File_HeaderInfo BMP_info;

using namespace std;

int label[13500] = { 0 };  //記錄label對應關係的容量

void show_bmp_header(Bitmap_File_Header BFH) {
	cout << "\n標頭檔資訊:" << endl;
	cout << "檔案大小：" << BFH.bfSize << endl;
	cout << "保留字_1：" << BFH.bfReserved1 << endl;
	cout << "保留字_2：" << BFH.bfReserved2 << endl;
	cout << "點陣圖資料開始之前的偏移量：" << BFH.bfOffBits << endl << endl;
	system("pause");
}

void show_bmp_headerinfo(Bitmap_File_HeaderInfo BFH_info) {
	cout << "\n標頭檔資訊：" << endl;
	cout << "Bitmap Info Header 的長度：" << BFH_info.biSize << endl;
	cout << "影像寬度：" << BFH_info.biWidth << endl;
	cout << "影像高度：" << BFH_info.biHeight << endl;
	cout << "點陣圖的位元圖層數：" << BFH_info.biPlanes << endl;
	cout << "像素位元數：" << BFH_info.biBitCount << endl;
	cout << "壓縮方式：" << BFH_info.biCompression << endl;
	cout << "點陣圖資料的大小：" << BFH_info.biSizeImage << endl;
	cout << "水平解析度：" << BFH_info.biXPelsPerMeter << endl;
	cout << "垂直解析度：" << BFH_info.biYPelsPerMeter << endl;
	cout << "點陣圖顏色數：" << BFH_info.biClrUsed << endl;
	cout << "重要的顏色數：" << BFH_info.biClrImportant << endl;
	system("pause");
}

void* new2d(int h, int w, int size)
{
	register int i;
	void **p;

	p = (void**)new char[h * sizeof(void*) + h * w*size];
	for (i = 0; i < h; i++)
	{
		p[i] = ((char *)(p + h)) + i * w*size;
	}

	return p;
}

bool determine_0_255(int &value)//判斷等於0或255
{
	if (value == 255 || value == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void setvalue(int &value, int &minimun)//設置配對值
{
	if (value != 0)
	{
		if (label[value] > minimun && label[value] != 255)
		{
			int buff_med_val = label[value];
			while (true)//將mapping表調整
			{
				if (label[buff_med_val] > minimun)
				{
					int mid = label[buff_med_val];
					label[buff_med_val] = minimun;
					buff_med_val = mid;
				}
				else
				{
					break;
				}
			}
			label[value] = minimun;
		}
		value = minimun;
	}
}

void compare_min(int &value, int &minimun)//比小
{
	if (value != 0 && value != 255)
	{
		if (minimun >= value)
		{
			minimun = value;
		}
	}
}

void compare_max(int &value, int &bigmun)//比小
{
	if (value != 0 && value != 255)
	{
		if (bigmun <= value)
		{
			bigmun = value;
		}
	}
}

//void draw_line(int &x0, int &y0, int &x1, int &y1)//畫線
//{
//
//	for (int i=x0; i <x1; i++)
//	{
//	
//	}
//}



int main() {
	char open_file[30];
	char write_file[30];
	Image_data *image_ori;//存原圖
	Image_data *image_change;//存更改後的圖
	//Image_data **image_change_med;//存更改後的圖
	int **image_change_med;//存更改後的圖
	int **image_change_med2;//存更改後的圖
	Image_data *image_change2;//存更改後的圖
	int width, height;
	int red, green, blue;
	int i, j;
	BYTE gray;
	int winsize = 3;
	int side = (winsize - 1) / 2;

	int label_num = 50;
	long time = clock();
	int value;



	cout << "請輸入檔案名稱(要打副檔名)：" << endl;
	cin >> open_file;
	if (open_file == NULL) {
		//puts("Loading File Error!");
		system("pause");
		exit(0);
	}

	FILE *input_file, *output_file;
	input_file = fopen(open_file, "rb");
	WORD bfType;
	fread(&bfType, 1, sizeof(WORD), input_file);


	fread(&BMP_head, 1, sizeof(Bitmap_File_Header), input_file);  //讀檔案標頭
	//show_bmp_header(BMP_head);
	fread(&BMP_info, 1, sizeof(Bitmap_File_HeaderInfo), input_file);
	//show_bmp_headerinfo(BMP_info); 

	width = BMP_info.biHeight; //將長寬資訊對調
	height = BMP_info.biWidth;
	image_ori = (Image_data*)malloc(width * height * sizeof(Image_data));  //設動態配置
	image_change = (Image_data*)malloc(width * height * sizeof(Image_data));
	image_change2 = (Image_data*)malloc((width + side * 2) * (height + side * 2) * sizeof(Image_data));
	image_change_med = NEW2D(height, width, int);
	image_change_med2 = NEW2D(height + side * 2, width + side * 2, int);

	fread(image_ori, sizeof(struct Image_data) * width, height, input_file);


	//for (unsigned int i = 0; i < BMP_info.biClrUsed; i++)  //讀RGB 調色盤
	//{
	//	fread((char *)&RGB_color[i].rgbBlue, 1, sizeof(BYTE), input_file);
	//	fread((char *)&RGB_color[i].rgbGreen, 1, sizeof(BYTE), input_file);
	//	fread((char *)&RGB_color[i].rgbRed, 1, sizeof(BYTE), input_file);
	//}

	//for (int i = 0; i < height; ++i)  //將像素寫入image_ori
	//{
	//	for (int j = 0; j < width; ++j)
	//	{
	//		(*(image_ori + i * width + j)).blue = 0;
	//		(*(image_ori + i * width + j)).green = 0;
	//		(*(image_ori + i * width + j)).red = 0;
	//	}
	//}

	int sum;
	for (i = 0; i < width*height; ++i) {   ///轉灰階
		sum = (*(image_ori + i)).blue + (*(image_ori + i)).green + (*(image_ori + i)).red;
		(*(image_ori + i)).blue = (*(image_ori + i)).green = (*(image_ori + i)).red = sum / 3;
	}

	for (i = 0; i < width*height; ++i) {  //2值化
		if ((*(image_ori + i)).blue > 128)
			(*(image_ori + i)).blue = (*(image_ori + i)).green = (*(image_ori + i)).red = 255;
		else
			(*(image_ori + i)).blue = (*(image_ori + i)).green = (*(image_ori + i)).red = 0;
	}

	for (i = 0; i < height; ++i)  //丟進存圖
	{
		for (j = 0; j < width; ++j)
		{

			*(image_change + i * width + j) = *(image_ori + i * height + j);
		}
	}

	for (i = 0; i < height; i++)   //一為丟二為
	{
		for (j = 0; j < width; j++)
		{
			int buff;
			buff = (*(image_change + i * width + j)).blue;
			//printf("buff：%d\n", buff);
			//image_change_med[i][j]=
			image_change_med[i][j] = buff;  /////原圖
			//printf("i：%d , j：%d , 像素值：%d , buff值：%d",i,j, in_Image[i][j], *(image_change + i * width + j));

		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////拓寬開始(複製外圈)

	for (i = side; i < height + side; i++)   //拓寬圖 OK
	{
		for (j = side; j < width + side; j++)
		{
			image_change_med2[i][j] = image_change_med[i - side][j - side];
		}
	}

	for (i = 0; i < side; i++)   //拓寬圖-左邊 左上OK 
	{
		for (j = 0; j < side; j++)
		{
			image_change_med2[i][j] = 0;
		}
	}

	for (i = height; i < (height + 2 * side); i++)   //拓寬圖-左邊 左下
	{
		for (j = 0; j < side; j++)
		{
			image_change_med2[i][j] = 0;
		}
	}

	for (i = side; i < (height + side); i++)   //拓寬圖-左邊 OK 
	{
		for (j = 0; j < side; j++)
		{
			image_change_med2[i][j] = 0;
		}
	}

	for (i = 0; i < side; i++)   //拓寬圖-上邊 OK 
	{
		for (j = side; j < (width + side); j++)
		{
			image_change_med2[i][j] = 0;
		}
	}

	for (i = height; i < height + 2 * side; i++)   //拓寬圖-下邊
	{
		for (j = side; j < width + side; j++)
		{
			image_change_med2[i][j] = 0;
		}
	}

	for (i = side; i < height + side; i++)   //拓寬圖-右邊
	{
		for (j = width + side; j < (width + 2 * side); j++)
		{
			image_change_med2[i][j] = 0;
		}
	}

	for (i = 0; i < side; i++)   //拓寬圖-右邊 右上
	{
		for (j = width + side; j < width + 2 * side; j++)
		{
			image_change_med2[i][j] = 0;
		}
	}

	for (i = height + side; i < height + 2 * side; i++)   //拓寬圖-右邊 右下
	{
		for (j = width + side; j < width + 2 * side; j++)
		{
			image_change_med2[i][j] = 0;
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////拓寬完畢

	for (i = side; i < height + side; i++)   //TWO-PASS
	{
		for (j = side; j < width + side; j++)
		{
			if (image_change_med2[i][j] != 0)
			{
				if (image_change_med2[i][j] == 255 &&               //搜四鄰
					determine_0_255(image_change_med2[i - 1][j]) && //上
					determine_0_255(image_change_med2[i + 1][j]) && //下
					determine_0_255(image_change_med2[i][j - 1]) && //左
					determine_0_255(image_change_med2[i][j + 1]))   //右
				{

					label[label_num] = label_num;        ///label圖
					image_change_med2[i][j] = label_num;
					if (image_change_med2[i - 1][j] == 255)
					{
						image_change_med2[i - 1][j] = label_num;
					}
					if (image_change_med2[i + 1][j] == 255)
					{
						image_change_med2[i + 1][j] = label_num;
					}
					if (image_change_med2[i][j - 1] == 255)
					{
						image_change_med2[i][j - 1] = label_num;
					}
					if (image_change_med2[i][j + 1] == 255)
					{
						image_change_med2[i][j + 1] = label_num;
					}
					label_num += 10;
					if (label_num == 255) //防爆
					{
						label[label_num] = 255;
						label_num = 256;
					}
				}
				else
				{
					int findmini = 14000;   //找最小label
					compare_min(image_change_med2[i][j], findmini);
					compare_min(image_change_med2[i - 1][j], findmini); //上
					compare_min(image_change_med2[i + 1][j], findmini); //下
					compare_min(image_change_med2[i][j - 1], findmini); //左
					compare_min(image_change_med2[i][j + 1], findmini); //右

					setvalue(image_change_med2[i][j], findmini);  //把最小label傳回去
					setvalue(image_change_med2[i - 1][j], findmini); //上
					setvalue(image_change_med2[i + 1][j], findmini); //下
					setvalue(image_change_med2[i][j - 1], findmini); //左
					setvalue(image_change_med2[i][j + 1], findmini); //右
				}

			}

		}
	}

	for (size_t i = 1; i <= label_num; i++)//重新調整label的mapping?
	{
		label[i] = label[label[i]];
	}

	for (i = 0; i < height; ++i)  //////對圖像像素進行重新給值
	{
		for (j = 0; j < width; ++j)
		{
			if (image_change_med2[i][j] != 0)
			{
				image_change_med2[i][j] = label[image_change_med2[i][j]];
			}

		}
	}

	for (i = 0; i < height; ++i)  ///////丟進存圖
	{
		for (j = 0; j < width; ++j)
		{
			int buff2;
			buff2 = image_change_med2[i][j];
			(*(image_change2 + i * width + j)).blue = buff2;
			(*(image_change2 + i * width + j)).green = buff2;
			(*(image_change2 + i * width + j)).red = buff2;
			//printf("buff2：%d\n", buff2);
		}
	}

	int buff_h_min = 512;
	int buff_w_min = 512;
	int buff_h_max = 0;
	int buff_w_max = 0;
	int buff_sum[14000] = { 0 };
	int scal = 0;
	for (i = 20; i <= label_num; i += 10)//畫線
	{
		buff_h_min = 512;
		buff_w_min = 512;
		buff_h_max = 0;
		buff_w_max = 0;
		for (int h = 0; h < height; ++h)  //////
		{
			for (int w = 0; w < width; ++w)
			{
				if ((image_change_med2[h][w]) == i)
				{
					compare_min(h, buff_h_min);
					compare_min(w, buff_w_min);
					compare_max(h, buff_h_max);
					compare_max(w, buff_w_max);
					buff_sum[i]++;
				}
			}
		}

		if (buff_sum[i] > 1000) //>1000就框
		{

			for (int h = buff_h_min; h <= buff_h_max; ++h)  //////畫直線
			{
				if (h == buff_h_min)
				{
					for (int w = buff_w_min; w <= buff_w_max; ++w)
					{
						image_change_med2[h][w] = i;
						(*(image_change2 + h * width + w)).red = image_change_med2[h][w];
						(*(image_change2 + (h + 1) * width + w)).red = image_change_med2[h][w];
						(*(image_change2 + (h + 2) * width + w)).red = image_change_med2[h][w];
					}
				}
				if (h == buff_h_max)
				{
					for (int w = buff_w_min; w <= buff_w_max; ++w)
					{
						image_change_med2[h][w] = i;
						(*(image_change2 + h * width + w)).red = image_change_med2[h][w];
						(*(image_change2 + (h - 1) * width + w)).red = image_change_med2[h][w];
						(*(image_change2 + (h - 2) * width + w)).red = image_change_med2[h][w];
					}
				}
			}

			for (int w = buff_w_min; w <= buff_w_max; ++w)  //////畫橫線
			{
				if (w == buff_w_min)
				{
					for (int h = buff_h_min; h <= buff_h_max; ++h)
					{
						image_change_med2[h][w] = i;
						(*(image_change2 + h * width + w)).red = image_change_med2[h][w];
						(*(image_change2 + h * width + w + 1)).red = image_change_med2[h][w];
						(*(image_change2 + h * width + w + 2)).red = image_change_med2[h][w];
					}
				}
				if (w == buff_w_max)
				{
					for (int h = buff_h_min; h <= buff_h_max; ++h)
					{
						image_change_med2[h][w] = i;
						(*(image_change2 + h * width + w)).red = image_change_med2[h][w];
						(*(image_change2 + h * width + w - 1)).red = image_change_med2[h][w];
						(*(image_change2 + h * width + w - 2)).red = image_change_med2[h][w];
					}
				}
			}
			scal++;
			printf("第%d個區域面積：%d\n", scal, buff_sum[i]);
		}

	}

	/*cout << "\n輸入圖片將旋轉+RGB調換~" << endl;

	for (int i = 0; i <  height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
				*(image_change + i * width + j) = *(image_ori +j * height + i);
		}
	}     */

	printf("程式總花費時間：%dms\n", clock() - time);

	system("pause");

// saving file
	cout << "\n請輸入輸出檔案名稱(要打副檔名)：\n" << endl;  
	cin >> write_file;
	if (write_file == NULL) {
		puts("Loading File name Error!");
		system("pause");
		exit(0);
	}
	output_file = fopen(write_file, "wb");

	WORD bfType_w = 0x4d42;  
	fwrite(&bfType_w, 1, sizeof(WORD), output_file);
	fwrite(&BMP_head, 1, sizeof(Bitmap_File_Header), output_file);
	BMP_info.biWidth = width;
	BMP_info.biHeight = height;
	fwrite(&BMP_info, 1, sizeof(Bitmap_File_HeaderInfo), output_file);

	for (unsigned int i = 0; i < BMP_info.biClrUsed; i++)  ///
	{
		fwrite(&RGB_color[i].rgbBlue, 1, sizeof(BYTE), output_file);
		fwrite(&RGB_color[i].rgbGreen, 1, sizeof(BYTE), output_file);
		fwrite(&RGB_color[i].rgbRed, 1, sizeof(BYTE), output_file);
	}

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			fwrite(&(*(image_change2 + i * width + j)).blue, 1, sizeof(BYTE), output_file);// RGB chanl
			fwrite(&(*(image_change2 + i * width + j)).green, 1, sizeof(BYTE), output_file);
			fwrite(&(*(image_change2 + i * width + j)).red, 1, sizeof(BYTE), output_file);
		}
	}

	fclose(input_file);
	fclose(output_file);
	free(image_ori);
	free(image_change);
	delete[] image_change_med;
	delete[] image_change_med2;
}
