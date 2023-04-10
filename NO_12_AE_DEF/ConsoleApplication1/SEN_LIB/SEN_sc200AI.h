#include<stdio.h>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

#define P_CLK 154800000
#define HTS 2200  //  1100  60 PFS  2200 30FPS 

void Set_Exp(VideoCapture CAM, int a);

void Set_G(VideoCapture CAM, int a);
