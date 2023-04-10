#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <iostream>
#include"SEN_sc200AI.h"
#include "AE_LIB.h"
#include "NC.h"
#include <cmath>
#if(SENSOR_IDX == SC200AI)
#include"SEN_sc200AI.h"
#endif

using namespace cv;

//
// tuning  UI start
//

#define Absy_Dynamic_target    0 //Ŀ�������Ƿ�̬��������ǣ��Ǿ��Ǿ�̬Ŀ�� 

#define TARGET_ABSY   128  //static target �� ��̬Ŀ��

#define DEF_ExpIdx_EN  0 //��Ϊ����sensor�������� ��������������С�ع��� 
	#define DEF_ExpIdx_MAX 8000 
	#define DEF_ExpIdx_MIN 0 

#define State_CTRL  1 

//��̬PIDϵ������
#define PID_P  1   // MXA 6 - +
#define PID_I  0.1 
#define PID_D  1 //not used

// ������ ��̬PIDϵ�� ��̬Ŀ������
// ǰ��ֻ�ǲο��ع�ָ����״ֵ̬
#define NC_DATA_H  6  //just for NC_Data
#define NC_DATA_V  5  //just for NC_Data
int Out_Data[NC_DATA_H - 2]; //����  �����������ĸ����׶���Ҫ��ֵ��
int NC_Data[NC_DATA_H * NC_DATA_V] = //�����׶ε��ٶȿ��ƣ�����С�ع�ʱ �ٶȽ��� ����֮��Ȼ
{// ���   �յ�  ����    ����      ΢��    ��̬����Ŀ��
//     S    E     P       I        D       D_T_Absy     ! ! !  ---> 1000/1000
//	8000,  65535, 5000,  1200,    170,
	1420,  16000,  1200,  300,    0,      100,//�ͻ�������
	720,   1400,   1000,  200,     0,     120,
	700,    900,    700, 200,    0,      120,
	200,	   300,	   400, 200,     0,      160,
	0,     20,     10,    0,       0,    120,//�߻�������
};


float Time_Target[][2] = {
//   ʱ��/ms �� ����ֵ
	{ 20, 128 },
	{ 20, 128 },
	{ 20, 128 },
};



//
// tuning UI end
//

