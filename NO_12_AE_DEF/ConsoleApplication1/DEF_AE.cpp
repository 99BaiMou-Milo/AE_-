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
	//����׼��
	long add_absy = 0;
	Mat outputImage = inputImage.clone();  //����ʵ�ε���ʱ����
	int rowNumber = outputImage.rows;  //����
	int colNumber = outputImage.cols * outputImage.channels();  //���� x ͨ����=ÿһ��Ԫ�صĸ���
	//˫��ѭ�����������е�����ֵ

	for (int i = 1; i < rowNumber ; i++)  //��ѭ��
	{
		uchar* data = inputImage.ptr<uchar>(i);  //��ȡ��i�е��׵�ַ

		for (int j = 1; j < colNumber ; j++)   //��ѭ��
		{
			// ---------����ʼ����ÿ�����ء�-------------  
			//datao[j] = abs((data1[j] - data1[j + 2]) + (2 * data2[j] - 2 * data2[j + 2]) + (data3[j] - data3[j + 2]));
			add_absy += data[j];//a < 50 ? 0 : 255;
			// ----------�����������---------------------
		}  //�д������
	}
	return add_absy / (rowNumber*colNumber);
}


long Get_FOUCS(Mat& outputImage)
{
	//����׼��
	//outputImage = inputImage.clone();  //����ʵ�ε���ʱ����

	int rowNumber = outputImage.rows;  //����
	int colNumber = outputImage.cols * outputImage.channels();  //���� x ͨ����=ÿһ��Ԫ�صĸ���
	long jsq = 0;
	//˫��ѭ�����������е�����ֵ
	for (int i = 1; i < rowNumber; i++)  //��ѭ��
	{
		uchar* data1 = outputImage.ptr<uchar>(i);  //��ȡ��i�е��׵�ַ

		for (int j = 1; j < colNumber; j++)   //��ѭ��
		{
			// ---------����ʼ����ÿ�����ء�-------------  
			//datao[j] = abs((data1[j] - data1[j + 2]) + (2 * data2[j] - 2 * data2[j + 2]) + (data3[j] - data3[j + 2]));
			if (data1[j] >= 180)
			{
				jsq++;
			}
			// ----------�����������---------------------
		}  //�д������
	}
	//printf("����ֵ = %d \n", jsq);
	return jsq;
}

int main() {

	VideoCapture camera(0); // 0 ��ʾѡ��Ĭ�ϵ�����ͷ

	if (!camera.isOpened()) {
		std::cerr << "�޷�������ͷ" << std::endl;
		return -1;
	}
	camera.set(CV_CAP_PROP_FRAME_WIDTH, 800);
	camera.set(CV_CAP_PROP_FRAME_HEIGHT, 600);

	Mat frame;
	long Gain = 2000;
	long absy = 0;

	int proportion = 0;//����
	int integral = 0;//������
	int differential = 0;//������

	camera.set(CAP_PROP_BACKLIGHT, 720);

	while (1)
	{
		camera.read(frame);
		//cv::cvtColor(frame,frame,cv::COLOR_BGR2GRAY);
		imshow("��ȡԭʼ��Ƶ", frame);  //��ʾ��ǰ֡
		absy = Absy_get(frame);
		//cv::cvtColor(frame, frame0, cv::COLOR_BGR2GRAY);
		//Edge_Chek(frame0, frame1, 1);
		//imshow("��ȡ������Ƶ", frame1);  //��ñ�Եͼ

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
	//��ȡһ��ͼƬ
	cv::Mat i = cv::imread("qiaoba.jpg",cv::IMREAD_ANYCOLOR);//�Ҷȶ�ȡ  cv::IMREAD_GRAYSCALE
	//����һ����Ϊ��image���Ĵ��ڣ����Ըı䴰�ڴ�С
	cv::namedWindow("image", cv::WINDOW_NORMAL);
	//�ڴ�������ʾͼ��
	printf("mei nv !");
	cv::imshow("image", i);
	//���س������ڹر�
	cv::waitKey(0);

}*/
