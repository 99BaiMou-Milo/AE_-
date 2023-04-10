#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>

using namespace cv;

#define SENSOR_IDX SC200AI
#define SC200AI     0

#define HIST_MODE  1 //��ֱ��ͼ��
#define MEAN_MODE  2 // ��ֵ�� 


#define ABSY_MODE   2  //����ͳ��ģʽ����


void Get_interpolation(int absy, int* array, int h, int l, int* Out_Data);

float Absy_get_hist(Mat& inputImage); //ֱ��ͼ��

float Absy_get(Mat& inputImage); //��ֵ��






