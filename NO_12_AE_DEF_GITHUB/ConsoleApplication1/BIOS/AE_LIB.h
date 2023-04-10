#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>

using namespace cv;

#define SENSOR_IDX SC200AI
#define SC200AI     0

#define HIST_MODE  1 //带直方图法
#define MEAN_MODE  2 // 均值法 


#define ABSY_MODE   2  //亮度统计模式控制


void Get_interpolation(int absy, int* array, int h, int l, int* Out_Data);

float Absy_get_hist(Mat& inputImage); //直方图法

float Absy_get(Mat& inputImage); //均值法






