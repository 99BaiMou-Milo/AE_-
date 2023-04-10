#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <iostream>

#include"SEN_sc200AI.h"

#define PID_P  6   // MXA 6 - +
#define PID_I  0.1 
#define PID_D  1 
using namespace cv;
float Absy_get(Mat& inputImage)
{
	//参数准备
	long add_absy = 0;
	Mat outputImage = inputImage.clone();  //拷贝实参到临时变量
	int rowNumber = outputImage.rows;  //行数
	int colNumber = outputImage.cols * outputImage.channels();  //列数 x 通道数=每一行元素的个数
	//双重循环，遍历所有的像素值

	for (int i = 1; i < rowNumber ; i++)  //行循环
	{
		uchar* data = inputImage.ptr<uchar>(i);  //获取第i行的首地址

		for (int j = 1; j < colNumber ; j++)   //列循环
		{
			// ---------【开始处理每个像素】-------------  
			//datao[j] = abs((data1[j] - data1[j + 2]) + (2 * data2[j] - 2 * data2[j + 2]) + (data3[j] - data3[j + 2]));
			add_absy += data[j];//a < 50 ? 0 : 255;
			// ----------【处理结束】---------------------
		}  //行处理结束
	}
	return add_absy / (rowNumber*colNumber);
}


long Get_FOUCS(Mat& outputImage)
{
	//参数准备
	//outputImage = inputImage.clone();  //拷贝实参到临时变量

	int rowNumber = outputImage.rows;  //行数
	int colNumber = outputImage.cols * outputImage.channels();  //列数 x 通道数=每一行元素的个数
	long jsq = 0;
	//双重循环，遍历所有的像素值
	for (int i = 1; i < rowNumber; i++)  //行循环
	{
		uchar* data1 = outputImage.ptr<uchar>(i);  //获取第i行的首地址

		for (int j = 1; j < colNumber; j++)   //列循环
		{
			// ---------【开始处理每个像素】-------------  
			//datao[j] = abs((data1[j] - data1[j + 2]) + (2 * data2[j] - 2 * data2[j + 2]) + (data3[j] - data3[j + 2]));
			if (data1[j] >= 180)
			{
				jsq++;
			}
			// ----------【处理结束】---------------------
		}  //行处理结束
	}
	//printf("焦点值 = %d \n", jsq);
	return jsq;
}

int main() {

	VideoCapture camera(0); // 0 表示选择默认的摄像头

	if (!camera.isOpened()) {
		std::cerr << "无法打开摄像头" << std::endl;
		return -1;
	}
	camera.set(CV_CAP_PROP_FRAME_WIDTH, 800);
	camera.set(CV_CAP_PROP_FRAME_HEIGHT, 600);

	Mat frame;
	long Gain = 2000;
	long absy = 0;

	int proportion = 0;//比例
	int integral = 0;//积分项
	int differential = 0;//积分项

	camera.set(CAP_PROP_BACKLIGHT, 720);

	while (1)
	{
		camera.read(frame);
		//cv::cvtColor(frame,frame,cv::COLOR_BGR2GRAY);
		imshow("读取原始视频", frame);  //显示当前帧
		absy = Absy_get(frame);
		//cv::cvtColor(frame, frame0, cv::COLOR_BGR2GRAY);
		//Edge_Chek(frame0, frame1, 1);
		//imshow("读取处理视频", frame1);  //获得边缘图

		proportion = (128 - absy);

		integral += (128 - absy);
		integral = integral >  600 ? 600 : integral;
		integral = integral <  -600 ? -600 : integral;

		if (Gain <= 65000)
		{
			Gain += proportion * PID_P;
			Gain += integral   * PID_I;
		}
		else
		{
			Gain = 65000;
		}
		//Gain += (128 - absy)*PID_P;
		Gain = Gain <  0 ? 0 : Gain;
		Gain = Gain >  65535 ? 65535: Gain;

		Set_G(camera, (int)Gain);

		/*
		exprouse = exprouse < 0 ? 0 : exprouse;
		exprouse = exprouse > 54000 ? 54000 : exprouse;
		camera.set(CAP_PROP_GAIN, exprouse);
		*/

		printf("absy == %d \n", (int)absy);
		printf("exprouse == %d \n", Gain);
		cv::waitKey(5);

	}
	return 0;
}

/*
void main()
{
	//读取一张图片
	cv::Mat i = cv::imread("qiaoba.jpg",cv::IMREAD_ANYCOLOR);//灰度读取  cv::IMREAD_GRAYSCALE
	//创建一个名为“image”的窗口，可以改变窗口大小
	cv::namedWindow("image", cv::WINDOW_NORMAL);
	//在窗口中显示图像
	printf("mei nv !");
	cv::imshow("image", i);
	//按回车键窗口关闭
	cv::waitKey(0);

}*/
