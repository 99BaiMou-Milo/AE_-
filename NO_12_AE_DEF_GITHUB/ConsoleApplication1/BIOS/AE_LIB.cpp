#include "AE_LIB.h"

//Math.Ceiling



#if(ABSY_MODE ==  HIST_MODE)

#define hist_bulk  16

int Absy_hist[hist_bulk+1] = {
	// hist_bulk == 16 将数据16等分成16段，然后每一段设置一个权重系数（这个权重系数以16为一倍）
	// 这些系数标明了贡献亮度的大小，这样会导致谁占据主导地位
	// 不太清楚 所有的系数相加是不是该等于 单位值乘以块的数目  这样做貌似是归一化的
	//0, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	// 比如高光优先 倾向变亮 过曝 
	// 	0, 16, 16, 16, 16, 16, 16, 16, 16, 16, 14, 14, 14, 10, 10, 10, 10, //亮度被缩小
	 //	0, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 14, 13, 12, 11, 10, 9, //亮度被缩小
	//	0, 16, 16, 16, 16, 16, 16, 16, 16, 16, 14, 14, 14, 10, 10, 10, 10, //亮度被缩小
	//   0, 20, 20, 20, 20, 18, 18, 18, 16, 16, 14, 14, 14, 12, 12, 12, 12, //归一化 没有放大缩小效果 ，效果OK

	// 比如暗光优先 倾向变暗 欠曝 
	// 0, 24, 24, 20,20, 18, 18, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	// 0, 28, 28, 26, 26, 24, 24, 22, 16, 16, 16, 16, 16, 16, 16, 16, 16, //
	//   0, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 18, 19, 20,//容易震荡 
	 0, 12, 12, 14, 14, 14, 14, 16, 16, 16, 16, 18, 18, 18, 18, 20, 20, //归一化的  没有放大缩小效果
	/*
	0, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	   32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	*/
};

	float Absy_get_hist(Mat& inputImage)
	{
		long add_absy = 0; // 亮度累计
		long add_Pix = 0; //

		Mat outputImage = inputImage.clone();
		int rowNumber = outputImage.rows;
		int colNumber = outputImage.cols * outputImage.channels();  //列数 x 通道数=每一行元素的个数
		int int_idx;
		for (int i = 1; i < rowNumber; i += 3)  //行
		{
			uchar* data = inputImage.ptr<uchar>(i);  //获取第i行的行地址

			for (int j = 1; j < colNumber; j += 3)   //列
			{
				//datao[j] = abs((data1[j] - data1[j + 2]) + (2 * data2[j] - 2 * data2[j + 2]) + (data3[j] - data3[j + 2]));
				int_idx = ceil((float)data[j] / (float)(256 / (float)hist_bulk));
				add_absy += (data[j] * Absy_hist[int_idx]) / hist_bulk;
				//add_absy += data[j];//a < 50 ? 0 : 255;
				add_Pix++;
			}  
		}
		return add_absy / add_Pix;
	}



#else if (ABSY_MODE ==  MEAN_MODE)

//
//tuning UI start
//
#define Skip_Pix   3
//
//tuning UI end
//

#define Skip_MinPix  1
#define Skip        Skip_MinPix+Skip_Pix

	float Absy_get(Mat& inputImage)
	{
		long add_absy = 0;
		long add_Pix = 0;
		Mat outputImage = inputImage.clone();
		int rowNumber = outputImage.rows;
		int colNumber = outputImage.cols * outputImage.channels();  //列数 x 通道数=每一行元素的个数
		for (int i = 1; i < rowNumber; i += Skip)  //行循环
		{
			uchar* data = inputImage.ptr<uchar>(i);  //获取第i行的首地址

			for (int j = 1; j < colNumber; j += Skip)   //列循环
			{
				// ---------【开始处理每个像素】-------------  
				//datao[j] = abs((data1[j] - data1[j + 2]) + (2 * data2[j] - 2 * data2[j + 2]) + (data3[j] - data3[j + 2]));
				add_absy += data[j];//a < 50 ? 0 : 255;
				add_Pix++;
				// ----------【处理结束】---------------------
			}  //行处理结束
		}
		return add_absy / add_Pix;
	}

#endif
