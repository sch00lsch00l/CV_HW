#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include "BMP_Header.h"
#include "stdlib.h"
#include "math.h"

#define input_filename 30//the max length of bmp file name

using namespace std;

Bitmap_File_Header BMP_head;
Palette_RGB RGB_color[256];//256調色盤
Bitmap_File_HeaderInfo BMP_info;

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


int main() {
	char open_file[30];
	char write_file[30];
	Image_data *image_ori;//存原圖
	Image_data *image_change;//存旋轉後的圖
	int width, height;

	cout << "請輸入檔案名稱(要打副檔名)：" << endl;
	cin >> open_file;
	FILE *input_file, *output_file;
	input_file = fopen(open_file, "rb");


	WORD bfType;
	fread(&bfType, 1, sizeof(WORD), input_file);
	
		
	fread(&BMP_head, 1, sizeof(Bitmap_File_Header), input_file);  //讀檔案標頭
	//show_bmp_header(BMP_head);//看檔案標頭Bitmap_File_Header
	fread(&BMP_info, 1, sizeof(Bitmap_File_HeaderInfo), input_file);
	show_bmp_headerinfo(BMP_info);  //看檔案標頭Bitmap_File_HeaderInfo

	//get the proportion of RGB
	for (unsigned int i = 0; i < BMP_info.biClrUsed; i++)
	{
		fread((char *)&RGB_color[i].rgbBlue, 1, sizeof(BYTE), input_file);
		fread((char *)&RGB_color[i].rgbGreen, 1, sizeof(BYTE), input_file);
		fread((char *)&RGB_color[i].rgbRed, 1, sizeof(BYTE), input_file);
		cout << "RGB_color[i].rgbBlue" << RGB_color[i].rgbBlue << endl;
		cout << "RGB_color[i].rgbGreen" << RGB_color[i].rgbGreen << endl;
		cout << "RGB_color[i].rgbRed" << RGB_color[i].rgbRed << endl;
	}
	width = BMP_info.biHeight;
	height = BMP_info.biWidth;
	image_ori = (Image_data*)malloc(width * height * sizeof(Image_data));
	image_change = (Image_data*)malloc( width   * height * sizeof(Image_data));

	//orianginal
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			(*(image_ori + i * width + j)).blue = 0;
			(*(image_ori + i * width + j)).green = 0;
			(*(image_ori + i * width + j)).red = 0;
		}
	}
	//rotation
/*	for (int i = 0; i <  height; ++i)
	{
		for (int j = 0; j <  width; ++j)
		{
			(*(image_change + j   * width + i)).blue = 0;
			(*(image_change + j   * width + i)).green = 0;
			(*(image_change + j   * width + i)).red = 0;
		}
	}	*/
	//infomation of image pixel
	fread(image_ori, sizeof(struct Image_data) * width, height, input_file);
	

	
	cout << "\n輸入圖片將旋轉+RGB調換" << endl;
        
	for (int i = 0; i <  height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
				*(image_change + i * width + j) = *(image_ori +j * height + i);
		}
	}     
	
	system("pause");

	//writing the BMP file
	cout << "\n請輸入輸出檔案名稱(要打副檔名)：\n" << endl;
	cin >> write_file;
	output_file = fopen(write_file, "wb");
	
	WORD bfType_w = 0x4d42;
	fwrite(&bfType_w, 1, sizeof(WORD), output_file);
	fwrite(&BMP_head, 1, sizeof(Bitmap_File_Header), output_file);
	BMP_info.biWidth =  width;
	BMP_info.biHeight =  height;
	fwrite(&BMP_info, 1, sizeof(Bitmap_File_HeaderInfo), output_file);

	for (unsigned int i = 0; i < BMP_info.biClrUsed; i++)
	{
		fwrite(&RGB_color[i].rgbBlue, 1, sizeof(BYTE), output_file);
		fwrite(&RGB_color[i].rgbGreen, 1, sizeof(BYTE), output_file);
		fwrite(&RGB_color[i].rgbRed, 1, sizeof(BYTE), output_file);
	}
	
	for (int i = 0; i <  height; ++i)
	{
		for (int j = 0; j <  width; ++j)
		{
			fwrite(&(*(image_change + i   * width + j)).green, 1, sizeof(BYTE), output_file);//排顏色
			fwrite(&(*(image_change + i   * width + j)).blue, 1, sizeof(BYTE), output_file);
			fwrite(&(*(image_change + i   * width + j)).red, 1, sizeof(BYTE), output_file);
		}
	}

	fclose(input_file);
	fclose(output_file);
	delete[] image_ori;
	delete[] image_change;
}
