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
Palette_RGB RGB_color[256];
Bitmap_File_HeaderInfo BMP_info;

using namespace std;

int label[13500] = { 0 };

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

bool determine_0_255(int &value)
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

void setvalue(int &value, int &minimun)
{
	if (value != 0)
	{
		if (label[value] > minimun && label[value] != 255)
		{
			int buff_med_val = label[value];
			while (true)
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

void compare_min(int &value, int &minimun)
{
	if (value != 0 && value != 255)
	{
		if (minimun >= value)
		{
			minimun = value;
		}
	}
}

void compare_max(int &value, int &bigmun)
{
	if (value != 0 && value != 255)
	{
		if (bigmun <= value)
		{
			bigmun = value;
		}
	}
}


int main() {
	char open_file[]="distortion.bmp";
	char write_file[] = "OUT.bmp";
	Image_data *image_ori;
	Image_data *image_change;
	int **image_change_med;
	int **image_change_med2;
	Image_data *image_change2;
	int width, height;
	int red, green, blue;
	int i, j;
	BYTE gray;
	int r, sita;
	float w = 0.0036;
	float x_max, y_max;

	int label_num = 50;
	long time = clock();
	int value;

	FILE *input_file, *output_file;
	input_file = fopen(open_file, "rb");
	WORD bfType;
	fread(&bfType, 1, sizeof(WORD), input_file);


	fread(&BMP_head, 1, sizeof(Bitmap_File_Header), input_file);  
	fread(&BMP_info, 1, sizeof(Bitmap_File_HeaderInfo), input_file);

	height = BMP_info.biHeight; //swap the hight and width
	width = BMP_info.biWidth;

	float x0 = width / 2;
	float y0 = height / 2;
	float sita_rd_max = atan2(height - y0, width - x0);
	float rd_max = sqrt(pow(height - y0, 2) + pow(width - x0, 2));
	float ru_max = tan(rd_max*w) / (2 * tan(w / 2));

	x_max = ru_max * cos(sita_rd_max);
	y_max = ru_max * sin(sita_rd_max);
	int Hu, Wu;
	Hu = 2 * y_max;
	Wu = 2 * x_max;

	float Wu_x0 = Wu / 2;
	float Hu_y0 = Hu / 2;

	image_ori = (Image_data*)malloc(width * height * sizeof(Image_data)); //Adaptive
	image_change = (Image_data*)malloc(width * height * sizeof(Image_data));
	image_change2 = (Image_data*)malloc(Hu*Wu * sizeof(Image_data));
	image_change_med = NEW2D(height, width, int);
	image_change_med2 = NEW2D(Hu, Wu, int);

	fread(image_ori, sizeof(struct Image_data) * width, height, input_file);

	int sum;
	for (i = 0; i < width*height; ++i) {   ///gray scal
		sum = (*(image_ori + i)).blue + (*(image_ori + i)).green + (*(image_ori + i)).red;
		(*(image_ori + i)).blue = (*(image_ori + i)).green = (*(image_ori + i)).red = sum / 3;
	}


	for (i = 0; i < height; ++i) 
	{
		for (j = 0; j < width; ++j)
		{

			*(image_change + i * width + j) = *(image_ori + i * width + j);
		}
	}

	for (i = 0; i < height; i++) 
	{
		for (j = 0; j < width; j++)
		{
			int buff_B, buff_G, buff_R;
			buff_B = (*(image_change + i * width + j)).blue;
			image_change_med[i][j] = buff_B; 
		}
	}

	for (i = 0; i < Hu; i++)   //initialization
	{
		for (j = 0; j < Wu; j++)
		{
			image_change_med2[i][j] = 0;  //original
		}
	}	

	for (i = 0; i < Hu; i++)   // restructure the pixel
	{
		for (j = 0; j < Wu; j++)
		{
			float ru = sqrt(pow(i - Hu_y0, 2) + pow(j - Wu_x0, 2));
			float rd = (1 / w)*atan(2*ru*(tan(w/2)));

			float theta = atan2(i - Hu_y0, j - Wu_x0);
			float n_x = rd * cos(theta) + Wu_x0;
			float n_y = rd * sin(theta) + Hu_y0;
			//printf("n_x：%d , n_y：%d\n", n_x, n_y);
			//printf("i：%d , j：%d\n", i, j);
			int n_x_c = int(n_x);
			int n_y_c = int(n_y);
			int save_v = image_change_med[n_x_c][n_y_c];

			if ((0 <= n_x_c && n_x_c < height)) {
				if ((0 <= n_y_c && n_y_c < width)) {
					image_change_med2[i][j] = save_v;
				}
			}

		}
	}

	for (i = 0; i < Hu; ++i) 
	{
		for (j = 0; j < Wu; ++j)
		{
			int buff2;
			buff2 = image_change_med2[i][j];
			(*(image_change2 + j * Wu + i)).blue = buff2;
			(*(image_change2 + j * Wu + i)).green = buff2;
			(*(image_change2 + j * Wu + i)).red = buff2;
			//printf("buff2：%d\n", buff2);
		}
	}

	printf("程式總花費時間：%dms\n", clock() - time);

	system("pause");

	//saving file
	output_file = fopen(write_file, "wb");

	WORD bfType_w = 0x4d42;  
	fwrite(&bfType_w, 1, sizeof(WORD), output_file);
	fwrite(&BMP_head, 1, sizeof(Bitmap_File_Header), output_file);
	BMP_info.biWidth = Wu;
	BMP_info.biHeight = Hu;
	fwrite(&BMP_info, 1, sizeof(Bitmap_File_HeaderInfo), output_file);

	for (unsigned int i = 0; i < BMP_info.biClrUsed; i++) 
	{
		fwrite(&RGB_color[i].rgbBlue, 1, sizeof(BYTE), output_file);
		fwrite(&RGB_color[i].rgbGreen, 1, sizeof(BYTE), output_file);
		fwrite(&RGB_color[i].rgbRed, 1, sizeof(BYTE), output_file);
	}

	for (int i = 0; i < Hu; ++i)
	{
		for (int j = 0; j < Wu; ++j)
		{
			fwrite(&(*(image_change2 + i * Wu + j)).blue, 1, sizeof(BYTE), output_file);//RGB chanal
			fwrite(&(*(image_change2 + i * Wu + j)).green, 1, sizeof(BYTE), output_file);
			fwrite(&(*(image_change2 + i * Wu + j)).red, 1, sizeof(BYTE), output_file);
		}
	}

	fclose(input_file);
	fclose(output_file);
	free(image_ori);
	free(image_change);
	delete [] image_change_med;
	delete [] image_change_med2;
}