void AE_ExpIdx(float AbsyImg, VideoCapture CAM)
{
	static long slExpIdx = 1000;

	int int_proportion = 0;//����

	static int int_integral = 0;//������
	int int_differential = 0;//΢����

	NC_2((int)slExpIdx, NC_Data, NC_DATA_H, NC_DATA_V, Out_Data); //��ȡ�����ع�ָ���µĸ���Ŀ��ֵ������ֵ

#if( Absy_Dynamic_target == 1) //��̬����Ŀ��
	int_proportion = (Out_Data[3] - AbsyImg);
//	printf("Out_Data[3] == %d", Out_Data[3]); //debug

	int_integral += (Out_Data[3] - AbsyImg);
#else //��̬Ŀ��
	int_proportion = (TARGET_ABSY - AbsyImg);

	int_integral += (TARGET_ABSY - AbsyImg);
#endif

	int_integral = int_integral >  600 ? 600 : int_integral;
	int_integral = int_integral <  -600 ? -600 : int_integral;

	if (int_proportion < 5)//��������ȶ�  ��ʼ�˻��ֱ���
	{
		int_integral = (int_integral / 2);
	}

#if(State_CTRL == 1)

	//�ֶ�ѡȡ�ȶ���Χ  ������������� ���ǽضϴ���
	if ((abs(int_proportion) < 5) && (slExpIdx>20) )
		return;

	if ((abs(int_proportion) < 30) && (slExpIdx<20))
		return;

#endif //State_CTRL
	/*
	if ((slExpIdx <= DEF_ExpIdx_MAX))//С��ĳ��Ӳ������������ع�ָ��ֵ  
	{

		//NC_2((int)slExpIdx,NC_Data,5, 3, Out_Data);

		//printf("Out_Data P == %d\n", int((float)int_proportion * (float)PID_P * (float)(Out_Data[0] / 1000)));
		//printf("Out_Data I  ==  %d\n", int((float)int_integral   * (float)PID_I * (float)(Out_Data[1] / 1000)));

		slExpIdx += int((float)int_proportion * (float)PID_P * ((float)Out_Data[0] / (float)1000));
		slExpIdx += int((float)int_integral   * (float)PID_I * ((float)Out_Data[1] / (float)1000));
//		slExpIdx += int((float)int_differential   * (float)PID_D * ((float)Out_Data[2] / (float)1000));
		/*
		//ExpIdx += (long)(int_proportion * (float)(PID_P*(ExpIdx/1400)));
		if (ExpIdx >= ((10 * P_CLK) / (HTS * 1000))) //����10ms
		{
			ExpIdx += int_proportion * PID_P;
			ExpIdx += int_integral   * PID_I;
		}

		else if(ExpIdx >= 30) //С��10ms ������ ��С�����ع���  10 ~ minPixH
		{
			if (int_proportion > 0)//���ع�
			{
				ExpIdx += (int_proportion * PID_P)*0.1;
			}
			else //���ع�
			{
				ExpIdx += (int_proportion * PID_P)*0.1;
			}
			//ExpIdx += (int_integral   * PID_I)*0.1;
		}
		else //������������
		{
			if (int_proportion > 0)//���ع�
			{
				ExpIdx++;
			}
			else //���ع�
			{
				ExpIdx--;
			}
		}
		*/


		//ExpIdx += (long)(int_integral * (float)(PID_I*(ExpIdx / 1400)));
/*	}
	else
	{
		slExpIdx = DEF_ExpIdx_MAX;//mix sensor gain
	}*/

	slExpIdx += int((float)int_proportion * (float)PID_P * ((float)Out_Data[0] / (float)1000));
	slExpIdx += int((float)int_integral   * (float)PID_I * ((float)Out_Data[1] / (float)1000));

#if(DEF_ExpIdx_EN == 1 )
	slExpIdx = slExpIdx < DEF_ExpIdx_MIN ? DEF_ExpIdx_MIN : slExpIdx;
	slExpIdx = slExpIdx > DEF_ExpIdx_MAX ? DEF_ExpIdx_MAX : slExpIdx;
#else
	slExpIdx = slExpIdx < SEN_ExpIdx_MIN ? SEN_ExpIdx_MIN : slExpIdx;
	slExpIdx = slExpIdx > SEN_ExpIdx_MAX ? SEN_ExpIdx_MAX : slExpIdx;
#endif

	if (slExpIdx < ((Time_Target[1][0] * P_CLK) / (HTS * 1000)))
	{
		Set_Exp(CAM,(int)slExpIdx);
		Set_G(CAM, 0);
	}
	else
	{
		//(ExpIdx - ((Absy_Target[1][0] * P_CLK) / (HTS * 1000))
		Set_Exp(CAM, (int)((Time_Target[1][0] * P_CLK) / (HTS * 1000)));
		Set_G(CAM, (int)(slExpIdx - ((Time_Target[1][0] * P_CLK) / (HTS * 1000))));
	}

	printf("ExpIdx == %d \n", (int)slExpIdx);
	printf("absy == %f4.2 \n", AbsyImg);

//	printf("int_proportion == %d \n", int_proportion);
//	printf("int_integral == %d \n", int_integral);

}

int main() {

	VideoCapture camera(0); // 0 ��ʾѡ��Ĭ�ϵ�����ͷ
	if (!camera.isOpened()) {
		std::cerr << "�޷�������ͷ" << std::endl;
		return -1;
	}

	camera.set(CV_CAP_PROP_FRAME_WIDTH, 800); //���ÿ�ͼ�ķֱ��ʵĳ�
	camera.set(CV_CAP_PROP_FRAME_HEIGHT, 600);//���ÿ�ͼ�ķֱ��ʵĸ�

	Mat frame; //�����ڴ� 
	long absy = 0; //��ǰ����
	long absy1 = 0; //��ǰ����
	camera.set(CAP_PROP_BACKLIGHT, 720); //�����ع�ʱ���ʼֵ ����ν

	while (1)
	{
		camera.read(frame); //�������ȡͼƬ
		imshow("��ȡԭʼ��Ƶ", frame);  //��ʾ��ǰ֡

		cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

#if(ABSY_MODE ==  MEAN_MODE)
		absy = Absy_get(frame);
#else if(ABSY_MODE ==  HIST_MODE)
		absy = Absy_get_hist(frame); //��ȡ��ǰͼ������Absy_get_hist(frame);
#endif


		AE_ExpIdx(absy, camera);//AE����������ع�
		//cv::cvtColor(frame, frame0, cv::COLOR_BGR2GRAY);
		//Edge_Chek(frame0, frame1, 1);
		//imshow("��ȡ������Ƶ", frame1);  //��ñ�Եͼ


		cv::waitKey(5);
	}
	return 0;
}


