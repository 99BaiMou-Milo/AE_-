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

#define Absy_Dynamic_target    0 //目标亮度是否动态，如果不是，那就是静态目标 

#define TARGET_ABSY   128  //static target ！ 静态目标

#define DEF_ExpIdx_EN  0 //人为控制sensor工作区间 设置最大增益或最小曝光行 
	#define DEF_ExpIdx_MAX 8000 
	#define DEF_ExpIdx_MIN 0 

#define State_CTRL  1 

//静态PID系数部分
#define PID_P  1   // MXA 6 - +
#define PID_I  0.1 
#define PID_D  1 //not used

// 包括了 动态PID系数 动态目标亮度
// 前面只是参考曝光指数的状态值
#define NC_DATA_H  6  //just for NC_Data
#define NC_DATA_V  5  //just for NC_Data
int Out_Data[NC_DATA_H - 2]; //容器  储存计算出来的各个阶段需要的值，
int NC_Data[NC_DATA_H * NC_DATA_V] = //各个阶段的速度控制，建议小曝光时 速度降低 ，反之亦然
{// 起点   终点  比例    积分      微分    动态亮度目标
//     S    E     P       I        D       D_T_Absy     ! ! !  ---> 1000/1000
//	8000,  65535, 5000,  1200,    170,
	1420,  16000,  1200,  300,    0,      100,//低环境亮度
	720,   1400,   1000,  200,     0,     120,
	700,    900,    700, 200,    0,      120,
	200,	   300,	   400, 200,     0,      160,
	0,     20,     10,    0,       0,    120,//高环境亮度
};


float Time_Target[][2] = {
//   时间/ms ， 备用值
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

	int int_proportion = 0;//比例

	static int int_integral = 0;//积分项
	int int_differential = 0;//微分项

	NC_2((int)slExpIdx, NC_Data, NC_DATA_H, NC_DATA_V, Out_Data); //获取基于曝光指数下的各个目标值和配置值

#if( Absy_Dynamic_target == 1) //动态亮度目标
	int_proportion = (Out_Data[3] - AbsyImg);
//	printf("Out_Data[3] == %d", Out_Data[3]); //debug

	int_integral += (Out_Data[3] - AbsyImg);
#else //静态目标
	int_proportion = (TARGET_ABSY - AbsyImg);

	int_integral += (TARGET_ABSY - AbsyImg);
#endif

	int_integral = int_integral >  600 ? 600 : int_integral;
	int_integral = int_integral <  -600 ? -600 : int_integral;

	if (int_proportion < 5)//如果趋于稳定  则开始退积分饱和
	{
		int_integral = (int_integral / 2);
	}

#if(State_CTRL == 1)

	//分段选取稳定范围  抑制最高亮度震荡 这是截断处理
	if ((abs(int_proportion) < 5) && (slExpIdx>20) )
		return;

	if ((abs(int_proportion) < 30) && (slExpIdx<20))
		return;

#endif //State_CTRL
	/*
	if ((slExpIdx <= DEF_ExpIdx_MAX))//小于某个硬件决定的最大曝光指数值  
	{

		//NC_2((int)slExpIdx,NC_Data,5, 3, Out_Data);

		//printf("Out_Data P == %d\n", int((float)int_proportion * (float)PID_P * (float)(Out_Data[0] / 1000)));
		//printf("Out_Data I  ==  %d\n", int((float)int_integral   * (float)PID_I * (float)(Out_Data[1] / 1000)));

		slExpIdx += int((float)int_proportion * (float)PID_P * ((float)Out_Data[0] / (float)1000));
		slExpIdx += int((float)int_integral   * (float)PID_I * ((float)Out_Data[1] / (float)1000));
//		slExpIdx += int((float)int_differential   * (float)PID_D * ((float)Out_Data[2] / (float)1000));
		/*
		//ExpIdx += (long)(int_proportion * (float)(PID_P*(ExpIdx/1400)));
		if (ExpIdx >= ((10 * P_CLK) / (HTS * 1000))) //大于10ms
		{
			ExpIdx += int_proportion * PID_P;
			ExpIdx += int_integral   * PID_I;
		}

		else if(ExpIdx >= 30) //小于10ms 但大于 最小限制曝光行  10 ~ minPixH
		{
			if (int_proportion > 0)//加曝光
			{
				ExpIdx += (int_proportion * PID_P)*0.1;
			}
			else //减曝光
			{
				ExpIdx += (int_proportion * PID_P)*0.1;
			}
			//ExpIdx += (int_integral   * PID_I)*0.1;
		}
		else //处于最亮区域
		{
			if (int_proportion > 0)//加曝光
			{
				ExpIdx++;
			}
			else //减曝光
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

	VideoCapture camera(0); // 0 表示选择默认的摄像头
	if (!camera.isOpened()) {
		std::cerr << "无法打开摄像头" << std::endl;
		return -1;
	}

	camera.set(CV_CAP_PROP_FRAME_WIDTH, 800); //设置开图的分辨率的长
	camera.set(CV_CAP_PROP_FRAME_HEIGHT, 600);//设置开图的分辨率的高

	Mat frame; //申请内存 
	long absy = 0; //当前亮度
	long absy1 = 0; //当前亮度
	camera.set(CAP_PROP_BACKLIGHT, 720); //设置曝光时间初始值 无所谓

	while (1)
	{
		camera.read(frame); //从相机获取图片
		imshow("读取原始视频", frame);  //显示当前帧

		cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

#if(ABSY_MODE ==  MEAN_MODE)
		absy = Absy_get(frame);
#else if(ABSY_MODE ==  HIST_MODE)
		absy = Absy_get_hist(frame); //获取当前图像亮度Absy_get_hist(frame);
#endif


		AE_ExpIdx(absy, camera);//AE计算后设置曝光
		//cv::cvtColor(frame, frame0, cv::COLOR_BGR2GRAY);
		//Edge_Chek(frame0, frame1, 1);
		//imshow("读取处理视频", frame1);  //获得边缘图


		cv::waitKey(5);
	}
	return 0;
}


