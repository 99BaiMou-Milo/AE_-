#ifndef	_NC_H_
#define	_NC_H



 #define Get_2DArray_ColSize(array)      (sizeof(array[0])/sizeof(array[0][0]))
 #define Get_2DArray_RowSize(array)		(sizeof(array)/sizeof(array[0]))
 
/*
int Out_Data[5]; 
int NC_Data[][5]=
{
//  ABSY_S   ABSY_E    DATA_1   DATA_2   DATA_3	
    { 350,    500,        36,     38,        30},
    { 260,    300,         2,      3,        90},
	{ 120,    180,         7,      9,       190},
	{ 40,     80,          5,      6,       170},
	{  0,     20,          3,      5,       150},
  };
*/

float Xielv(float a_X,float a_Y,float b_X,float b_Y);
int NC_1(int absy,float x1,float y1,float x2,float y2);
int NC_2(int absy,int* array,int h,int l,int* Out_Data);





#endif
