#include<stdio.h>
#include "NC.h"

float Xielv(float a_X,float a_Y,float b_X,float b_Y)//计算斜率 
{
	return ( ( b_Y-a_Y ) / (  b_X-a_X ) );	
}

int NC_2(int absy,int* array,int h,int l,int* Out_Data)//插值实现方法 
{
	int i=0;
	int j;
   for(i=0;i<l;i++)        //while 
   {
   	if( ( absy>*( array +(h*i) ) )  &&  ( absy<*( (array + (h*i) )+1) ) ) //while 3
   	 {
  	        for(j=0;j<(h-2);j++)
			   {
			   	(*(Out_Data+j))= (*(array+(i*h)+2+j));
				} 
				return 0;	
		}
   }      
   
   for(i=0;i<l;i++)
   {
        if( absy >= *( array +1 ) )//ABSY > Max_Absy s2
        {
           for(j=0;j<(h-2);j++)
			   {
			   	(*(Out_Data+j))= (*(array+(i*h)+2+j));
				}  	
				return 0;
		}
		else if(absy==0)//ABSY = 0    s1
		{
		   for(j=0;j<(h-2);j++)
			   {
			   	(*(Out_Data+j))= (*(array+((l-1)*h)+2+j));
				}
				return 0; 	
		}
		else if( ( absy < *( array +(h*i) ) )  &&  ( absy >  *( (array + (h*(i+1)) )+1)   ) ) //absy out domain
		{
		       for(j=0;j<(h-2);j++)
			   {   	
			   	////////////////////////////////////////////
			   	////线性插值核心 
			   	(*(Out_Data+j))=NC_1(absy,
				     (float)(*( (array + (h*(i+1)) )+1)),
				     (float)(*( (array + (h*(i+1)) )+2+j)),
				     (float)(*( array +(h*i) )),//120
				     (float)(*( array +(h*i) +2+j ))
					     );     
				}  	
				return 0;
		}
   } 
}

int NC_1(int absy,float x1,float y1,float x2,float y2)
{
   return (Xielv(x1,y1,x2,y2)>0)?((((absy-x1)*(Xielv(x1,y1,x2,y2))+y1)-(int)((absy-x1)*(Xielv(x1,y1,x2,y2))+y1))>=(0.6))?(( (absy-x1) * (Xielv(x1,y1,x2,y2)) + y1 +1 )):( (absy-x1) * (Xielv(x1,y1,x2,y2)) + y1 ):((((absy-x1)*(Xielv(x1,y1,x2,y2))+y1)-(int)((absy-x1)*(Xielv(x1,y1,x2,y2))+y1))>(0.6) )?( (absy-x1) * (Xielv(x1,y1,x2,y2)) + y1 +1 ):( (absy-x1) * (Xielv(x1,y1,x2,y2)) + y1 );  
} 


