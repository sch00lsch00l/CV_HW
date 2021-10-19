/*Implement the reference paper "Projective Mappings for Image Warping" */

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

int i, j, k;
using namespace std;

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

unsigned char*** creat_3D_ARR(int x, int y, int z)
{
	unsigned char*** IN_array = new unsigned char**[x];
	for (int i(0); i < x; ++i)
	{
		IN_array[i] = new unsigned char*[y];

		for (int j(0); j < y; ++j)
		{
			IN_array[i][j] = new unsigned char[z];

			for (int k(0); k < z; ++k)
			{
				IN_array[i][j][k] = 0;
			}
		}
	}
	return IN_array;
}

void delet_3D_ARR(unsigned char*** the_array, int x, int y, int z)
{
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < y; ++j)
		{
			delete[] the_array[i][j];
		}
		delete[] the_array[i];
	}
	delete[] the_array;
}

int main() {
	char open_file[30];
	char write_file[30];

	char OriFile_L[] = "left.raw"; //intput raw image
	char OriFile_R[] = "right.raw";

	int width, height;
	int chanel = 3;
	int size_one = 370 * 510 * chanel;
	long time = clock();
	int value;

	FILE *input_file_L, *input_file_R, *output_file;
	input_file_L = fopen(OriFile_L, "rb");
	input_file_R = fopen(OriFile_R, "rb");
	
	unsigned char* left_im_in = new unsigned char[size_one];
	unsigned char* right_im_in = new unsigned char[size_one];

	uchar* IM_end = new uchar[480 * 900 * 3]; 
	unsigned char*** left_IM = creat_3D_ARR(370, 510, chanel); 
	unsigned char*** right_IM = creat_3D_ARR(370, 510, chanel); 
	unsigned char*** BIG_IM = creat_3D_ARR(480, 900, chanel); 

	fread(left_im_in, 1, size_one, input_file_L);  
	fread(right_im_in, 1, size_one, input_file_R);

	for (i = 0; i < 370; i++)   //1D to 3D
	{
		for (j = 0; j < 510; j++)
		{
			for (k = 0; k < 3; k++)
			{
				left_IM[i][j][k] = left_im_in[i*chanel * 510 + j * chanel + k];
			}
		}
	}

	for (i = 80; i < 450; i++)  //left image
	{
		for (j = 0; j < 510; j++)
		{
			for (k = 0; k < chanel; k++)
			{
				BIG_IM[i][j][k] = left_IM[i - 80][j][k];
			}
		}
	}

	for (i = 0; i < 370; i++)   //right image
	{
		for (j = 0; j < 510; j++)
		{
			for (k = 0; k < 3; k++)
			{
				right_IM[i][j][k] = right_im_in[i*chanel * 510 + j * chanel + k];
			}
		}
	}
	
	float u = 0, v = 0;
	float x0 = 130, x1 = 470, x2 = 475, x3 = 0;   
	float y0 = 250, y1 = 310, y2 = 900, y3 = 770;
	//define from paper_P.4
	float g = ((x0 - x1 + x2 - x3)*(y3 - y2) - (x3 - x2)*(y0 - y1 + y2 - y3)) / ((x1 - x2)*(y3 - y2) - (x3 - x2)*(y1 - y2));
	float h = ((x1 - x2)*(y0 - y1 + y2 - y3) - (x0 - x1 + x2 - x3)*(y1 - y2)) / ((x1 - x2)*(y3 - y2) - (y1 - y2)*(x3 - x2));
	float a = x1 - x0 + g * x1;
	float b = x3 - x0 + h * x3;
	float c = x0;
	float d = y1 - y0 + g * y1;
	float e = y3 - y0 + h * y3;
	float f = y0;
	float ii = 1; //parameter i
	int buffer_u = 0, buffer_v = 0;

	for (i = 0; i < 480; i++)
	{
		for (j = 0; j < 900; j++)
		{
			for (k = 0; k < chanel; k++)
			{
				u = ((e*ii - f * h)*i + (c*h - b * ii)*j + (b*f - c * e)) / ((d*h - e * g)*i + (b*g - a * h)*j + (a*e - b * d));  //paper_P.2
				v = ((f*g - d * ii)*i + (a*ii - c * g)*j + (c*d - a * f)) / ((d*h - e * g)*i + (b*g - a * h)*j + (a*e - b * d));
				
				buffer_u = u * 370;
				buffer_v = v * 510;
				if (buffer_u >= 0 && buffer_v >= 0 && buffer_u < 370 && buffer_v < 510)
				{
					BIG_IM[i][j][k] = right_IM[buffer_u][buffer_v][k];
				}
			}
		}
	}

	printf("程式總花費時間：%dms\n", clock() - time);

	system("pause");

	cout << "\n請輸入輸出結果檔案名稱(要打副檔名)：\n" << endl;  
	cin >> write_file;
	if (write_file == NULL) {
		puts("Loading File name Error!");
		system("pause");
		exit(0);
	}
	output_file = fopen(write_file, "wb");
	
	for (int i = 0; i < 480; i++)  
	{
		for (int j = 0; j < 900; j++)
		{
			for (int k = 0; k < chanel; k++)
			{
				IM_end[i * 3 * 900 + j * 3 + k] = BIG_IM[i][j][k];
			}
		}
	}
	fwrite(IM_end, 1, 480 * 900 * 3, output_file);

	fclose(output_file);
	delete left_im_in;
	delete right_im_in;

	delet_3D_ARR(left_IM, 480, 900, 3);
	delet_3D_ARR(right_IM, 480, 900, 3);
	delet_3D_ARR(BIG_IM, 480, 900, 3);
}
