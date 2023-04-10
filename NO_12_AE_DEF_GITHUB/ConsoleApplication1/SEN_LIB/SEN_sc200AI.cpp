#include"SEN_sc200AI.h"

#if(SENSOR_IDX == SC200AI)

	void Set_Exp(VideoCapture CAM, int exprouse){
//		exprouse = exprouse < SEN_ExpIdx_MIN ? SEN_ExpIdx_MIN : exprouse;
//		exprouse = exprouse > SEN_ExpIdx_MAX ? SEN_ExpIdx_MAX : exprouse;
		CAM.set(CAP_PROP_BACKLIGHT, exprouse);
	}

	void Set_G(VideoCapture CAM, int Gain){

		Gain = Gain < SEN_Gain_MIN ? SEN_Gain_MIN : Gain;
		Gain = Gain > SEN_Gain_MAX ? SEN_Gain_MAX : Gain;

		CAM.set(CAP_PROP_GAIN, Gain);
	}


#endif