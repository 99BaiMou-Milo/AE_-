#include<stdio.h>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

#if(SENSOR_IDX == SC200AI)

	#define P_CLK 154800000
	#define HTS 2200  //  1100  60 PFS  2200 30FPS 

	#define SEN_ExpIdx_MAX 2560 
	#define SEN_ExpIdx_MIN 0

	#define SEN_Gain_MAX 54600 
	#define SEN_Gain_MIN 0

	void Set_Exp(VideoCapture CAM, int a);

	void Set_G(VideoCapture CAM, int a);

#endif
