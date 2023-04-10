#include "AE_LIB.h"

//Math.Ceiling



#if(ABSY_MODE ==  HIST_MODE)

#define hist_bulk  16

int Absy_hist[hist_bulk+1] = {
	// hist_bulk == 16 ������16�ȷֳ�16�Σ�Ȼ��ÿһ������һ��Ȩ��ϵ�������Ȩ��ϵ����16Ϊһ����
	// ��Щϵ�������˹������ȵĴ�С�������ᵼ��˭ռ��������λ
	// ��̫��� ���е�ϵ������ǲ��Ǹõ��� ��λֵ���Կ����Ŀ  ������ò���ǹ�һ����
	//0, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	// ����߹����� ������� ���� 
	// 	0, 16, 16, 16, 16, 16, 16, 16, 16, 16, 14, 14, 14, 10, 10, 10, 10, //���ȱ���С
	 //	0, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 14, 13, 12, 11, 10, 9, //���ȱ���С
	//	0, 16, 16, 16, 16, 16, 16, 16, 16, 16, 14, 14, 14, 10, 10, 10, 10, //���ȱ���С
	//   0, 20, 20, 20, 20, 18, 18, 18, 16, 16, 14, 14, 14, 12, 12, 12, 12, //��һ�� û�зŴ���СЧ�� ��Ч��OK

	// ���簵������ ����䰵 Ƿ�� 
	// 0, 24, 24, 20,20, 18, 18, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	// 0, 28, 28, 26, 26, 24, 24, 22, 16, 16, 16, 16, 16, 16, 16, 16, 16, //
	//   0, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 18, 19, 20,//������ 
	 0, 12, 12, 14, 14, 14, 14, 16, 16, 16, 16, 18, 18, 18, 18, 20, 20, //��һ����  û�зŴ���СЧ��
	/*
	0, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	   32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	*/
};

	float Absy_get_hist(Mat& inputImage)
	{
		long add_absy = 0; // �����ۼ�
		long add_Pix = 0; //

		Mat outputImage = inputImage.clone();
		int rowNumber = outputImage.rows;
		int colNumber = outputImage.cols * outputImage.channels();  //���� x ͨ����=ÿһ��Ԫ�صĸ���
		int int_idx;
		for (int i = 1; i < rowNumber; i += 3)  //��
		{
			uchar* data = inputImage.ptr<uchar>(i);  //��ȡ��i�е��е�ַ

			for (int j = 1; j < colNumber; j += 3)   //��
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
		int colNumber = outputImage.cols * outputImage.channels();  //���� x ͨ����=ÿһ��Ԫ�صĸ���
		for (int i = 1; i < rowNumber; i += Skip)  //��ѭ��
		{
			uchar* data = inputImage.ptr<uchar>(i);  //��ȡ��i�е��׵�ַ

			for (int j = 1; j < colNumber; j += Skip)   //��ѭ��
			{
				// ---------����ʼ����ÿ�����ء�-------------  
				//datao[j] = abs((data1[j] - data1[j + 2]) + (2 * data2[j] - 2 * data2[j + 2]) + (data3[j] - data3[j + 2]));
				add_absy += data[j];//a < 50 ? 0 : 255;
				add_Pix++;
				// ----------�����������---------------------
			}  //�д������
		}
		return add_absy / add_Pix;
	}

#endif
